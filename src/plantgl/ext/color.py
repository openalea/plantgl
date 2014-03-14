import random as rd

class Color (object):
	"""Classe managing simple color : R,G,B and Transparency
	
	:author: Jerome Chopard
	:license: don't touch"""
	
	def __init__ (self, red=0, green=0, blue=0, alpha=1) :
		"""constructor, store the components of a color with values between 0 and 1
		
		:Parameters:
			- `red` : red channel, defaul=0
			- `green` : green channel, defaul=0
			- `blue` : blue channel, defaul=0
			- `alpha` : alpha channel (oppacity), defaul=1
		:Types:
			- `red` : float
			- `green` : float
			- `blue` : float
			- `alpha` : float"""
		self._red=float(red)
		self._green=float(green)
		self._blue=float(blue)
		self._alpha=float(alpha)
	
	def __str__ (self) :
		"""string representation : red,green,blue,alpha
		
		:rtype: str"""
		return ("color : %d\t%d\t%d\t%.1f" % (self.red, self.green, self.blue, self.alpha))
	
	def copy (self) :
		"""deep copy
		
		:rtype: Color"""
		return type(self)(self._red, self._green, self._blue, self._alpha)
	
	# ##########################################################
	#
	#		acces aux elements
	#
	# ##########################################################
	def __getitem__ (self, index) :
		"""acces to color channels
		index =0 (red) =1 (green) =2 (blue) =3 (alpha)
		
		:param index: channel to retrieve
		:type index: int
		:rtype: float"""
		if index==0 : return self._red
		elif index==1 : return self._green
		elif index==2 : return self._blue
		elif index==3 : return self._alpha
		else :
			raise IndexError(str(index))
	
	def __setitem__ (self, index, value) :
		"""set color channels
		index =0 (red) =1 (green) =2 (blue) =3 (alpha)
		
		:Parameters:
			- `index` : channel to set
			- `value` : the new value bound between 0 and 1
		:Types:
			- `index` : int
			- `value` : float
		:rtype: float"""
		if index==0 : self._red=value
		elif index==1 : self._green=value
		elif index==2 : self._blue=value
		elif index==3 : self._alpha=value
		else :
			raise IndexError(str(index))
	
	def __iter__ (self) :
		"""iterator on all channels
		
		:rtype: iter of float"""
		yield self._red
		yield self._green
		yield self._blue
		yield self._alpha
	
	def get_int_channel (self, index) :
		"""return the value of a channel between 0 and 255
		index =0 (red) =1 (green) =2 (blue) =3 (alpha)
		
		:param index: index of the channel to retrieve
		:type index: int
		:rtype: int"""
		return int(255*self[index])
	
	def set_int_channel (self, index, value) :
		"""set color channels from a value betwwen 0 and 255
		index =0 (red) =1 (green) =2 (blue) =3 (alpha)
		
		:Parameters:
			- `index` : channel to set
			- `value` : the new value bound between 0 and 1
		:Types:
			- `index` : int
			- `value` : float
		:rtype: float"""
		self[index]=min(255,max(0,value))/255.
	
	#RED
	def get_red (self) :
		return self.get_int_channel(0)
	
	def set_red (self, value) :
		self.set_int_channel(0,value)
	
	red=property(get_red,set_red)
	
	#GREEN
	def get_green (self) :
		return self.get_int_channel(1)
	
	def set_green (self, value) :
		self.set_int_channel(1,value)
	
	green=property(get_green,set_green)
	
	#BLUE
	def get_blue (self) :
		return self.get_int_channel(2)
	
	def set_blue (self, value) :
		self.set_int_channel(2,value)
	
	blue=property(get_blue,set_blue)
	
	#ALPHA
	def get_alpha (self) :
		return self.get_int_channel(3)
	
	def set_alpha (self, value) :
		self.set_int_channel(3,value)
	
	alpha=property(get_alpha,set_alpha)
	
	#TRANSPARENCY
	def get_transparency (self) :
		return 255-self.get_int_channel(3)
	
	def set_transparency (self, value) :
		self.set_int_channel(3,255-value)
	
	transparency=property(get_transparency,set_transparency)
	
	# ##########################################################
	#
	#				operations comparaison
	#
	# ##########################################################
	def __eq__ (self, color) :
		"""equality operator
		, if color has length 3 don't use alpha channel for comparison
		
		:param color: the second member
		:type color: iter of float
		:rtype: bool"""
		try :
			for ind,channel in enumerate(color) :
				if self[ind]!=channel : return False
		except TypeError :
			return False
		
		return True
	
	# ##########################################################
	#
	#				operations algebriques
	#
	# ##########################################################
	def __iadd__ (self, color) :
		"""add a second color in place
		the resulting alpha channel is the average of the two inital alpha channels
		the resulting color may have channels beyond the official bounds
		use `normalize`
		
		:param color: the values to add
		:type color: float|iter of float
		:rtype: self"""
		try :
			for i in xrange(3) : self[i]+=color[i]
			self[3]=(self[3]+color[3])/2.
		except TypeError :
			for i in xrange(3) : self[i]+=color
		
		return self
	
	def __add__ (self, color) :
		"""return a copy of self plus color
		see `__iadd__`
		
		:param color: the values to add
		:type color: float|iter of float
		:rtype: self"""
		ret=self.copy()
		ret+=color
		return ret
	
	def __isub__ (self, color) :
		"""substract a second color in place
		see `__iadd__` for more explanations
		
		:param color: the values to add
		:type color: float|iter of float
		:rtype: self"""
		try :
			for i in xrange(3) : self[i]-=color[i]
			self[3]=(self[3]+color[3])/2.
		except TypeError :
			for i in xrange(3) : self[i]-=color
		
		return self
	
	def __sub__ (self, color) :
		"""return a copy of self minus color
		see `__isub__`
		
		:param color: the values to substract
		:type color: float|iter of float
		:rtype: self"""
		ret=self.copy()
		ret-=color
		return ret
	
	def __imul__ (self, val) :
		"""change luminosity of the color
		alpha channel is unchanged
		the resulting color may have channels beyond the official bounds
		use `normalize`
		
		:param val: multiply factor
		:type val: float|iter of float
		:rtype: self"""
		try :
			for i in xrange(3) : self[i]*=val[i]
		except TypeError :
			for i in xrange(3) : self[i]*=val
		
		return self
	
	def __mul__ (self, val) :
		"""return a copy of self multiplied by color
		see `__imul__`
		
		:param val: multiply factor
		:type val: float|iter of float
		:rtype: self"""
		ret=self.copy()
		ret*=val
		return ret
	
	def __idiv__ (self, val) :
		"""change luminosity of the color
		see `__imul__` for more informations
		
		:param val: divide factor
		:type val: float|iter of float
		:rtype: self"""
		try :
			for i in xrange(3) : self[i]/=val[i]
		except TypeError :
			for i in xrange(3) : self[i]/=val
		
		return self
	
	def __div__ (self, val) :
		"""return a copy of self divided by color
		see `__idiv__`
		
		:param val: multiply factor
		:type val: float|iter of float
		:rtype: self"""
		ret=self.copy()
		ret/=val
		return ret
	
	# ##########################################################
	#
	#		operateurs plus specifiques
	#
	# ##########################################################
	def combine (self, color) :
		"""combine self with this color, considering color is under self
		alpha channel play a major role here
		if self.alpha==1 no change are visible
		
		:param color: background color
		:type color: Color
		:rtype: self"""
		self_alpha=self._alpha
		color_alpha=color[3]
		self[3]=self_alpha+color_alpha-self_alpha*color_alpha
		for i in xrange(3) : self[i]=(self[i]*self_alpha+color[i]*color_alpha*(1-self_alpha))/self[3]
		return self
	
	def __mod__ (self, color) :
		"""return a combined copy of self with color
		see `combine`
		
		:param color: background color
		:type color: Color
		:rtype: self"""
		ret=self.copy()
		ret.combine(color)
		return ret
	
	def normalize (self) :
		"""modify each channel to ensure it is bound between 0 and 1"""
		for i,channel in enumerate(self) :
			self[i]=max(0.,channel)
			self[i]=min(1.,channel)
	
	def get_normalized (self) :
		"""return a normalized copy of self
		
		:rtype: Color"""
		ret=self.copy()
		ret.normalize()
		return ret
	
	def get_CMYB (self) :
		"""return the channels in CMYB mode"""
		raise NotImplementedError
	
	######################################
	#
	#		access functions
	#
	######################################
	def i3tuple (self) :
		return (self.red,self.green,self.blue)
	
	def i4tuple (self) :
		return (self.red,self.green,self.blue,self.alpha)
	
	def f3tuple (self) :
		return (self._red,self._green,self._blue)
	
	def f4tuple (self) :
		return tuple(self)

