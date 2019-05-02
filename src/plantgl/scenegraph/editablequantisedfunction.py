from . import _pglsg

class EditableQuantisedFunction (object):
    def __init__(self, curve, sampling = _pglsg.QuantisedFunction.DEFAULT_SAMPLING, clamped = _pglsg.QuantisedFunction.DEFAULT_CLAMPED):
        self.setCurve(curve)
        self.__update(sampling, clamped)

    def __call__(self, x):
        if not self.__up_to_date: self.__update()
        return self.__qfunc(x)

    def __update(self, sampling = None, clamped = None):
            if not clamped : clamped = self.__qfunc.clamped
            if not sampling : sampling = self.__qfunc.sampling
            self.__qfunc = _pglsg.QuantisedFunction(self.__originalcurve, sampling)
            self.__qfunc.clamped = clamped
            self.firstx, self.lastx = self.__qfunc.firstx, self.__qfunc.lastx
            self.__up_to_date    = True

    def getCurve(self):
        """ Return a copy of the curve used to compute the function """
        return self.__originalcurve

    def setCurve(self, curve):
        """ Set the curve to quantify  as a function """
        import copy
        if not isinstance(curve,_pglsg.Curve2D):
            self.__originalcurve = _pglsg.Polyline2D(curve)
        else:
            self.__originalcurve = copy.deepcopy(curve)
        self.__up_to_date    = False

    curve = property(getCurve, setCurve)

    def setclamped(self, clamped):
        self.__qfunc.clamped = clamped
    def getclamped(self):
        return self.__qfunc.clamped

    clamped = property(getclamped, setclamped)

    def __deepcopy__(self, memo):
        return EditableQuantisedFunction(self.__originalcurve,self.__qfunc.sampling,self.__qfunc.clamped)

    def __repr__(self):
        return self.__class__.__name__+'('+repr(self.__originalcurve)+','+repr(self.__qfunc.sampling)+','+repr(self.__qfunc.clamped)+')'


def test():
    import openalea.plantgl.scenegraph.editablequantisedfunction as eqf
    reload(eqf)
    f = eqf.EditableQuantisedFunction([(0,0),(1,1)])
    print(f(0.5))
    assert f(0.5) == 0.5
    f.curve = [(0,0),(1,3)]
    print(f.curve)
    print(f(0.5))
    assert f(0.5) == 1.5

