# -*- python -*-
#
#       OpenAlea.OALab: Multi-Paradigm GUI
#
#       Copyright 2014 INRIA
#
#       File author(s): Guillaume Baty <guillaume.baty@inria.fr>
#
#       File contributor(s):
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
#
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
###############################################################################

from openalea.core.interface import IInterface


class IColorList(IInterface):
    """
    List of tuple name, rgb(a) color.
    Example:

    [
        ('C1', (255,0,0,255)),
        ('C2', (0,255,0,255))
    ]

    """
    __alias__ = 'Color List'

    def __init__(self):
        self.value = self.sample()

    def sample(self):
        """
        Reinitialize control to default value
        """
        from openalea.plantgl.all import Color3
        value = [
            ("Color_0", (80, 80, 80, 255), 2.), # Grey
            ("Color_1", (65, 45, 15), 2.), # Brown
            ("Color_2", (30, 60, 10), 2.), # Green
            ("Color_3", (60, 0, 0), 2.), # Red
            ("Color_4", (60, 60, 15), 2.), # Yellow
            ("Color_5", (0, 0, 60), 2.), # Blue
            ("Color_6", (60, 0, 60), 2.), # Purple
            ]
        return value

    def _repr_py__(self):
        return 

class IMaterialList(IInterface):
    """
    List of tuple name, rgb(a) color.
    Example:

    [
        ('C1', Material((255,0,0,255)),
        ('C2', Material((0,255,0,255))
    ]

    """
    __alias__ = 'Material List'

    def __init__(self):
        self.value = self.sample()

    def sample(self):
        """
        Reinitialize control to default value
        """
        from openalea.plantgl.all import Material, PglTurtle

        value = PglTurtle().getColorList()
        return value

    def module_dependence(self):
        return 'openalea.plantgl.all'

class ICurve2D(IInterface):
    """
    NurbsCurve2D(Point3Array([Vector3(-0.5,0,1),Vector3(-0.166667,0,1),Vector3(0.166667,0,1),Vector3(0.5,0,1)]), width = 2)
    """
    def __init__(self):
        self.value = self.sample()

    __alias__ = 'Curve 2D'

    def sample(self):
        """
        Reinitialize control to default value
        """
        from openalea.plantgl.all import NurbsCurve2D, Point3Array, Vector3
        curve = NurbsCurve2D(
            Point3Array([
                Vector3(-0.5, 0, 1),
                Vector3(-0.166667, 0, 1),
                Vector3(0.166667, 0, 1),
                Vector3(0.5, 0, 1)
            ]),
            width=2)
        return curve

    def module_dependence(self):
        return 'openalea.plantgl.all'


class IQuantisedFunction(IInterface):
    """
   
    """
    __alias__ =  'QuantisedFunction'
    def __init__(self):
        self.value = self.default()


    def default(self):
        """
        Reinitialize control to default value
        """
        from openalea.plantgl.all import EditableQuantisedFunction, NurbsCurve2D
        nbP = 4
        value = EditableQuantisedFunction(NurbsCurve2D([(float(i)/(nbP-1),0,1) for i in xrange(nbP)]) )
        return value

    def module_dependence(self):
        return 'openalea.plantgl.all'



class IPatch(IInterface):
    """
   
    """
    __alias__ = 'Patch'
    def __init__(self):
        self.value = self.default()


    def default(self):
        """
        Reinitialize control to default value
        """
        from openalea.plantgl.all import NurbsPatch 
        return NurbsPatch([[(j-1.5,i-1.5,0,1) for j in range(4)] for i in range(4)])

    def module_dependence(self):
        return 'openalea.plantgl.all'





class PlantGLOAInterfacePlugin(object):

    def __call__(self):

        all = [IColorList, IMaterialList, ICurve2D, IQuantisedFunction, IPatch]
        return all
