from . import _pglsg as sg
from openalea.plantgl.math import Vector4, Vector3
import numpy as np
import itertools

class NurbsPatch3D:
    def __init__(self, points, 
                 udegree = 3, vdegree = 3, wdegree = 3, 
                 uknotList = None, vknotList = None, wknotList = None,
                 ustride = 30, vstride = 30, wstride = 30):
        self.udegree = udegree
        self.vdegree = vdegree
        self.wdegree = wdegree

        self.setCtrlPointMatrix(points)

        self.uknotList = uknotList if not uknotList is None else sg.NurbsCurve.defaultKnotList(self.__udim,self.udegree)
        self.vknotList = vknotList if not vknotList is None else sg.NurbsCurve.defaultKnotList(self.__vdim,self.vdegree)
        self.wknotList = wknotList if not wknotList is None else sg.NurbsCurve.defaultKnotList(self.__wdim,self.wdegree)

        self.ustride = ustride
        self.vstride = vstride
        self.wstride = wstride

        self.name = ''

        assert self.isValid()

    def _getFirstUKnot(self): return self.uknotList[0]
    firstUKnot = property(_getFirstUKnot)

    def _getFirstVKnot(self): return self.vknotList[0]
    firstVKnot = property(_getFirstVKnot)

    def _getFirstWKnot(self): return self.wknotList[0]
    firstWKnot = property(_getFirstWKnot)

    def _getLastUKnot(self): return self.uknotList[-1]
    lastUKnot = property(_getLastUKnot)

    def _getLastVKnot(self): return self.vknotList[-1]
    lastVKnot = property(_getLastVKnot)

    def _getLastWKnot(self): return self.wknotList[-1]
    lastWKnot = property(_getLastWKnot)

    def getCtrlPointMatrix(self):
        return self.__points

    def setCtrlPointMatrix(self, points):
        dim = (len(points),len(points[0]),len(points[0][0]))

        assert 1 <= self.udegree < dim[0]
        assert 1 <= self.vdegree < dim[1]
        assert 1 <= self.wdegree < dim[2]

        if not isinstance(points, (np.ndarray, np.generic)):
            self.__points = np.empty( dim, dtype=object )
            for index in itertools.product(*[list(range(d)) for d in dim]):
                self.__points[index] = points[index[0]][index[1]][index[2]]
        else:
            self.__points = points

        self.__udim = dim[0]
        self.__vdim = dim[1]
        self.__wdim = dim[2]

    ctrlPointMatrix = property(getCtrlPointMatrix, setCtrlPointMatrix)

    def isValid(self):
        """ Test whether self is valid or not """

        assert self.__udim == len(self.__points)
        assert self.__vdim == len(self.__points[0])
        assert self.__wdim == len(self.__points[0][0])

        assert 1 <= self.udegree < self.__udim
        assert 1 <= self.vdegree < self.__vdim
        assert 1 <= self.wdegree < self.__wdim

        assert not self.uknotList is None and len(self.uknotList) > self.__udim + 1
        assert not self.vknotList is None and len(self.vknotList) > self.__vdim + 1
        assert not self.wknotList is None and len(self.wknotList) > self.__wdim + 1

        return True
    
    def isUKnotListToDefault(self):
        return list(self.uknotList) == list(sg.NurbsCurve.defaultKnotList(self.__udim,self.udegree))

    def isVKnotListToDefault(self):
        return list(self.vknotList) == list(sg.NurbsCurve.defaultKnotList(self.__vdim,self.vdegree))

    def isWKnotListToDefault(self):
        return list(self.wknotList) == list(sg.NurbsCurve.defaultKnotList(self.__wdim,self.wdegree))

    def isUDegreeToDefault(self):
        return self.udegree == sg.NurbsCurve.DEFAULT_NURBS_DEGREE

    def isVDegreeToDefault(self):
        return self.vdegree == sg.NurbsCurve.DEFAULT_NURBS_DEGREE

    def isWDegreeToDefault(self):
        return self.wdegree == sg.NurbsCurve.DEFAULT_NURBS_DEGREE

    def isUStrideToDefault(self):
        return self.ustride == sg.NurbsCurve.DEFAULT_STRIDE

    def isVStrideToDefault(self):
        return self.vstride == sg.NurbsCurve.DEFAULT_STRIDE

    def isWStrideToDefault(self):
        return self.wstride == sg.NurbsCurve.DEFAULT_STRIDE

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
                    tmpVec += self.__points[uspan-udeg+k][vspan-vdeg+j][wspan-wdeg+i].wtoxyz() * Nu[k]
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
                    tmpVec += self.__points[uspan-udeg+k][vspan-vdeg+j][wspan-wdeg+i] * Nu[k]
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


    def getUPatch(self,u):
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(u,udeg,self.uknotList)        
        Nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknotList)
       
        tmp = [[None for j in range(self.__wdim)] for i in range(self.__vdim)]
        for i in range(0,self.__vdim):
            for j in range(0,self.__wdim):
                tmpVec = Vector4(0,0,0,0)
                for k in range(0,udeg+1):
                    npi = Vector4(self.__points[uspan-udeg+k][i][j]).wtoxyz()
                    tmpVec += npi * Nu[k]
                tmpVec.x/=tmpVec.w
                tmpVec.y/=tmpVec.w
                tmpVec.z/=tmpVec.w
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
                    tmpVec += self.__points[i][vspan-vdeg+k][j].wtoxyz() * Nv[k]
                tmpVec.x/=tmpVec.w
                tmpVec.y/=tmpVec.w
                tmpVec.z/=tmpVec.w
                tmp[i][j] = tmpVec
        return sg.NurbsPatch(tmp,self.uknotList,self.wknotList,self.udegree,self.wdegree,self.ustride,self.wstride)

    def getWPatch(self,w):
        wdeg = self.wdegree
        wspan = sg.NurbsCurve.findSpan(w,wdeg,self.wknotList)
        Nw = sg.NurbsCurve.basisFunctions(wspan, w, wdeg, self.wknotList)
       
        tmp = [[None for j in range(self.__vdim)] for i in range(self.__udim)]
        for i in range(0,self.__udim):
            for j in range(0,self.__vdim):
                tmpVec = Vector4(0,0,0,0)
                for k in range(0,wdeg+1):
                    tmpVec += self.__points[i][j][wspan-wdeg+k].wtoxyz() * Nw[k]
                tmpVec.x/=tmpVec.w
                tmpVec.y/=tmpVec.w
                tmpVec.z/=tmpVec.w
                tmp[i][j] = tmpVec
        return sg.NurbsPatch(tmp,self.uknotList,self.vknotList,self.udegree,self.vdegree,self.ustride,self.vstride)
        
    def getBoundingBox(self):
        def toV3(p): return Vector3(p.x,p.y,p.z)
        p1 = self.__points[0,0,0]
        p2 = self.__points[0,0,1]
        bbx = sg.BoundingBox(toV3(p1),toV3(p2))
        for index in itertools.product(*[list(range(d)) for d in self.__points.shape]):
            bbx.extend(toV3(self.__points[index]))
        return bbx

    def getIsoUVSectionAt(self,u,v):
        return self.getUPatch(u).getIsoUSectionAt(v)

    def getIsoUWSectionAt(self,u,w):
        return self.getUPatch(u).getIsoVSectionAt(w)

    def getIsoVWSectionAt(self,v,w):
        return self.getVPatch(v).getIsoVSectionAt(w)

    def __repr__(self):
        res = 'NurbsPatch3D('+repr(self.__points.tolist())
        if not self.isUDegreeToDefault()   : res += ', udegree='+repr(self.udegree)
        if not self.isVDegreeToDefault()   : res += ', vdegree='+repr(self.vdegree)
        if not self.isWDegreeToDefault()   : res += ', wdegree='+repr(self.wdegree)
        if not self.isUKnotListToDefault() : res += ', uknotList='+repr(self.uknotList) 
        if not self.isVKnotListToDefault() : res += ', vknotList='+repr(self.vknotList) 
        if not self.isWKnotListToDefault() : res += ', wknotList='+repr(self.wknotList) 
        if not self.isUStrideToDefault()   : res += ', ustride='+repr(self.ustride)
        if not self.isVStrideToDefault()   : res += ', vstride='+repr(self.vstride)
        if not self.isWStrideToDefault()   : res += ', wstride='+repr(self.wstride)
        res+=')'
        return res

    @staticmethod
    def default(xsize = 4, ysize = 4, zsize = 4):
        """ return a default nurbs patch """
        dx = 1/(xsize-1)
        dy = 1/(ysize-1)
        dz = 1/(zsize-1)
        udegree = min(xsize-1,3)
        vdegree = min(ysize-1,3)
        wdegree = min(zsize-1,3)
        return NurbsPatch3D([[[Vector4(i*dx-0.5,j*dy-0.5,k*dz-0.5,1) for k in range(zsize)] for j in range(ysize)] for i in range(xsize)], 
                            udegree=udegree,vdegree=vdegree,wdegree=wdegree,
                            ustride=5,vstride=5,wstride=5)