# ##########################################################
#
#		predefined colors
#
# ##########################################################
red = Color(1.,0.,0.)
green = Color(0.,1.,0.)
blue = Color(0.,0.,1.)
yellow = Color(1.,1.,0.)
magenta = Color(1.,0.,1.)
cyan = Color(0.,1.,1.)
white = Color(1.,1.,1.)
black = Color(0.,0.,0.)

color_list=[Color(0.,0.,0.),
			Color(1.,0.,0.),
			Color(0.,1.,0.),
			Color(0.,0.,1.),
			Color(1.,1.,0.),
			Color(0.,1.,1.),
			Color(1.,0.,1.),
			Color(0.5,1.,0.),
			Color(0.,0.5,1.),
			Color(1.,0.,0.5),
			Color(0.,1.,0.5),
			Color(0.5,0.,1.),
			Color(1.,0.5,0.),
			Color(0.5,0.5,1.),
			Color(1.,0.5,0.5),
			Color(0.5,1.,0.5),
			Color(1.,1.,1.)
			]

# ##########################################################
#
#		convenience functions
#
# ##########################################################
def create (red=0, green=0, blue=0, alpha=1) :
	"""create a color from the given channels
	same as `Color.__init__`"""
	return Color(red,green,blue,alpha)

def int_create (red=0,green=0,blue=0,alpha=255) :
	"""create color with integer channels between 0 and 255"""
	c=Color()
	c.red=red
	c.green=green
	c.blue=blue
	c.alpha=alpha
	return c

