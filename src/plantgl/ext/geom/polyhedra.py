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
from .polygon import Polygon
from .segment import Segment
from .point import Point
from .tools import Vmean,barycenter

class Polyhedra (AbstractPolyhedra) :
	"""
	a convex polyhedra in space
	"""
	dim=3
	
	def __init__ (self, faces) :
		"""
		construct a polyhedra from a list of faces
		each face is an ordered set of pid or a polygon
		"""
		self._faces=list(faces)
		self._corners=set()
		for face in faces :
			self._corners|=set(face)
	
	def centroid (self, positions) :
		return Vmean([positions[pid] for pid in self._corners])
	centroid.__doc__=AbstractPolyhedra.centroid.__doc__
	
	def barycenter (self, positions) :
		point_weights=dict( (pid,0) for pid in self._corners )
		for edge in self.edges() :
			weight=edge.volume(positions)
			for pid in edge :
				point_weights[pid]+=weight
		return barycenter(point_weights,positions)
	barycenter.__doc__=AbstractPolyhedra.barycenter.__doc__
	
	def volume (self, positions) :
		mid=self.centroid(positions)
		#face walk
		vol=0.
		for face in self._faces :
			face_dir=face.centroid(positions)-mid
			#edge walk
			for seg in face.edges() :
				pos1,pos2=[positions[pid] for pid in seg]
				vol+=abs(((pos1-mid).cross(pos2-mid)).dot(face_dir))
		return vol/6.
	volume.__doc__=AbstractPolyhedra.volume.__doc__
	
	def size (self, positions) :
		return (self.volume(positions))**(1./3.)
	size.__doc__=AbstractPolyhedra.size.__doc__
	
	def corners (self) :
		return iter(self._corners)
	corners.__doc__=AbstractPolyhedra.corners.__doc__
	
	def __iter__ (self) :
		return iter(self._corners)
	__iter__.__doc__=AbstractPolyhedra.__iter__.__doc__
	
	def nb_corners (self) :
		return len(self._corners)
	nb_corners.__doc__=AbstractPolyhedra.nb_corners.__doc__
	
	def edges (self) :
		edge_list=set()
		for face in self._faces :
			edge_list|=set([tuple(seg) for seg in face.edges()])
		for edge in edge_list :
			yield Segment([Point(pid) for pid in edge])
	
	def faces (self) :
		return iter(self._faces)
