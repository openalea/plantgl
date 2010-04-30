# -*- python -*-
#
#       plantgl/blender: function to convert between plantgl and blender
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
Defines functions to transform object from plantgl into blender
and vice versa.
"""

__license__ = "Cecill-C"
__revision__ = " $Id: $ "

import sys, time
from numpy import array
import Blender
from Blender import Scene,Mesh,Material
from vplants.plantgl.algo import Discretizer

def shp_to_blender (shp, bld_mesh = None, discretizer = None) :
	"""Create a blender mesh with faces painted with the right color
	
	:Parameters:
	 - `shp` (Shape) - the shape to transform
	 - `bld_mesh` (Mesh) - a mesh in which to append the shape.
	                       If None, a blank new one will be created
	
	:Returns Type: Mesh
	"""
	#create bld_mesh
	if bld_mesh is None :
		bld_mesh = Mesh.New("pglmesh%d" % shp.id)
		bld_mesh.vertexColors = True
	
	#geometry (mesh)
	if discretizer is None:
		d = Discretizer()
	else:
		d = discretizer 
	shp.geometry.apply(d)
	geom = d.result
	
	#color
	ambient = shp.appearance.ambient
	
	#append mesh
	pts = array(geom.pointList)
	nb = len(bld_mesh.verts)
	faces = nb + array(geom.indexList)
	
	bld_mesh.verts.extend(pts)
	nb = len(bld_mesh.faces)
	bld_mesh.faces.extend(faces.tolist())
	
	#set colors
	for fid in xrange(nb,len(bld_mesh.faces) ) :
		face = bld_mesh.faces[fid]
		for i,v in enumerate(face) :
			col = face.col[i]
			col.r = ambient.red
			col.g = ambient.green
			col.b = ambient.blue
	
	for vtx in bld_mesh.verts :
		vtx.sel = 0
	
	#return
	return bld_mesh

def sc_to_blender (sc) :
	"""Create a blender mesh with faces painted with the right color
	
	:Parameters:
	 - `sc` (Scene) - the scene to transform
	
	:Returns Type: Object
	"""
	#create mesh
	m = Mesh.New("pglscmesh")
	m.vertexColors = True
	
	#fill with shapes
	t = time.time()	
	d = Discretizer()
	for ind,shp in enumerate(sc) :
		if ind % 10 == 0 :
			print ind,
			sys.stdout.flush()
		shp_to_blender(shp,m,d)
	print
	print 'Time :', time.time() - t
	
	#create object
	bldsc = Scene.GetCurrent()
	ob = bldsc.objects.new(m,"pglscobj")
	
	mat = Material.New("pglmat")
	mat.setRGBCol(1.,1.,1.)
	mat.setSpecCol(0.5,0.5,0.5)
	mat.mode = Material.Modes['VCOL_PAINT'] \
	         + Material.Modes['TRACEABLE'] \
	         + Material.Modes['SHADOW'] \
	         + Material.Modes['RAYBIAS']
	ob.setMaterials([mat])
	ob.colbits = 1
	ob.sel = True
	
	#return
	print "created"
	bldsc.update(1)
	Blender.Redraw()
	
	return ob

