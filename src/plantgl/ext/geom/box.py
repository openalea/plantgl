# -*- python -*-
# -*- coding: latin-1 -*-
#
#       Geometry : celltissue package
#
#       Copyright or © or Copr. 2006 INRIA - CIRAD - INRA  
#
#       File author(s): Jerome Chopard <jerome.chopard@sophia.inria.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       VPlants WebSite : https://gforge.inria.fr/projects/vplants/
#

__doc__="""
This module provide a set of geometrical concepts to express tissue geometry
"""

__license__= "Cecill-C"
__revision__=" $Id: graph.py 116 2007-02-07 17:44:59Z tyvokka $ "

from interface.abstract_geometry import AbstractShape
from tools import Vmean

class Box (AbstractShape) :
	"""
	a box aligned with axes
	"""
	def __init__ (self, lower_left_id, top_rigth_id) :
		"""
		construct a box specified by two extermities
		of main diagonal
		"""
		self._extremities=(lower_left_id,top_rigth_id)
	
	def centroid (self, positions) :
		return Vmean([positions[pid] for pid in self._extremities])
	centroid.__doc__=AbstractShape.centroid.__doc__
	
	def barycenter (self, positions) :
		return Vmean([positions[pid] for pid in self._extremities])
	barycenter.__doc__=AbstractShape.barycenter.__doc__
	
	def volume (self, positions) :
		pos1,pos2=[positions[pid] for pid in self._extremities]
		diag=pos2-pos1
		V=1.
		for coord in diag :
			V*=coord
		return V
	volume.__doc__=AbstractShape.volume.__doc__

