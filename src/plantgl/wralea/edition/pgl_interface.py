# -*- python -*-
#
#
#       Copyright 2006-2009 INRIA - CIRAD - INRA  
#
#       File author(s): Pradal, Boudon, Barbeau 
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
"""declaration of pix interface and its widget """

__license__ = "Cecill-C"
__revision__ = " $Id: interface.py 2245 2010-02-08 17:11:34Z cokelaer $"

from openalea.core.interface import IInterface, IInterfaceMetaClass
from openalea.plantgl.scenegraph import Curve2D, NurbsCurve2D

class ICurve2D(IInterface) :
    """
    interface for different curves 2D
    """
    __metaclass__ = IInterfaceMetaClass
    __pytype__ = Curve2D
    def __init__ (self,**kargs) :
        IInterface.__init__(self, **kargs)
        
    @classmethod
    def default(cls):
        return NurbsCurve2D([(0.,0.,1.),(1/3.,0.,1.),(2/3.,0.,1.),(1.,0.,1.)])







