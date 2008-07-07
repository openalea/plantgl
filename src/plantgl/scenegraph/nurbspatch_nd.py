import _pglsg as sg
from openalea.plantgl.math import Vector4

class NurbsPatch3D:
    def __init__(self, points, 
                 udeg = 3, vdeg = 3, wdeg = 3, 
                 uknots = None, vknots = None, wknots = None,
                 ustride = 30, vstride = 30, wstride = 30):
        self.points = points
        self.udegree = udeg
        self.udim = len(points)
        self.vdegree = vdeg
        self.vdim = len(points[0])
        self.wdegree = wdeg
        self.wdim = len(points[0][0])
        self.uknots = uknots
        if self.uknots is None:
            self.uknots = sg.NurbsCurve.defaultKnotList(self.udim,self.udegree)
        self.vknots = vknots
        if self.vknots is None:
            self.vknots = sg.NurbsCurve.defaultKnotList(self.vdim,self.vdegree)
        self.wknots = wknots
        if self.wknots is None:
            self.wknots = sg.NurbsCurve.defaultKnotList(self.wdim,self.wdegree)
        self.ustride = ustride
        self.vstride = vstride
        self.wstride = wstride
        assert self.isValid()
    def isValid(self):
        """ Test whether self is valid or not """
        usize = len(self.points) 
        assert usize > 1 
        vsize = len(self.points[0])
        assert vsize > 1 
        wsize = len(self.points[0][0])
        assert wsize > 1
        assert not self.uknots is None and len(self.uknots) > self.udim + 2
        assert not self.vknots is None and len(self.vknots) > self.vdim + 2
        assert not self.wknots is None and len(self.wknots) > self.wdim + 2
        return True
    def getPointAt(self,u,v,w):
        """ Compute point at (u,v,w) """
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(u,udeg,self.uknots)        
        Nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknots)
        vdeg = self.vdegree
        vspan = sg.NurbsCurve.findSpan(v,vdeg,self.vknots)
        Nv = sg.NurbsCurve.basisFunctions(vspan, v, vdeg, self.vknots)
        wdeg = self.wdegree
        wspan = sg.NurbsCurve.findSpan(w,wdeg,self.wknots)
        Nw = sg.NurbsCurve.basisFunctions(wspan, w, wdeg, self.wknots)
        tmp = [[None for i in xrange(self.vdegree+1)] for i in xrange(self.wdegree+1)]
        for i in xrange(0,self.wdegree):
            for j in xrange(0,self.vdegree):
                tmpVec = Vector4()
                for k in xrange(0,self.udegree):
                    tmpVec += self.points[uspan-udeg+k][vspan-vdeg+j][wspan-wdeg-i] * Nu[k]
                tmp[i][j] = tmpVec
        tmp2 = [None for i in xrange(self.wdegree+1)]
        for i in xrange(0,self.wdegree):
            tmpVec = Vector4()
            for j in xrange(0,self.vdegree):
                tmpVec += tmp[j][i] * Nv[j]
            tmp2[i] = tmpVec
        res = Vector4()
        for i in xrange(0,self.wdegree):
            res += tmp2[i] * Nw[i]
        return res
        
    def getParametrization(self,p):
        """ return (u,v,t) """
    def getTDerivativeAt(self,u,v,w):
        """ Compute point at (u,v,w) """
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(u,udeg,self.uknots)        
        Nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknots)
        vdeg = self.vdegree
        vspan = sg.NurbsCurve.findSpan(v,vdeg,self.vknots)
        Nv = sg.NurbsCurve.basisFunctions(vspan, v, vdeg, self.vknots)
        wdeg = self.wdegree
        wspan = sg.NurbsCurve.findSpan(w,wdeg,self.wknots)
        Nw = sg.NurbsCurve.derivatesBasisFunctions(1, w, wspan, wdeg, self.wknots)
        tmp = [[None for i in xrange(self.vdegree+1)] for i in xrange(self.wdegree+1)]
        for i in xrange(0,self.wdegree):
            for j in xrange(0,self.vdegree):
                tmpVec = Vector4()
                for k in xrange(0,self.udegree):
                    tmpVec += self.points[uspan-udeg+k][vspan-vdeg+j][wspan-wdeg-i] * Nu[k]
                tmp[i][j] = tmpVec
        tmp2 = [None for i in xrange(self.wdegree+1)]
        for i in xrange(0,self.wdegree):
            tmpVec = Vector4()
            for j in xrange(0,self.vdegree):
                tmpVec += tmp[j][i] * Nv[j]
            tmp2[i] = tmpVec
        res = Vector4()
        for i in xrange(0,self.wdegree):
            res += tmp2[i] * Nw[i]
        return res
    
        
        