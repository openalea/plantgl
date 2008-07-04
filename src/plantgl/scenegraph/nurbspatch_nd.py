import _pglsg as sg

class NurbsPatch3D:
    def __init__(self, points, 
                 udeg = 3, tdeg = 3, tdeg = 3, 
                 uknots = None, vknots = None, tknots = None,
                 ustride = 30, vstride = 30, tstride = 30):
        self.points = points
        self.udegree = udeg
        self.udim = len(points)
        self.vdegree = vdeg
        self.vdim = len(points[0])
        self.tdegree = tdeg
        self.tdim = len(points[0][0])
        self.uknots = uknots
        if self.uknots is None:
            self.uknots = sg.NurbsCurve.defaultKnotList(self.udim,self.udegree)
        self.vknots = vknots
        if self.vknots is None:
            self.vknots = sg.NurbsCurve.defaultKnotList(self.vdim,self.vdegree)
        self.tknots = tknots
        if self.tknots is None:
            self.tknots = sg.NurbsCurve.defaultKnotList(self.tdim,self.tdegree)
        self.ustride = ustride
        self.vstride = vstride
        self.tstride = tstride
        assert self.isValid()
    def isValid(self):
        """ Test whether self is valid or not """
        usize = len(self.points) 
        assert usize > 1 
        vsize = len(self.points[0])
        assert vsize > 1 
        tsize = len(self.points[0][0])
        assert tsize > 1
        assert not self.uknot is None and len(self.uknot) < self.udim + 2
        assert not self.vknot is None and len(self.vknot) < self.vdim + 2
        assert not self.tknot is None and len(self.tknot) < self.tdim + 2
    def getPointAt(self,u,v,t):
        """ Compute point at (u,v,t) """
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(udeg,self.uknots,u)        
        nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknots)
        vdeg = self.vdegree
        vspan = sg.NurbsCurve.findSpan(vdeg,self.vknots,v)
        nv = sg.NurbsCurve.basisFunctions(vspan, v, vdeg, self.vknots)
        tdeg = self.tdegree
        tspan = sg.NurbsCurve.findSpan(tdeg,self.tknots,t)
        nt = sg.NurbsCurve.basisFunctions(tspan, t, tdeg, self.tknots)
        tmp = [[None for i in xrange(self.vdegree+1)] for i in xrange(self.tdegree+1)]
        for i in xrange(0,self.tdegree):
            for j in xrange(0,self.vdegree):
                tmpVec = Vector4()
                for k in xrange(0,self.udegree):
                    tmpVec += self.points[uspan-udeg+k][vspan-vdeg+j][tspan-tdeg-i] * Nu[k]
                tmp[i][j] = tmpVec
        tmp2 = [None for i in xrange(self.tdegree+1)]
        for i in xrange(0,self.tdegree):
            tmpVec = Vector4()
            for j in xrange(0,self.vdegree):
                tmpVec += tmp[j][i] * Nv[j]
        res = Vector4()
        for i in xrange(0,self.tdegree):
            res += tmp2[i] * Nv[i]
        return res
        
    def getParametrization(self,p):
        """ return (u,v,t) """
    def getTDerivativeAt(self,u,v,t):
        """ Compute point at (u,v,t) """
        udeg = self.udegree
        uspan = sg.NurbsCurve.findSpan(udeg,self.uknots,u)        
        nu = sg.NurbsCurve.basisFunctions(uspan, u, udeg, self.uknots)
        vdeg = self.vdegree
        vspan = sg.NurbsCurve.findSpan(vdeg,self.vknots,v)
        nv = sg.NurbsCurve.basisFunctions(vspan, v, vdeg, self.vknots)
        tdeg = self.tdegree
        tspan = sg.NurbsCurve.findSpan(tdeg,self.tknots,t)
        nt = sg.NurbsCurve.derivatesBasisFunctions(1, t, tspan, tdeg, self.tknots)
        tmp = [[None for i in xrange(self.vdegree+1)] for i in xrange(self.tdegree+1)]
        for i in xrange(0,self.tdegree):
            for j in xrange(0,self.vdegree):
                tmpVec = Vector4()
                for k in xrange(0,self.udegree):
                    tmpVec += self.points[uspan-udeg+k][vspan-vdeg+j][tspan-tdeg-i] * Nu[k]
                tmp[i][j] = tmpVec
        tmp2 = [None for i in xrange(self.tdegree+1)]
        for i in xrange(0,self.tdegree):
            tmpVec = Vector4()
            for j in xrange(0,self.vdegree):
                tmpVec += tmp[j][i] * Nv[j]
        res = Vector4()
        for i in xrange(0,self.tdegree):
            res += tmp2[i] * Nv[i]
        return res
    
        
        