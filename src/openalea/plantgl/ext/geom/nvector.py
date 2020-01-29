import math
import operator
from .geometrical_concept import GeometricalConcept

class NVector (GeometricalConcept) :
	"""general vector in N dimensions
	
	:author: Jerome Chopard
	:license: don't touch"""
	
	def __init__ (self, vec=[]) :
		"""constructor
		
		:param vec: a list of coordinates
		:type vec: iter of (int|float)
		:rtype: NVector"""
		self._coord=[i for i in vec]
	
	def __str__ (self) :
		"""string representation of the vector
		
		:return: "<x1, x2, ..., xN>
		:rtype: str"""
		ret="<"
		for i in self :
			ret+="%.2f," % (i)
		ret+=">"
		return ret
	
	# ##########################################################
	#
	#		dimensions
	#
	# ##########################################################
	def __len__ (self) :
		"""number of coordinates, see `dim`
		
		:rtype: int"""
		return len(self._coord)
	
	def dim (self) :
		"""dimension of the vector
		
		:rtype: int"""
		return len(self._coord)
	
	def size (self) :
		"""norme of the vector
		
		:rtype: float"""
		return self.norm()
	
	# ##########################################################
	#
	#		acces aux elements
	#
	# ##########################################################
	def __getitem__ (self, index) :
		"""return the indexieme coordinates, or length+index if index<0
		
		:param index: index of the coordinate to retrieve
		:type index: int or slice
		:rtype: int|float"""
		return self._coord[index]
	
	def __setitem__ (self, index, value) :
		"""set the indexieme coordinate to value
		
		:Parameters:
			- `index` : index of the coordinate to modify
			- `value` : new value of the coordinate
		:Types:
			- `index` : int
			- `value` : int|float"""
		self._coord[index]=value
	
	def __iter__ (self) :
		"""iterator on coordinates
		
		:rtype: iter of (int|float)"""
		return self._coord.__iter__()
	
	# #######
	#
	# personnal magic alias
	#
	# #######
	def get_coordinate_x (self) :
		return self._coord[0]
	
	def set_coordinate_x (self,val) :
		self._coord[0]=val
	
	x=property(get_coordinate_x,set_coordinate_x)
	
	def get_coordinate_y (self) :
		return self._coord[1]
	
	def set_coordinate_y (self,val) :
		self._coord[1]=val
	
	y=property(get_coordinate_y,set_coordinate_y)
	
	def get_coordinate_z (self) :
		return self._coord[2]
	
	def set_coordinate_z (self,val) :
		self._coord[2]=val
	
	z=property(get_coordinate_z,set_coordinate_z)
	
	def get_coordinate_t (self) :
		return self._coord[3]
	
	def set_coordinate_t (self,val) :
		self._coord[3]=val
	
	t=property(get_coordinate_t,set_coordinate_t)
	
	# ##########################################################
	#
	#		comparison
	#
	# ##########################################################
	def approx (self, objet, seuil=1e-6) :
		"""approximation egality operator
		
		:Parameters:
			- `objet` : the objet to compare
			- `seuil` : seuil under wich two number are the same, default=1e-6
		:Types:
			- `objet` : any
			- `seuil` : float
		:rtype: bool"""
		for i,coord in enumerate(self) :
			if abs(coord-objet[i])>seuil : return False
		
		return True
	
	def binary_compare_operation (self, vector, comparator) :
		"""internal function comparator on each coordinate
		
		:Parameters:
			- `vector` : the second member of the test
			- `comparator` : test function
		:Types:
			- `vector` : float|iter of float
			- `comparator` : func(vector,vector)->bool
		:rtype: bool"""
		try :
			for i,vector_coord in enumerate(vector) :
				if not comparator(self[i],vector_coord) : return False
			
			return True
		except TypeError :
			for i,coord in enumerate(self) :
				if not comparator(coord,vector) : return False
				
			return True
	
	# ##########################################################
	#
	#		operations geometriques
	#
	# ##########################################################
	def binary_geometric_operation (self, vector, operation) :
		"""internal function applying operation to each coordinate
		
		:Parameters:
			- `vector` : the second member of the operation
			- `operation` : operation function
		:Types:
			- `vector` : float|iter of float
			- `operation` : func(vector,vector)->vector
		:rtype: self"""
		try :
			for i,vector_coord in enumerate(vector) :
				self[i]=operation(self[i],vector_coord)
		except TypeError :
			for i,coord in enumerate(self) :
				self[i]=operation(coord,vector)
		
		return self
	
	def __iadd__ (self, val) :
		"""translation in place
		
		:param val: displacement to add for each coordinate, duplicated to match the dimension of the vector
		:type val: float|iter of float
		:rtype: self"""
		return self.binary_geometric_operation(val,operator.add)
	
	def __isub__ (self, val) :
		"""translation in place opposite of `__iadd__`
		
		:param val: displacement to substract for each coordinate, duplicated to match the dimension of the vector
		:type val: float|iter of float
		:rtype: self"""
		return self.binary_geometric_operation(val,operator.sub)
	
	def __imul__ (self, val) :
		"""homothetie in place
		
		:param val: each coordinate is multiply by val, duplicated to match the dimension of the vector
		:type val: float|iter of float
		:rtype: self"""
		return self.binary_geometric_operation(val,operator.mul)
	
	def __idiv__ (self, val) :
		"""homothetie in place, inverse of `__imul__`
		
		:param val: each coordinate is divided by val, duplicated to match the dimension of the vector
		:type val: float|iter of float
		:rtype: self"""
		return self.binary_geometric_operation(val,operator.div)
	
	def __iand__ (self, vector) :
		"""minimize in place coordinates between self and vector
		
		:param vector: value compared with self, duplicated to match the dimension of the vector
		:type vector: float|iter of float
		:rtype: self"""
		return self.binary_geometric_operation(vector,min)
	
	def __ior__ (self, vector) :
		"""maximize in place coordinates between self and vector
		
		:param vector: value compared with self, duplicated to match the dimension of the vector
		:type vector: float|iter of float
		:rtype: self"""
		return self.binary_geometric_operation(vector,max)
	
	# ##########################################################
	#
	#		changement de referentiel
	#
	# ##########################################################
	def global_coordinates (self, basis) :
		"""change coordinate system to global coordinate system
		inverse function of `local_coordinates`
		
		:param basis: the local basis of reference
		:type basis: NBase
		:rtype: None"""
		global_vector=basis.origin().copy()
		for i,coord in enumerate(self._coord) :
			global_vector+=basis.axis(i)*coord
		
		self._coord=list(global_vector)
	
	def local_coordinates (self, basis) :
		"""change coordinate system to local coordinate system of the basis
		inverse function of `global_coordinates`
		
		:param basis: the local basis of reference
		:type basis: NBase
		:rtype: None"""
		local_vector=self-basis.origin()
		self._coord=[local_vector.dot(axis)/axis.normsquare() for axis in basis]
	
	# ##########################################################
	#
	#		operateurs plus specifiques
	#
	# ##########################################################
	def normsquare (self) :
		"""square of the norme of the vector
		
		:rtype: float"""
		return self.dot(self)
	
	def norm (self) :
		"""norme of the vector
		
		:rtype: float"""
		return math.sqrt(self.dot(self))
	
	def normalize (self) :
		"""change the coordinates in place to obtaint a normalized vector
		
		:rtype: None"""
		self.__idiv__(self.norm())
	
	def get_normalized (self) :
		"""construct a new vector which norme is 1
		
		:rtype: NVector"""
		return self.__div__(self.norm())
	
	def distance (self, vector) :
		"""euclidian distance between two vectors
		
		:param vector: second vector
		:type vector: iter of (int|float)
		:rtype: float"""
		return (self-vector).norm()
	
	def dot (self, vector) :
		"""scalar product of two vectors
		
		:warning: the two vectors must have the same dimension
		:param vector: the second vector
		:type vector: iter of (int|float)
		:rtype: float"""
		l=len(self)
		lb=len(vector)
		if l!=lb :
			raise UserWarning ("les deux vecteurs n'ont pas la meme dimension")
		else :
			return sum(self*vector)
	
	def cross (self, vector) :
		"""vector cross product
		
		:warning: only available in dimension 2 and 3
		:param vector: the second vector
		:type vector: iter of (int|float)
		:rtype: NVector"""
		d=self.dim()
		if d==2 :
			#le produit vectoriel entre deux vecteurs de dim 2 pose
			#plein de probleme. Est-ce qu'il faut renvoyer un scalaire
			#ou un vecteur de dimension 3.
			#je choisit de renvoyer un scalaire
			#pour permettre des operations du type
			#a^(b^c), j'autorise aussi de faire un produit vectoriel
			#entre un vecteur 2D et un scalaire, sous entendu ce scalaire
			#est le resultat d'un precedent produit scalaire
			if type(vector) in [int,float] :
				return type(self)([self[1]*vector,-self[0]*vector])
			else : return self[0]*vector[1]-self[1]*vector[0]
		elif d==3 :
			xx=self[1]*vector[2]-self[2]*vector[1]
			yy=self[2]*vector[0]-self[0]*vector[2]
			zz=self[0]*vector[1]-self[1]*vector[0]
			return type(self)([xx,yy,zz])
		else :
			raise NotImplementedError
		
	
	def find_normal (self) :
		"""create a new vector normal to this vector
		
		:return: a vector normal to this vector which norme is 1
		:rtype: NVector"""
		for i,coord in enumerate(self) :
			if coord==0 : return axis(i,self.dim())
		
		normal=zeros(self.dim())
		normal[0]=self[1]
		normal[1]=-self[0]
		normal.normalize()
		return normal
	
	#transformation rotation
	def rotate (self, angle=0, axis=None) :
		"""rotate the vector according to a given axis
		
		:Parameters:
			- `angle` : oriented angle in radian
			- `axis` : axis of the rotation, last axis if ``None``
		:Types:
			- `angle` : float
			- `axis` : NVector
		:rtype: self"""
		d=self.dim()
		if d==2 :
			ortho=NVector([-self[1],self[0]])
			self*=math.cos(angle)
			self+=ortho*math.sin(angle)
		elif d==3 :
			if axis is None :
				axe=NVector([0.,0.,1.])
			else :
				axe=axis.get_normalized()
			s=math.sin(angle)
			c=math.cos(angle)
			old_self=self.copy()
			self*=c
			self+=axe*((1-c)*old_self.dot(axe))+(axe.cross(old_self))*s
		else : raise NotImplementedError
	
	def get_rotated (self, angle=0, axis=None) :
		"""return a rotated copy of self according to a given axis
		
		:Parameters:
			- `angle` : oriented angle in radian
			- `axis` : axis of the rotation, last axis if ``None``
		:Types:
			- `angle` : float
			- `axis` : NVector
		:rtype: self"""
		ret=self.copy()
		ret.rotate(angle,axis)
		return ret
	
	def angle (self, vector=None) :
		"""angle between two vectors
		to go from vector to self
		
		:warning: orientation in dimension 3 is not garanteed
		:param vector: the second vector
		:type vector: NVector
		:return: an angle in radian
		:rtype: float"""
		if self.dim()==2 :
			if vector is None : vector=axis(0,dim=2)
			else : vector=vector.get_normalized()
			y=vector.cross(self)#[2]
			x=self.dot(vector)
			return math.atan2(y,x)
		else : 
			if vector is None : vector=axis(0,self.dim())
			else : vector=vector.get_normalized()
			axis_normal=vector.cross(self)
			if axis_normal.norm()<1e-6 :
				if self.dot(vector)>0 : return 0.
				else : return math.pi
			
			axis_proj=axis_normal.cross(vector)
			axis_proj.normalize()
			y=self.dot(axis_proj)
			x=self.dot(vector)
			return math.atan2(y,x)

	

