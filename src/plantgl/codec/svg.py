import openalea.plantgl.math as mt
import openalea.plantgl.scenegraph as sg

class SvgCodec (sg.SceneCodec) :
	"""
	codec that try to transform an svg file into a pgl scene
	and try to save all 2D objects into an svg file
	"""
	def __init__ (self) :
		sg.SceneCodec.__init__(self,"SVG",sg.SceneCodec.Mode.ReadWrite)
	
	def formats (self) :
		return [sg.SceneFormat("Svg codec",["svg"],"the Scalable Vector Graphics file format") ]
	
	def read (self, filename) :
		print "svg"
		sc=sg.Scene()
		return sc
	
	def write (self, filename ,scene) :
		f=open(filename,'w')
		for shp in scene :
			f.write(str(type(shp)))
		f.close()

codec = SvgCodec()
sg.SceneFactory.get().registerCodec(codec)
