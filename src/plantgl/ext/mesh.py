#set of functions related to meshes
#in 3D (point,edge,face,cell)
from openalea.plantgl.math import Vector3
from openalea.plantgl.scenegraph import TriangleSet

def cell_geometry (cid, mesh, position) :
	"""
	compute the triangle set representing the geometry
	of this cell
	"""
	trans={}
	pos_list=[]
	face_list=[]
	for pid in mesh.borders(3,cid,3) :
		trans[pid]=len(pos_list)
		pos_list.append(position[pid])
	for fid in mesh.borders(3,cid) :
		pts=set()
		center_ind=len(pos_list)
		for eid in mesh.borders(2,fid) :
			pid1,pid2=mesh.borders(1,eid)
			face_list.append( (center_ind,trans[pid1],trans[pid2]) )
			pts.add(pid1)
			pts.add(pid2)
		pos_list.append(sum( (position[pid] for pid in pts) ,Vector3())/len(pts))
	return TriangleSet(pos_list,face_list)

