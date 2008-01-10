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

class AbstractShape (object) :
	"""
	a geometrical object that work on abstract points
	and not on positions
	"""
	def centroid (self, positions) :
		"""
		return the centroid of the object according
		to positions of points stored in positions
		:param positions: dict of {pid:vector}
		"""
		raise NotImplementedError
	
	def barycenter (self, positions) :
		"""
		return the barycenter of the object according
		to positions of points stored in positions
		:param positions: dict of {pid:vector}
		"""
		raise NotImplementedError
	
	def volume (self, positions) :
		"""
		return the volume (or surface in 2D or length in 1D or 0 in 0D)
		of the object according
		to positions of points stored in positions
		:param positions: dict of {pid:vector}
		"""
		raise NotImplementedError
	
	def size (self, positions) :
		"""
		return the size (1D length)
		of the object according
		to positions of points stored in positions
		:param positions: dict of {pid:vector}
		rtype : float
		"""
		raise NotImplementedError

class AbstractPolyhedra (AbstractShape) :
	"""
	a specification of previous concept for shapes
	that are defined by corners
	"""
	dim=0
	"""
	topological dimension of the polyhedra
	"""
	def corners (self) :
		"""
		iterator on all corners of the shape
		"""
		raise NotImplementedError
	
	def __iter__ (self) :
		"""
		alias for 'corners'
		"""
		raise NotImplementedError
	
	def nb_corners (self) :
		"""
		number of corners that define the polyhedral shape
		"""
		raise NotImplementedError
