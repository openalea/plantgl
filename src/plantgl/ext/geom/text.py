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

class Text (AbstractShape) :
	"""
	a text centered around a point in space
	"""
	def __init__ (self, message, pid, size=8) :
		"""
		construct a text specified by message
		centered on pid with size equal to size
		"""
		self._message=message
		self._pid=pid
		self._size=size
	
	def centroid (self, positions) :
		return positions[self._pid]
	centroid.__doc__=AbstractShape.centroid.__doc__
	
	def barycenter (self, positions) :
		return positions[self._pid]
	barycenter.__doc__=AbstractShape.barycenter.__doc__
	
	def volume (self, positions) :
		"""
		real length of the text
		"""
		return len(self._message)*self._size
	volume.__doc__+=AbstractShape.volume.__doc__
	
	def position (self) :
		return self._pid
	
	def message (self) :
		return self._message
	
	def size (self) :
		return self._size
