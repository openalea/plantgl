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
from Blender import Scene,Mesh,Material,Object
from vplants.plantgl.algo import Discretizer

def material_to_blender (pglmat) :
	"""Create a blender material
	
	:Parameters:
	 - `pglmat` (pgl.Material)
	
	:Returns Type: bld.Material
	"""
	#creation
	if pglmat.isNamed() :
		name = pglmat.name
	else :
		name = "pglmat%d" % pglmat.getId()
	
	mat = Material.New(name)
	
	#ambient
	col = pglmat.ambient
	mat.setRGBCol(col.red / 255.,
	              col.green / 255.,
	              col.blue / 255.)
	
	#specular
	col = pglmat.specular
	mat.setSpecCol(col.red / 255.,
	               col.green / 255.,
	               col.blue / 255.)
	
	#TODO other material properties
	
	#return
	return mat

def geometry_to_blender (geom, bld_mesh = None, discretizer = None) :
	"""Create a blender mesh
	
	Paint the faces too if needed
	
	:Parameters:
	 - `geom` (pgl.Geometry) - the geometry to transform
	 - `bld_mesh` (Mesh) - a mesh in which to append the shape.
	                       If None, a blank new one will be created
	 - `discretizer` (Discretizer) - algorithm to triangulate the geometry
	
	:Returns Type: Mesh
	"""
	#create bld_mesh
	if bld_mesh is None :
		if geom.isNamed() :
			name = geom.name
		else :
			name = "pglgeom%d" % geom.getId()
		
		bld_mesh = Mesh.New(name)
	
	#geometry (mesh)
	if discretizer is None:
		d = Discretizer()
	else:
		d = discretizer 
	
	geom.apply(d)
	geom = d.result
	
	#fill mesh
	pts = array(geom.pointList)
	nbv = len(bld_mesh.verts)
	faces = nbv + array(geom.indexList)
	
	bld_mesh.verts.extend(pts)
	nbf = len(bld_mesh.faces)
	bld_mesh.faces.extend(faces.tolist() )
	
	#set vertex colors if needed
	mat = None
	if not geom.isColorListToDefault() :
		bld_mesh.vertexColors = True
		
		#create material to render mesh colors
		try :
			mat = Material.Get("default_vtx_mat")
		except NameError :
			mat = Material.New("default_vtx_mat")
			mat.mode += Material.Modes['VCOL_PAINT']
		
		bld_mesh.materials = [mat]
		
		#modify color list to duplicate colors per face per vertex
		if geom.colorPerVertex and geom.isColorIndexListToDefault() :
			#each vertex has a color
			for i,inds in enumerate(faces) :
				face = bld_mesh.faces[nbf + i]
				for j,v in enumerate(face) :
					col = face.col[j]
					pglcol = geom.colorList[geom.indexList[i][j] ]
					col.r = pglcol.red
					col.g = pglcol.green
					col.b = pglcol.blue
					col.a = pglcol.alpha
		elif geom.colorPerVertex and not geom.isColorIndexListToDefault() :
			#each local vertex of each face has a color
			for i,inds in enumerate(geom.colorIndexList) :
				face = bld_mesh.faces[nbf + i]
				for j,v in enumerate(face) :
					col = face.col[j]
					pglcol = geom.colorList[inds[j] ]
					col.r = pglcol.red
					col.g = pglcol.green
					col.b = pglcol.blue
					col.a = pglcol.alpha
		else :
			#each face has a color
			for i,col in enumerate(geom.colorList) :
				face = bld_mesh.faces[nbf + i]
				R,G,B,A = col.red,col.green,col.blue,col.alpha
				for j,v in enumerate(face) :
					col = face.col[j]
					col.r = R
					col.g = G
					col.b = B
					col.a = A
		
#		#assign 
#		for fid in xrange(nb,len(bld_mesh.faces) ) :
#			face = bld_mesh.faces[fid]
#			for i,v in enumerate(face) :
#				col = face.col[i]
#				col.r = ambient.red
#				col.g = ambient.green
#				col.b = ambient.blue
	
#	for vtx in bld_mesh.verts :
#		vtx.sel = 0
	
	#return
	return bld_mesh,mat

def shp_to_blender (shp, discretizer = None, mats = {}) :
	"""Create a blender object for this shape
	
	:Parameters:
	 - `shp` (Shape) - the shape to transform
	 - `discretizer` (Discretizer) - algorithm to triangulate the geometry
	 - `mats` (dict of (mat_id,Material) ) - precomputed pgl Material
	               to avoid to recreate many instances of the same material
	
	:Returns Type: Object
	"""
	#name
	if shp.isNamed() :
		name = shp.getName()
	else :
		name = "pglshp%d" % shp.getId()
	
	#create material
	app = shp.appearance
	try :
		mat = mats[app.getId()]
	except KeyError :
		mat = material_to_blender(app)
		mats[app.getId()] = mat
	
	#create mesh
	mesh,mmat = geometry_to_blender(shp.geometry)
	
	#create object
	ob = Object.New('Mesh',name)
	ob.link(mesh)
	ob.setMaterials([mat])
	if mmat is None :
		ob.colbits = 1 << 0
	
	
	#return
	return ob

def sc_to_blender (sc) :
	"""Export each shape in the scene as a new object
	
	:Parameters:
	 - `sc` (Scene) - the scene to transform
	
	:Returns Type: None
	"""
	bldsc = Scene.GetCurrent()
	
	#fill with shapes
	t = time.time()
	d = Discretizer()
	for ind,shp in enumerate(sc) :
		if ind % 10 == 0 :
			print ind,
			sys.stdout.flush()
		ob = shp_to_blender(shp,d)
		bldsc.link(ob)
		ob.sel = True
	
	print
	print 'Time :', time.time() - t
	
	#return
	print "created"
	bldsc.update(1)
	Blender.Redraw()
	
	return ob

#def sc_to_blender (sc) :
#	"""Create a blender mesh with faces painted with the right color
#	
#	:Parameters:
#	 - `sc` (Scene) - the scene to transform
#	
#	:Returns Type: Object
#	"""
#	#create mesh
#	m = Mesh.New("pglscmesh")
#	m.vertexColors = True
#	
#	#fill with shapes
#	t = time.time()	
#	d = Discretizer()
#	for ind,shp in enumerate(sc) :
#		if ind % 10 == 0 :
#			print ind,
#			sys.stdout.flush()
#		shp_to_blender(shp,m,d)
#	print
#	print 'Time :', time.time() - t
#	
#	#create object
#	bldsc = Scene.GetCurrent()
#	ob = bldsc.objects.new(m,"pglscobj")
#	
#	mat = Material.New("pglmat")
#	mat.setRGBCol(1.,1.,1.)
#	mat.setSpecCol(0.5,0.5,0.5)
#	mat.mode = Material.Modes['VCOL_PAINT'] \
#	         + Material.Modes['TRACEABLE'] \
#	         + Material.Modes['SHADOW'] \
#	         + Material.Modes['RAYBIAS']
#	ob.setMaterials([mat])
#	ob.colbits = 1
#	ob.sel = True
#	
#	#return
#	print "created"
#	bldsc.update(1)
#	Blender.Redraw()
#	
#	return ob

