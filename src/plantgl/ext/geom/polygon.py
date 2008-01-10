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

from interface.abstract_geometry import AbstractPolyhedra
from segment import Segment
from tools import Vmean,barycenter

class Polygon (AbstractPolyhedra) :
	"""
	a convex polygon (flat) in space
	"""
	dim=2
	
	def __init__ (self, edges) :
		"""
		edges is a list of segments that represents
		the edges of the polygon
		"""
		self._edges=list(edges)
		self._corners=set()
		for edge in edges :
			self._corners|=set(edge)
	
	def centroid (self, positions) :
		return Vmean([positions[pid] for pid in self._corners])
	centroid.__doc__=AbstractPolyhedra.centroid.__doc__
	
	def barycenter (self, positions) :
		point_weights=dict( (pid,0) for pid in self._corners )
		for edge in self._edges :
			weight=edge.volume(positions)
			for pid in edge :
				point_weights[pid]+=weight
		return barycenter(point_weights,positions)
	barycenter.__doc__=AbstractPolyhedra.barycenter.__doc__
	
	def volume (self, positions) :
		surf=0.
		mid=self.centroid(positions)
		if mid.dim()==2 :
			for edge in self._edges :
				pos1,pos2=[positions[pid] for pid in edge]
				surf+=abs((pos2-mid).cross(pos1-mid))
		else :
			for edge in self._edges :
				pos1,pos2=[positions[pid] for pid in edge]
				surf+=(pos2-mid).cross(pos1-mid).norm()
		return surf/2.
	volume.__doc__=AbstractPolyhedra.volume.__doc__
	
	def size (self, positions) :
		return (self.volume(positions))**(1./2.)
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
		return iter(self._edges)
	
	def nb_edges (self) :
		return len(self._edges)
