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

from interface.abstract_geometry import AbstractShape
volume_coeff=[1.,2.,2./3.]

class Sphere (AbstractShape) :
	"""
	a sphere centered on a point
	"""
	def __init__ (self, center_id, radius) :
		"""
		construct a sphere from a center point
		and a radius
		"""
		self._center=center_id
		self._radius=radius
	
	def centroid (self, positions) :
		return positions[self._center]
	centroid.__doc__=AbstractShape.centroid.__doc__
	
	def barycenter (self, positions) :
		return positions[self._center]
	barycenter.__doc__=AbstractShape.barycenter.__doc__
	
	def volume (self, positions) :
		pos=positions[self._center]
		V=1.
		for ind,coord in enumerate(pos) :
			V*=volume_coeff[ind]*self._radius
		return V
	volume.__doc__=AbstractShape.volume.__doc__
	
	def size (self, positions) :
		return self._radius
	size.__doc__=AbstractShape.size.__doc__
	
	def center (self) :
		return self._center
	
	def radius (self) :
		return self._radius