def random () :
	"""create a random color with alpha=1
	color channels are uniformly reparted"""
	return Color(rd.random(),rd.random(),rd.random(),1)

def gauss_bound (mean, sd) :
	"""internal function, gaussian bounded by 0 and 1"""
	return max(0.,min(1.,rd.gauss(mean,sd)))

def gauss (mean=0.5, sd=0.2) :
	"""create a random color with alpha=1
	color channel follow a gaussian"""
	try :
		iter(mean)
	except TypeError :
		mean=[mean]*3
	try :
		iter(sd)
	except TypeError :
		sd=[sd]*3
	return Color(gauss_bound(mean[0],sd[0]),gauss_bound(mean[1],sd[1]),gauss_bound(mean[2],sd[2]),1.)

class ColorMap (object) :
	"""Functor used to associate a color to
	a set of values.
	"""
	
	def __init__ (self) :
		pass
	
	def get_color (self, value) :
		"""Return the color corresponding
		to this value in the color map.
		
		value: float or int or whatever
		return: Color
		"""
		pass
	
	def __call__ (self, value) :
		"""Magic alias for `get_color`
		"""
		return self.get_color(value)
	
	def invert (self, color) :
		"""Compute the value that will give color
		using this color range
		invert function of get_color
		"""
		pass

class ColorRange (ColorMap) :
	"""Usefull class for creating a range of colors
	like degrade of photoshop
	
	:author: Jerome Chopard
	:license: don't touch"""
	
	def __init__ (self, value_range=(0.0,1.0),
	                    color_list=[blue,green,red],
	                    position_list=None,
	                    outside_values=False) :
		"""
		constructor
		
		:Parameters:
			- `value_range` : value range in which we pick values
			- `color_list` : ordonnate list of color to interpolate
			- `position_list` : relative position (between 0 and 1) of the specified color
			- `outside_values`: if true value outside of range are truncated
		:Types:
			- `value_range` : (float,float)
			- `color_list` : list of `Color`
			- `position_list` : list of float
			- `outside_values` : bool
		"""
		ColorMap.__init__(self)
		self.set_value_range( value_range )
		
		if len(color_list)<=1 :
			raise ValueError ("list of color too small %d, need at least two values" % len(color_list))
		self._color_list=color_list
		
		if position_list is None :
			l=len(color_list)-1
			self._position_list=[float(i)/l for i in xrange(l)]
			self._position_list.append(1.)
		else :
			if len(position_list)!=len(color_list) :
				raise ValueError ("color_list and position_list must have the same number of items")
			self._position_list=position_list
		
		self.outside_values=outside_values

	def set_value_range( self, value_range=(0., 1.) ):
		"""sets a value  range.
		
		:Parameters:
			- `value_range` : value range in which we pick values
		:Types:
			- `value_range` : (float,float)
		
		"""
		self._value_min = float(value_range[0])
		self._value_max = float(value_range[1])
		if self._value_min >= self._value_max :
			raise ValueError("max==min %f" % self._value_max)

	
	def get_color (self, value) :
		"""compute the color corresponding to value by interpolating colors
		
		:param value: the value for wich you need a color
		:type value: float
		:rtype: Color"""
		norm=(float(value)-self._value_min)/(self._value_max-self._value_min)
		if norm>1. :
			if self.outside_values :
				norm=1.
			else :
				raise ValueError ("value %.3f outside of range : [%.3f,%.3f]" % (value,self._value_min,self._value_max))
		if norm<0. :
			if self.outside_values :
				norm=0.
			else :
				raise ValueError ("value %.3f outside of range : [%.3f,%.3f]" % (value,self._value_min,self._value_max))
		i=1
		while self._position_list[i]<norm : i+=1
		c_inf=self._color_list[i-1]
		c_sup=self._color_list[i]
		value_pos=(norm-self._position_list[i-1])/(self._position_list[i]-self._position_list[i-1])
		return c_inf*(1-value_pos)+c_sup*value_pos
	
	def __call__ (self, value) :
		"""magic alias for `get_color`
		
		:param value: the value for which you need a color
		:type value: float
		:rtype: Color"""
		return self.get_color(value)
	
	def invert (self, color) :
		"""
		compute the value that will give color
		using this color range
		invert function of get_color
		"""
		print list(color)
		for i in xrange(1,len(self._color_list)) :
			c_inf=self._color_list[i-1]
			c_sup=self._color_list[i]
			print list(c_inf),"sup",list(c_sup)
			val=[]
			for ind in xrange(3) :
				if c_sup[ind]==c_inf[ind] :
					if color[ind]!=c_sup[ind] :
						val.append(-1)
				else :
					val.append((color[ind]-c_inf[ind])/(c_sup[ind]-c_inf[ind]))
			if len(val)==0 :
				raise UserWarning("color range not valid")
			else :
				if -1 not in val :
					test=True
					val_ref=val[0]
					for c in val :
						if 0.<=c<=1. :
							if abs(c-val_ref)>1e-6 :
								test=False
						else :
							test=False
					if test :
						#youpi c'est la bonne
						norm=self.position_list[i-1]+val_ref*(self._position_list[i]-self._position_list[i-1])
						return self._value_min+norm*(self._value_max-self._value_min)
		raise UserWarning("color outside of range")

