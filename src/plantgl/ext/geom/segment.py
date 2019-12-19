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
__revision__=" $Id$ "

from .interface.abstract_geometry import AbstractPolyhedra
from .tools import Vmean

class Segment (AbstractPolyhedra) :
	"""
	a segment 1D in space
	"""
	dim=1
	
	def __init__ (self, extremities) :
		"""
		construct a segment by giving its
		two extremities [pid1,pid2]
		"""
		self._extremities=tuple(extremities)
	
	def centroid (self, positions) :
		return Vmean([positions[pid] for pid in self.corners()])
	centroid.__doc__=AbstractPolyhedra.centroid.__doc__
	
	def barycenter (self, positions) :
		return Vmean([positions[pid] for pid in self.corners()])
	barycenter.__doc__=AbstractPolyhedra.barycenter.__doc__
	
	def volume (self, positions) :
		point1,point2=self._extremities
		pid1,=point1
		pid2,=point2
		return (positions[pid2]-positions[pid1]).__norm__()
	volume.__doc__=AbstractPolyhedra.volume.__doc__
	
	def size (self, positions) :
		return self.volume(positions)
	size.__doc__=AbstractPolyhedra.size.__doc__
	
	def corners (self) :
		for point in self._extremities :
			for pid in point :
				yield pid
	corners.__doc__=AbstractPolyhedra.corners.__doc__
	
	def __iter__ (self) :
		return self.corners()
	__iter__.__doc__=AbstractPolyhedra.__iter__.__doc__
	
	def nb_corners (self) :
		return 2
	nb_corners.__doc__=AbstractPolyhedra.nb_corners.__doc__

