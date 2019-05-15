from openalea.plantgl.scenegraph import Scene

class ShapeDatabase (dict) :
	"""
	a view of a scene as a database of shapes
	a dictionary of shape_id, shapes
	"""
	def scene (self) :
		"""
		convert the database into a plantgl scene
		"""
		sc=Scene()
		for shp_id,shp in self.items() :
			shp.id=shp_id
			sc.add(shp)
		return sc
	
	def read (self, filename) :
		"""
		read a database (a bgeom file)
		"""
		sc=Scene()
		sc.read(filename)#,'BGEOM')
		self.clear()
		for shp in sc :
			self[shp.id]=shp
	
	def save (self, filename) :
		"""
		save a database into a file (a bgeom file)
		"""
		sc=self.scene()
		sc.save(filename)#,'BGEOM')

