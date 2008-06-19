from openalea.plantgl.math import norm

def opposite_point (mesh, eid, pid) :
	"""
	return opposite point of edge
	"""
	for cornid in mesh.borders(1,eid) :
		if cornid!=pid :
			return cornid

def opposite_edge (mesh, fid, eid, pid) :
	"""
	return opposite edge of a point in a face
	"""
	edges=set(mesh.regions(0,pid))&set(mesh.borders(2,fid))
	edges.remove(eid)
	return edges.pop()

def refine_triangular_mesh (mesh, position, fids) :
	"""
	refine a face of the mesh by subdividing the
	longest edge of the face using Rivara algorithm
	divide linked faces if necessary to maintain
	conformity of the mesh
	"""
	already_divided_edges={}
	to_divide_face=set(fids)
	divided_faces=[]
	divided_edges=[]
	while len(to_divide_face)>0 :
		fid=to_divide_face.pop()
		edges=list(mesh.borders(2,fid))
		#calcul de la longueur de chacune de aretes
		edge_length={}
		for eid in edges :
			pid1,pid2=mesh.borders(1,eid)
			edge_length[eid]=norm(position[pid1]-position[pid2])
		real_length=[]
		for eid in edges :
			if eid in already_divided_edges and already_divided_edges[eid] in edges :
				real_length.append( (edge_length[eid]+edge_length[already_divided_edges[eid]],eid) )
			else :
				real_length.append( (edge_length[eid],eid) )
		real_length.sort()
		div_eid=real_length[-1][1]
		if div_eid not in already_divided_edges :
			#subdivision de l'edge
			pid1,pid2=mesh.borders(1,div_eid)
			pid=mesh.add_wisp(0)
			position[pid]=(position[pid1]+position[pid2])/2.
			eid1=mesh.add_wisp(1)
			mesh.link(1,eid1,pid1)
			mesh.link(1,eid1,pid)
			eid2=mesh.add_wisp(1)
			mesh.link(1,eid2,pid2)
			mesh.link(1,eid2,pid)
			#raccord avec les faces
			faces=set(mesh.regions(1,div_eid))
			for bid in faces :
				mesh.link(2,bid,eid1)
				mesh.link(2,bid,eid2)
			#ajout a la liste des faces a traiter
			faces.remove(fid)
			to_divide_face|=faces
			#retrait de l'ancienne edge
			mesh.remove_wisp(1,div_eid)
			divided_edges.append( (div_eid,(eid1,eid2)) )
			#mise a jour de div_eid
			div_eid=eid1
			already_divided_edges[eid1]=eid2
			already_divided_edges[eid2]=eid1
		#division de la face en deux
		#recherche du sommet oppose a div_eid
		#recherche des trois sommets du triangle
		face_corners=set(mesh.borders(2,fid,2))
		edges=list(mesh.borders(2,fid))
		for eid in edges :
			if eid in already_divided_edges :
				eid2=already_divided_edges[eid]
				if eid2 in edges :
					edges.remove(eid2)
					#recherche point commun aux deux edges
					common_pid=set(mesh.borders(1,eid))&set(mesh.borders(1,eid2))
					face_corners-=common_pid
		assert len(face_corners)==3
		#retrait des deux points de la edge divisee
		eid2=already_divided_edges[div_eid]
		edge_pts=set(mesh.borders(1,div_eid))|set(mesh.borders(1,eid2))
		face_corners-=edge_pts
		top_pid=face_corners.pop()
		#creation de la nouvelle edge
		new_eid=mesh.add_wisp(1)
		mesh.link(1,new_eid,top_pid)
		common_pid=set(mesh.borders(1,div_eid))&set(mesh.borders(1,eid2))
		common_pid=common_pid.pop()
		mesh.link(1,new_eid,common_pid)
		#division de la face
		fid1=mesh.add_wisp(2)
		mesh.link(2,fid1,new_eid)
		mesh.link(2,fid1,div_eid)
		eid=div_eid
		pid=opposite_point(mesh,eid,common_pid)
		while pid!=top_pid :
			eid=opposite_edge(mesh,fid,eid,pid)
			mesh.link(2,fid1,eid)
			pid=opposite_point(mesh,eid,pid)
		fid2=mesh.add_wisp(2)
		mesh.link(2,fid2,new_eid)
		mesh.link(2,fid2,eid2)
		eid=eid2
		pid=opposite_point(mesh,eid,common_pid)
		while pid!=top_pid :
			eid=opposite_edge(mesh,fid,eid,pid)
			mesh.link(2,fid2,eid)
			pid=opposite_point(mesh,eid,pid)
		#retrait de l'ancienne face
		mesh.remove_wisp(2,fid)
		#mise a jour des faces modifiees
		divided_faces.append( (fid,[fid1,fid2]) )
		#ajout des faces a la liste des faces a traiter si necessaire
		for fid in (fid1,fid2) :
			if mesh.nb_borders(2,fid)>3 :
				to_divide_face.add(fid)
	return divided_faces,divided_edges

