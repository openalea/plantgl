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


class IPatch(IInterface):
    """
   
    """
    def __init__(self):
        self.value = self.default()

    def __repr__(self):
        return 'IPatch'

    def default(self):
        """
        Reinitialize control to default value
        """
        from openalea.plantgl.all import NurbsPatch 
        return NurbsPatch([[(j-1.5,i-1.5,0,1) for j in range(4)] for i in range(4)])





class PlantGLOAInterfacePlugin(object):

    def __call__(self):

        all = [IPatch]
        return all