# ##########################################################
#
#		convenience functions
#
# ##########################################################
def create (coeff=[]) :
	"""return a NVector initialised with coeff
	
	:rtype: NVector"""
	return NVector(coeff)

def ensure_vector (vector) :
	"""ensure the returned object is an instance of NVector
	
	:rtype: NVector"""
	if isinstance(vector,NVector) : return vector
	else : return NVector(vector)

def point2D (x=0, y=0) :
	"""return a integer NVector of dimension 2 initialised with x and y
	
	:rtype: NVector"""
	return NVector([x,y]).__int__()

def point3D (x=0, y=0, z=0) :
	"""return a integer NVector of dimension 3 initialised with x, y and z
	
	:rtype: NVector"""
	return NVector([x,y,z]).__int__()

def point4D (x=0, y=0, z=0, t=0) :
	"""return a integer NVector of dimension 4 initialised with x, y, z and t
	
	:rtype: NVector"""
	return NVector([x,y,z,t]).__int__()

def xy (x=0.,y=0.) :
	"""return a NVector of dimension 2 initialised with x and y
	
	:rtype: NVector"""
	return NVector([x,y]).__float__()

def xyz (x=0.,y=0.,z=0.) :
	"""return a NVector of dimension 3 initialised with x, y and z
	
	:rtype: NVector"""
	return NVector([x,y,z]).__float__()

def xyzt (x=0.,y=0.,z=0.,t=0.) :
	"""return a NVector of dimension 4 initialised with x, y, z and t
	
	:rtype: NVector"""
	return NVector([x,y,z,t]).__float__()

def zeros (dim=3) :
	"""return a vector initialised to 0
	
	:param dim: dimension of the vector to be created, default=3
	:type dim: int
	:rtype: NVector"""
	return NVector([0.]*dim)

def ones (dim=3) :
	"""return a vector initialised to 1
	
	:param dim: dimension of the vector to be created, default=3
	:type dim: int
	:rtype: NVector"""

	return NVector([1.]*dim)

def axis (order=0, dim=3) :
	"""return the orderieme axis, i.e. a vector with only the orderieme coordinate non nul
	
	:Parameters:
		- `order` : specify wich coordinate is non nul, default=0
		- `dim` : dimension of the vector to be created, default=3
	:Types:
		- `order` : int
		- `dim` : int
	:rtype: NVector"""
	ret=zeros(dim)
	ret[order]=1.
	return ret
