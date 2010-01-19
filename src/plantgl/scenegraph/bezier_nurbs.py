# -*- python -*-
#
#       plantgl: manipulate bezier curves
#
#       Copyright 2006 INRIA - CIRAD - INRA  
#
#       File author(s): Jerome Chopard <jerome.chopard@sophia.inria.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#

"""
This module defines a function to construct
a Nurbs from a set of ctrl points representing
a cubic bezier path as defined in:
http://www.w3.org/TR/SVG/paths.html#PathDataCurveCommands
"""

__license__= "Cecill-C"
__revision__=" $Id: $ "

from openalea.plantgl.math import Vector2,Vector3,norm
from openalea.plantgl.scenegraph import NurbsCurve2D

def cubic_bezier (ctrl_points, uniform = False, stride = 60) :
	"""Construct a nurbs from a set of ctrl_points
	
	ctrl_points are control points in space that
	define a cubic bezier nurbs as proposed in svg norm
	http://www.w3.org/TR/SVG/paths.html#PathDataCurveCommands
	
	An arc `i` of the resulting curve will interpolate
	points 4 * i, 4 * i +1, 4 * i + 2, 4 * i + 3.
	
	:Parameters:
	 - `ctrl_points` (list of Vector) - a list
	    of control points
	 - `uniform` (bool) - if True, the parameterization
	    of the curve will be chosen such that each
	    segment will be of length 1 in u space, wathever 
	    its real geometrical size.
	 - `stride` (int) - number of points to discretize
	    the curve
	
	:Returns Type: :class:NurbsCurve2D
	"""
	#TODO work in 3D too
	degree = 3
	ctrl_points = [Vector2(*vec) for vec in ctrl_points]
	
	nb_pts = len(ctrl_points)
	nb_arc = (nb_pts - 1) / degree
	nb_knots = degree + nb_pts
	p = 0.
	param = [p]
	for i in xrange(nb_arc) :
		if uniform :
			p += 1
		else :
			p += norm(ctrl_points[degree * i] \
			        - ctrl_points[degree * (i + 1)])
		param.append(p)
	kv = [param[0]]
	for p in param :
		for j in xrange(degree) :
			kv.append(p)
	kv.append(param[-1])
	
	#curve
	return NurbsCurve2D([Vector3(v[0],v[1],1.) for v in ctrl_points],
	                    kv,
	                    degree,
	                    stride)



