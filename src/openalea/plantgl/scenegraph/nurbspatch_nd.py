from . import _pglsg as sg
from openalea.plantgl.math import Vector4, Vector3
import numpy as np
import itertools

class NurbsPatch3D:
    def __init__(self, points, 
                 udegree = 3, vdegree = 3, wdegree = 3, 
                 uknotList = None, vknotList = None, wknotList = None,
                 ustride = 30, vstride = 30, wstride = 30):
        dim = (len(points),len(points[0]),len(points[0][0]))
        self.points = np.empty( dim, dtype=object )
        for index in itertools.product(*[list(range(d)) for d in dim]):
            self.points[index] = points[index[0]][index[1]][index[2]]
        self.udegree = udegree
        self.__udim = len(points)
        self.vdegree = vdegree
        self.__vdim = len(points[0])
        self.wdegree = wdegree
        self.__wdim = len(points[0][0])
        self.uknotList = uknotList
        if self.uknotList is None:
            self.uknotList = sg.NurbsCurve.defaultKnotList(self.__udim,self.udegree)
        self.vknotList = vknotList
        if self.vknotList is None:
            self.vknotList = sg.NurbsCurve.defaultKnotList(self.__vdim,self.vdegree)
        self.wknotList = wknotList
        if self.wknotList is None:
            self.wknotList = sg.NurbsCurve.defaultKnotList(self.__wdim,self.wdegree)
        self.ustride = ustride
        self.vstride = vstride
        self.wstride = wstride
        self.name = ''
        assert self.isValid()

    def isValid(self):
        """ Test whether self is valid or not """
        usize = len(self.points) 
        assert usize > 1 
        vsize = len(self.points[0])
        assert vsize > 1 
        wsize = len(self.points[0][0])
        assert wsize > 1
        assert not self.uknotList is None and len(self.uknotList) > self.__udim + 2
        assert not self.vknotList is None and len(self.vknotList) > self.__vdim + 2
        assert not self.wknotList is None and len(self.wknotList) > self.__wdim + 2
        return True
        
    def getPointAt(self,u,v,w):
        """ Compute point at (u,v,w) """
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(u,udeg,self.uknotList)        
        Nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknotList)

        vdeg = self.vdegree
        vspan = sg.NurbsCurve.findSpan(v,vdeg,self.vknotList)
        Nv = sg.NurbsCurve.basisFunctions(vspan, v, vdeg, self.vknotList)

        wdeg = self.wdegree
        wspan = sg.NurbsCurve.findSpan(w,wdeg,self.wknotList)
        Nw = sg.NurbsCurve.basisFunctions(wspan, w, wdeg, self.wknotList)
        
        tmp = [[None for i in range(vdeg+1)] for j in range(wdeg+1)]
        for i in range(0,wdeg+1):
            for j in range(0,vdeg+1):
                tmpVec = Vector4(0,0,0,0)
                for k in range(0,udeg+1):
                    tmpVec += self.points[uspan-udeg+k][vspan-vdeg+j][wspan-wdeg+i] * Nu[k]
                tmp[i][j] = tmpVec
                
        tmp2 = [None for i in range(wdeg+1)]
        for i in range(0,wdeg+1):
            tmpVec = Vector4(0,0,0,0)
            for j in range(0,vdeg+1):
                tmpVec += tmp[i][j] * Nv[j]
            tmp2[i] = tmpVec
        
        res = Vector4(0,0,0,0)
        for i in range(0,wdeg+1):
            res += tmp2[i] * Nw[i]
            
        if res.w != 0:
            return res.project()
        else:
            return Vector3(res.x,res.y,res.z)
        
    def getWDerivativeAt(self,u,v,w):
        """ Compute point at (u,v,w) """
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(u,udeg,self.uknotList)        
        Nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknotList)
        
        vdeg = self.vdegree
        vspan = sg.NurbsCurve.findSpan(v,vdeg,self.vknotList)
        Nv = sg.NurbsCurve.basisFunctions(vspan, v, vdeg, self.vknotList)
        
        wdeg = self.wdegree
        wspan = sg.NurbsCurve.findSpan(w,wdeg,self.wknotList)
        Nw = sg.NurbsCurve.derivatesBasisFunctions(1, w, wspan, wdeg, self.wknotList)
        
        tmp = [[None for i in range(vdeg+1)] for j in range(wdeg+1)]
        for i in range(0,wdeg+1):
            for j in range(0,vdeg+1):
                tmpVec = Vector4(0,0,0,0)
                for k in range(0,udeg+1):
                    tmpVec += self.points[uspan-udeg+k][vspan-vdeg+j][wspan-wdeg+i] * Nu[k]
                tmp[i][j] = tmpVec
        
        tmp2 = [None for i in range(self.wdegree+1)]
        for i in range(0,wdeg+1):
            tmpVec = Vector4(0,0,0,0)
            for j in range(0,vdeg+1):
                tmpVec += tmp[i][j] * Nv[j]
            tmp2[i] = tmpVec
        
        res = [Vector4() for i in range(2)]
        for j in range(2):
          for i in range(0,wdeg+1):
            res[j] += tmp2[i] * Nw[j,i]
        return res[1]

    def getWPatch(self,w):
        wdeg = self.wdegree
        wspan = sg.NurbsCurve.findSpan(w,wdeg,self.wknotList)
        Nw = sg.NurbsCurve.basisFunctions(wspan, w, wdeg, self.wknotList)
       
        tmp = [[None for j in range(self.__vdim)] for i in range(self.__udim)]
        for i in range(0,self.__udim):
            for j in range(0,self.__vdim):
                tmpVec = Vector4(0,0,0,0)
                for k in range(0,wdeg+1):
                    tmpVec += self.points[i][j][wspan-wdeg+k] * Nw[k]
                tmp[i][j] = tmpVec
        return sg.NurbsPatch(tmp,self.uknotList,self.vknotList,self.udegree,self.vdegree,self.ustride,self.vstride)

    def getUPatch(self,u):
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(u,udeg,self.uknotList)        
        Nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknotList)
       
        tmp = [[None for j in range(self.__wdim)] for i in range(self.__vdim)]
        for i in range(0,self.__vdim):
            for j in range(0,self.__wdim):
                tmpVec = Vector4(0,0,0,0)
                for k in range(0,udeg+1):
                    tmpVec += self.points[uspan-udeg+k][i][j] * Nu[k]
                tmp[i][j] = tmpVec
        return sg.NurbsPatch(tmp,self.vknotList,self.wknotList,self.vdegree,self.wdegree,self.vstride,self.wstride)

    def getVPatch(self,v):
        vdeg = self.vdegree
        vspan = sg.NurbsCurve.findSpan(v,vdeg,self.vknotList)
        Nv = sg.NurbsCurve.basisFunctions(vspan, v, vdeg, self.vknotList)
       
        tmp = [[None for j in range(self.__wdim)] for i in range(self.__udim)]
        for i in range(0,self.__udim):
            for j in range(0,self.__wdim):
                tmpVec = Vector4(0,0,0,0)
                for k in range(0,vdeg+1):
                    tmpVec += self.points[i][vspan-vdeg+k][j] * Nv[k]
                tmp[i][j] = tmpVec
        return sg.NurbsPatch(tmp,self.uknotList,self.wknotList,self.udegree,self.wdegree,self.ustride,self.wstride)
        
    def getBoundingBox(self):
        p1 = self.points[0,0,0]
        p2 = self.points[0,0,1]
        bbx = sg.BoundingBox(p1.project(),p2.project())
        for index in itertools.product(*[list(range(d)) for d in self.points.shape]):
            bbx.extend(self.points[index].project())
        return bbx

    def getUVIsoSectionAt(self,u,v):
        return self.getUPatch(u).getUIsoSectionAt(v)

    def getUWIsoSectionAt(self,u,w):
        return self.getUPatch(u).getVIsoSectionAt(w)

    def getVWIsoSectionAt(self,v,w):
        return self.getVPatch(v).getVIsoSectionAt(w)

    def __repr__(self):
        res = 'NurbsPatch3D('+repr(self.points.tolist())+','
        res += repr(self.udegree)+','+repr(self.vdegree)+','+repr(self.wdegree)+','
        res += repr(self.uknotList) +','+repr(self.vknotList) +','+repr(self.wknotList) +','
        res += repr(self.ustride)+','+repr(self.vstride)+','+repr(self.wstride)+')'
        return res

    @staticmethod
    def default():
        return NurbsPatch3D([[[Vector4(i-1.5,j-1.5,k-1.5,1) for k in range(4)] for j in range(4)] for i in range(4)], ustride=5,vstride=5,wstride=5)