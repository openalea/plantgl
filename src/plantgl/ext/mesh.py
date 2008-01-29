from openalea.plantgl.math import Vector3
from openalea.plantgl.scenegraph import TriangleSet

def discretisize_curve (curve, nb_points) :
	Ufunc=curve.getArcLengthToUMapping()
	Ufunc.clamped=False
	return [curve.getPointAt(Ufunc(i/float(nb_points))) for i in xrange(nb_points)]

class Mesh (TriangleSet) :
	"""
	extend plantgl mesh and provide editing functions
	"""
	def __init__ (self) :
		TriangleSet.__init__(self,[],[])
	
	def vertices (self) :
		return xrange(len(self.pointList))
	
	def faces (self) :
		return xrange(len(self.indexList))
	
	def vertex (self, fid, ind) :
		"""
		return the indexieme vertex of the face
		"""
		return self.indexList[fid][ind]
	
	def vertex_position (self, vid) :
		return self.pointAt(vid)
	
	def add_vertex (self, pos) :
		self.pointList.append(pos)
		return len(self.pointList)-1
	
	def add_face (self, corners) :
		self.indexList.append(corners)
		return len(self.indexList)-1
	
	def flip_face (self, fid) :
		corners=list(self.indexList[fid])
		corners.reverse()
		self.indexList[fid]=corners
		#TODO flip normals too
	
	def centroid (self) :
		return sum(self.pointList,Vector3())/len(self.pointList)
	
	def min_dist (self, path1, path2) :
		def ds (vid1, vid2) :
			return (self.vertex_position(vid1)-self.vertex_position(vid2)).__normSquared__()
		dists=[]
		for vid1 in path1 :
			for vid2 in path2 :
				dists.append( (ds(vid1,vid2),(vid1,vid2)) )
		dists.sort()
		return dists[0][1]
	
	def fill_vertex_loop (self, vertex_loop) :
		"""
		fill a ring of vertices with faces connected to the centroid of the vertices
		"""
		nb=len(vertex_loop)
		cent=sum((self.vertex_position(vid) for vid in vertex_loop),Vector3())/nb
		ind=self.add_vertex(cent)
		for i in xrange(nb) :
			self.add_face( (ind,vertex_loop[i],vertex_loop[(i+1)%nb]) )
	
	def fill_vertex_path (self, path1, path2, closed=False) :
		"""
		fill the gap between two pathes with faces
		if closed is True, the two pathes are in fact loops of vertices
		"""
		def d (i1, i2) :
			return (self.vertex_position(path1[i1])-self.vertex_position(path2[i2])).__norm__()
		len1=len(path1)
		len2=len(path2)
		if closed :
			nb1=len1
			nb2=len2
			i1_offset,i2_offset=self.min_dist(path1,path2)
		else :
			nb1=len1-1
			nb2=len2-1
			i1_offset,i2_offset=0,0
		i1=0
		i2=0
		while i1<nb1 and i2<nb2 :
			if d((i1_offset+i1)%len1,(i2_offset+i2+1)%len2)<d((i1_offset+i1+1)%len1,(i2_offset+i2)%len2) :
				self.add_face( (path1[(i1_offset+i1)%len1],path2[(i2_offset+i2+1)%len2],path2[(i2_offset+i2)%len2]) )
				i2+=1
			else :
				self.add_face( (path1[(i1_offset+i1)%len1],path1[(i1_offset+i1+1)%len1],path2[(i2_offset+i2)%len2]) )
				i1+=1
		if i1 == nb1 :
			for i in xrange(i2,nb2) :
				self.add_face( (path1[(i1_offset+i1)%len1],path2[(i2_offset+i+1)%len2],path2[(i2_offset+i)%len2]) )
		elif i2 == nb2 :
			for i in xrange(i1,nb1) :
				self.add_face( (path1[(i1_offset+i)%len1],path1[(i1_offset+i+1)%len1],path2[(i2_offset+i2)%len2]) )