##########################
#
#		predefined color_maps
#
##########################
class IntMap (ColorMap) :
	def get_color (self, value) :
		return color_list[value % len(color_list)]
	
	def invert (self, color) :
		raise NotImplementedError("")

class GrayMap (ColorRange) :
	def __init__ (self, val_min=0., val_max=1., **keys) :
		ColorRange.__init__(self,
		                    (val_min,val_max),
		                    [black,white],
		                    **keys)

class JetMap (ColorRange) :
	def __init__ (self, val_min=0., val_max=1., **keys) :
		ColorRange.__init__(self,
		                    (val_min,val_max),
		                    [Color(0.,0.,0.3),blue,green,
		                     yellow,red,Color(0.3,0.,0.)],
		                    (0.,0.15,0.3,0.7,0.85,1.),
		                    **keys)

class TerrainMap (ColorRange) :
	def __init__ (self, val_min=0., val_max=1., **keys) :
		ColorRange.__init__(self,
		                    (val_min,val_max),
		                    [Color(0.,0.651,0.),Color(0.902,0.902,0.),
		                     Color(0.925,0.694,0.463),Color(0.949,0.949,0.949)],
		                    (0.,0.4,0.8,1.),
		                    **keys)

class HeatMap (ColorRange) :
	def __init__ (self, val_min=0., val_max=1., **keys) :
		ColorRange.__init__(self,
		                    (val_min,val_max),
		                    [Color(0.2,0.,0.),red,
		                     yellow,white,Color(0.3,0.3,1.)],
		                    (0.,0.3,0.6,0.9,1.),
		                    **keys)

class JetMapWithoutRed (ColorRange) :
	def __init__ (self, val_min=0., val_max=1., **keys) :
		ColorRange.__init__(self,
		                    (val_min,val_max),
		                    [Color(0.,0.,0.3),blue,green,
		                     yellow,magenta],
		                    (0.,0.2,0.5,0.8,1.),
		                    **keys)

class GreenMap (ColorRange) :
	def __init__ (self, val_min=0., val_max=1., **keys) :
		ColorRange.__init__(self,
		                    (val_min,val_max),
		                    [Color(0.,0.,0.),Color(0.,0.1,0.),green],
		                    (0.,.5,1.),
		                    **keys)



class BlueMap (ColorRange) :
	"""Created by Michael Walker 16/05/2011. If Jerome has a problem he can sue me.
	"""
	def __init__ (self, val_min=0., val_max=1., **keys) :
		ColorRange.__init__(self,
		                    (val_min,val_max),
		                    [Color(.7,.7,1.),Color(0.5,0.5,.9),Color(.1,.1,.7)],
		                    (0.,.5,1.),
		                    **keys)



