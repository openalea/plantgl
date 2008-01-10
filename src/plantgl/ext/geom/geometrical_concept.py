import operator

class GeometricalConcept (object) :
	"""base class for implementing geometrical objects
	
	:author: Jerome Chopard
	:license: don't touch"""
	
	def copy (self) :
		"""deep copy
		
		:rtype: type(self)"""
		return type(self)(self)
	
	def __str__ (self) :
		raise NotImplementedError
	
	# ##########################################################
	#
	#		pickling
	#
	# ##########################################################
	"""def __getstate__ (self) :
		return self.__dict__
	
	def __setstate__ (self, dict) :
		self.__dict__.update(dict)"""
	
	# ##########################################################
	#
	#		dimensions
	#
	# ##########################################################
	def __len__ (self) :
		"""number of elements in self
		
		:rtype: int"""
		raise NotImplementedError
	
	def dim (self) :
		"""dimension of the elements space
		
		:rtype: int"""
		raise NotImplementedError
	
	def size (self) :
		"""spatial extension of self
		
		:rtype: float"""
		raise NotImplementedError
	
	# ##########################################################
	#
	#		comparison
	#
	# ##########################################################
	def binary_compare_operation (self, objet, comparator) :
		"""internal function comparator on objet
		
		:Parameters:
			- `objet` : the second member of the test
			- `comparator` : test function
		:Types:
			- `objet` : float|iter of float
			- `comparator` : func(self,objet)->bool
		:rtype: bool"""
		raise NotImplementedError
	
	def __eq__ (self, objet) :
		"""egality operator
		
		:param objet: the objet to compare
		:type objet: any
		:rtype: bool"""
		return self.binary_compare_operation(objet,operator.eq)
	
	def __ne__ (self, objet) :
		"""difference operator need `__eq__` to be implemented
		
		:return: not `__eq__`
		:rtype: bool"""
		return not self.__eq__(objet)
	
	def __le__ (self, objet) :
		"""use `binary_compare_operation` with __le__
		
		:type objet: type(self)
		:rtype: bool"""
		return self.binary_compare_operation(objet,operator.le)
	
	def __lt__ (self, objet) :
		"""use `binary_compare_operation` with __lt__
		
		:type objet: type(self)
		:rtype: bool"""
		return self.binary_compare_operation(objet,operator.lt)
	
	def __ge__ (self, objet) :
		"""use `binary_compare_operation` with __ge__
		
		:type objet: type(self)
		:rtype: bool"""
		return self.binary_compare_operation(objet,operator.ge)
	
	def __gt__ (self, objet) :
		"""use `binary_compare_operation` with __gt__
		
		:type objet: type(self)
		:rtype: bool"""
		return self.binary_compare_operation(objet,operator.gt)
	
	# ##########################################################
	#
	#		changement de type des coordonnees
	#
	# ##########################################################
	def apply_function (self, func) :
		"""new object with coordinates modified according to func
		
		:rtype: type(self)"""
		ret=self.copy()
		for i,coord in enumerate(self) :
			ret[i]=func(coord)
		return ret
	
	def __abs__ (self) :
		"""new object with positives coordinates
		
		:rtype: type(self)"""
		return self.apply_function(abs)
	
	def __int__ (self) :
		"""new object with integer coordinates
		
		:rtype: type(self)"""
		return self.apply_function(int)
	
	def __float__ (self) :
		"""new object with float coordinates
		
		:rtype: type(self)"""
		return self.apply_function(float)
	
	# ##########################################################
	#
	#		transformations geometriques
	#
	# ##########################################################
	def __iadd__ (self, val) :
		raise NotImplementedError
	
	def __add__ (self, val) :
		"""new object result of self+val
		
		:rtype: type(self)"""
		ret=self.copy()
		ret+=val
		return ret
	
	def __isub__ (self, val) :
		raise NotImplementedError
	
	def __sub__ (self, val) :
		"""new object result of self-val
		
		:rtype: type(self)"""
		ret=self.copy()
		ret-=val
		return ret
	
	def __imul__ (self, val) :
		raise NotImplementedError
	
	def __mul__ (self, val) :
		"""new object result of self*val
		
		:rtype: type(self)"""
		ret=self.copy()
		ret*=val
		return ret
	
	def __neg__ (self) :
		"""new object result of self*-1
		
		:rtype: type(self)"""
		return self*-1.
	
	def __idiv__ (self, val) :
		raise NotImplementedError
	
	def __div__ (self, val) :
		"""new object result of self/val
		
		:rtype: type(self)"""

		ret=self.copy()
		ret/=val
		return ret
	
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
		raise NotImplementedError
	
	def __lshift__ (self, basis) :
		"""magic function for `global_coordinates`"""
		obj=self.copy()
		obj.global_coordinates(basis)
		return obj
	
	def local_coordinates (self, basis) :
		"""change coordinate system to local coordinate system of the basis
		inverse function of `global_coordinates`
		
		:param basis: the local basis of reference
		:type basis: NBase
		:rtype: None"""
		raise NotImplementedError
	
	def __rshift__ (self, basis) :
		"""magic function for `local_coordinates`"""
		obj=self.copy()
		obj.local_coordinates(basis)
		return obj
	
	# ##########################################################
	#
	#		operations booleennes
	#
	# ##########################################################
	def __iand__ (self, objet) :
		raise NotImplementedError
	
	def __and__ (self, objet) :
		"""new object result of self&objet
		
		:rtype: type(self)"""
		ret=self.copy()
		ret&=objet
		return ret
	
	def __ior__ (self, objet) :
		raise NotImplementedError
	
	def __or__ (self, objet) :
		"""new object result of self|objet
		
		:rtype: type(self)"""
		ret=self.copy()
		ret|=objet
		return ret
	
