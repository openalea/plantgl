# -*- python -*-
#
#
#       Copyright 2006-2010 INRIA - CIRAD - INRA  
#
#       File author(s): Pradal, Boudon, Barbeau 
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
"""declaration of curve2D interface and its widget """

__license__ = "Cecill-C"
__revision__ = " $Id: interface.py 2245 2010-02-08 17:11:34Z cokelaer $"

from openalea.core.interface import IInterface, IInterfaceMetaClass
from openalea.plantgl.scenegraph import Curve2D, NurbsCurve2D, NurbsPatch

class ICurve2D(IInterface) :
    """
    interface for different curves 2D
    """
    __pytype__ = Curve2D
    def __init__ (self,func_constraint = False, **kargs) :
        IInterface.__init__(self, **kargs)
        self.func_constraint = func_constraint
        
    @classmethod
    def default(cls):
        return NurbsCurve2D([(0.,0.,1.),(1/3.,0.,1.),(2/3.,0.,1.),(1.,0.,1.)])


    def __repr__(self):
        return 'ICurve2D(func_constraint=%s)' % self.func_constraint


class INurbsPatch(IInterface) :
    """
    interface for Nurbs
    """
    __pytype__ = NurbsPatch
    def __init__ (self,nurbs= None, **kargs) :
        IInterface.__init__(self, **kargs)
        self.nurbs=nurbs 
        
    @classmethod
    def default(cls):
        return NurbsPatch([[(j-1.5,i-1.5,0,1) for j in range(4)] for i in range(4)])


    def __repr__(self):
        return 'INurbsPatch(nurbs=%s)' % self.nurbs





