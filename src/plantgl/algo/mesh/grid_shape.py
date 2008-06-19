"""
construct a mesh as a regular grid
"""
from openalea.container import Grid,Topomesh
from openalea.plantgl.math import Vector3

def grid2D (shape) :
	"""
	:param `shape`: shape of the grid tuple (nb_rows,nb_columns)
	"""
	t=Topomesh(2)
	positions={}
	NBI,NBJ=shape
	NBCELLS=NBI*NBJ
	xincr=1./NBI
	yincr=1./NBJ
	cell_grid=Grid( (NBI,NBJ) )
	for ind in cell_grid :
		cid=t.add_wisp(0,ind)
	point_grid=Grid( (NBI+1,NBJ+1) )
	for ind in point_grid :
		i,j=point_grid.coordinates(ind)
		eid=t.add_wisp(2,ind)
		positions[eid]=Vector3(xincr*i,yincr*j,0)
	#murs verticaux
	for j in xrange(NBJ) :
		for i in xrange(NBI+1) :
			wid=t.add_wisp(1)
			t.link(1,wid,point_grid.index( (i,j) ))
			t.link(1,wid,point_grid.index( (i,j+1) ))
			if i<NBI :
				t.link(0,cell_grid.index( (i,j) ),wid)
			if i>0 :
				t.link(0,cell_grid.index( (i-1,j) ),wid)
	#murs horizontaux
	for i in xrange(NBI) :
		for j in xrange(NBJ+1) :
			wid=t.add_wisp(1)
			t.link(1,wid,point_grid.index( (i,j) ))
			t.link(1,wid,point_grid.index( (i+1,j) ))
			if j<NBJ :
				t.link(0,cell_grid.index( (i,j) ),wid)
			if j>0 :
				t.link(0,cell_grid.index( (i,j-1) ),wid)
	#et voila
	return t,positions

def grid3D (shape) :
	"""
	:param `shape`: shape of the grid tuple (nb_rows,nb_columns,nb_slices)
	"""
	from celltissue import PolyhedralTissue
	from celltissue.geometry import Point,Segment
	t=PolyhedralTissue(3)
	positions=Mesh2D()
	NBI,NBJ,NBK=shape
	NBCELLS=NBI*NBJ*NBK
	xincr=1./NBI
	yincr=1./NBJ
	zincr=1./NBK
	cell_grid=Grid( (NBI,NBJ,NBK) )
	for ind in xrange(len(cell_grid)) :
		cid=t.add_wisp(0,ind)
	point_grid=Grid( (NBI+1,NBJ+1,NBK+1) )
	for ind in point_grid :
		i,j,k=point_grid.coordinates(ind)
		positions.add_point(xyz(xincr*i,yincr*j,zincr*k),ind)
	#edges
	JKedges=Grid( (NBI,NBJ+1,NBK+1) )
	JK={}
	for ind in JKedges :
		i,j,k=JKedges.coordinates(ind)
		eid=t.add_wisp(2)
		t.set_geometry(2,eid,Segment([Point(point_grid.index( (i,j,k) )),\
								Point(point_grid.index( (i+1,j,k) ))] ))
		JK[(i,j,k)]=eid
	IKedges=Grid( (NBI+1,NBJ,NBK+1) )
	IK={}
	for ind in IKedges :
		i,j,k=IKedges.coordinates(ind)
		eid=t.add_wisp(2)
		t.set_geometry(2,eid,Segment([Point(point_grid.index( (i,j,k) )),\
								Point(point_grid.index( (i,j+1,k) ))] ))
		IK[(i,j,k)]=eid
	IJedges=Grid( (NBI+1,NBJ+1,NBK) )
	IJ={}
	for ind in IJedges :
		i,j,k=IJedges.coordinates(ind)
		eid=t.add_wisp(2)
		t.set_geometry(2,eid,Segment([Point(point_grid.index( (i,j,k) )),\
								Point(point_grid.index( (i,j,k+1) ))] ))
		IJ[(i,j,k)]=eid
	#murs
	for k in xrange(NBK+1) :
		for j in xrange(NBJ) :
			for i in xrange(NBI) :
				wid=t.add_wisp(1)
				t.link(1,wid,IK[(i,j,k)])
				t.link(1,wid,IK[(i+1,j,k)])
				t.link(1,wid,JK[(i,j,k)])
				t.link(1,wid,JK[(i,j+1,k)])
				if k<NBK :
					t.link(0,cell_grid.index( (i,j,k) ),wid)
				if k>0 :
					t.link(0,cell_grid.index( (i,j,k-1) ),wid)
	for k in xrange(NBK) :
		for j in xrange(NBJ+1) :
			for i in xrange(NBI) :
				wid=t.add_wisp(1)
				t.link(1,wid,IJ[(i,j,k)])
				t.link(1,wid,IJ[(i+1,j,k)])
				t.link(1,wid,JK[(i,j,k)])
				t.link(1,wid,JK[(i,j,k+1)])
				if j<NBJ :
					t.link(0,cell_grid.index( (i,j,k) ),wid)
				if j>0 :
					t.link(0,cell_grid.index( (i,j-1,k) ),wid)
	for k in xrange(NBK) :
		for j in xrange(NBJ) :
			for i in xrange(NBI+1) :
				wid=t.add_wisp(1)
				t.link(1,wid,IK[(i,j,k)])
				t.link(1,wid,IK[(i,j,k+1)])
				t.link(1,wid,IJ[(i,j,k)])
				t.link(1,wid,IJ[(i,j+1,k)])
				if i<NBI :
					t.link(0,cell_grid.index( (i,j,k) ),wid)
				if i>0 :
					t.link(0,cell_grid.index( (i-1,j,k) ),wid)
	return t,positions

