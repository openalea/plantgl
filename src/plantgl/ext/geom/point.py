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

class Point (AbstractPolyhedra) :
	"""
	a point as 0D object in space
	hopefully not use
	"""
	dim=0
	
	def __init__ (self, pid) :
		self._pid=pid
	
	def centroid (self, positions) :
		return positions[self._pid]
	centroid.__doc__=AbstractPolyhedra.centroid.__doc__
	
	def barycenter (self, positions) :
		return positions[self._pid]
	barycenter.__doc__=AbstractPolyhedra.barycenter.__doc__
	
	def volume (self, positions) :
		return 0.
	volume.__doc__=AbstractPolyhedra.volume.__doc__
	
	def size (self, positions) :
		return 0.
	size.__doc__=AbstractPolyhedra.size.__doc__
	
	def corners (self) :
		yield self._pid
	corners.__doc__=AbstractPolyhedra.corners.__doc__
	
	def __iter__ (self) :
		yield self._pid
	__iter__.__doc__=AbstractPolyhedra.__iter__.__doc__
	
	def nb_corners (self) :
		return 1
	nb_corners.__doc__=AbstractPolyhedra.nb_corners.__doc__

