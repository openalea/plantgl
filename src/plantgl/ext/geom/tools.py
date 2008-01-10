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
This module provide a set of tools to simplify geometrical functions
"""

__license__= "Cecill-C"
__revision__=" $Id: graph.py 116 2007-02-07 17:44:59Z tyvokka $ "

from operator import add

def Vsum (vectors) :
	"""
	return the sum of all elements in vectors
	"""
	return reduce(add,vectors)

def Vmean (vectors) :
	"""
	return mean of vectors
	"""
	return Vsum(vectors)/len(vectors)

def barycenter (point_weights, positions) :
	return Vsum([positions[pid]*weight for pid,weight in point_weights.iteritems()])/sum(point_weights.values())

def shape_expand (shape, positions, expand_factor) :
	"""
	modify positions to expand them around
	their centroid
	:type vectors: dict of pid:vector
	"""
	cent=Vmean([positions[pid] for pid in shape])
	new_pos={}
	#for pid,pos in positions.iteritems() :
	for pid in shape :
		new_pos[pid]=cent+(positions[pid]-cent)*expand_factor
	return new_pos

__all__=["Vsum","Vmean","barycenter","shape_expand"]
