import numpy as np
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
    #       acces aux elements
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
    #               operations comparaison
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
    #               operations algebriques
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
        except (TypeError, IndexError) :
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
        except (TypeError, IndexError) :
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
    #       operateurs plus specifiques
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
    #       access functions
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
#       predefined colors
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
#       convenience functions
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
#       predefined color_maps
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

class CurvMap (ColorRange) :
    """Created by Guillaume Cerutti 06/07/2014
    """
    def __init__ (self, val_min=0., val_max=1., **keys) :
        ColorRange.__init__(self,
                            (val_min,val_max),
                            [Color(0.125,0.9,1.0),Color(0.125,0.3,1.0),Color(0.125,0.125,0.3),Color(0.3,0.125,0.125),Color(1.0,0.3,0.125),Color(1.,0.9,0.125)],
                            (0.,.2,.45,0.55,.8,1.),
                            **keys)

class MegaMap (ColorRange) :
    def __init__ (self, val_min=0., val_max=1., **keys) :
        ColorRange.__init__(self,
                            (val_min,val_max),
                            [Color(0.,0.,0.5),Color(0.,0.5,0.8),Color(1.0,1.0,0.),Color(1.,0.5,0.)],
                            (0.,.3,.8,1.),
                            **keys)

class InvertGreyMap (ColorRange) :
    def __init__ (self, val_min=0., val_max=1., **keys) :
        ColorRange.__init__(self,
                            (val_min,val_max),
                            [Color(1.,1.,1),Color(0.5,0.5,0.55),Color(0.,0.,0.)],
                            (0.,.3,1.),
                            **keys)

class GlasbeyMap (ColorRange) :
    """Copied from the ImageJ glasbey lookup table
    """
    def __init__ (self, val_min=0., val_max=1., **keys) :
        ColorRange.__init__(self,
                            (val_min,val_max),
                            [Color( 1.0 , 1.0 , 1.0 ),
                            Color( 0.0 , 0.0 , 1.0 ),
                            Color( 1.0 , 0.0 , 0.0 ),
                            Color( 0.0 , 1.0 , 0.0 ),
                            Color( 0.0 , 0.0 , 0.2 ),
                            Color( 1.0 , 0.0 , 0.713725490196 ),
                            Color( 0.0 , 0.325490196078 , 0.0 ),
                            Color( 1.0 , 0.827450980392 , 0.0 ),
                            Color( 0.0 , 0.623529411765 , 1.0 ),
                            Color( 0.603921568627 , 0.301960784314 , 0.258823529412 ),
                            Color( 0.0 , 1.0 , 0.745098039216 ),
                            Color( 0.470588235294 , 0.247058823529 , 0.756862745098 ),
                            Color( 0.121568627451 , 0.588235294118 , 0.596078431373 ),
                            Color( 1.0 , 0.674509803922 , 0.992156862745 ),
                            Color( 0.694117647059 , 0.8 , 0.443137254902 ),
                            Color( 0.945098039216 , 0.0313725490196 , 0.360784313725 ),
                            Color( 0.996078431373 , 0.560784313725 , 0.258823529412 ),
                            Color( 0.866666666667 , 0.0 , 1.0 ),
                            Color( 0.125490196078 , 0.101960784314 , 0.00392156862745 ),
                            Color( 0.447058823529 , 0.0 , 0.333333333333 ),
                            Color( 0.462745098039 , 0.423529411765 , 0.58431372549 ),
                            Color( 0.0078431372549 , 0.678431372549 , 0.141176470588 ),
                            Color( 0.78431372549 , 1.0 , 0.0 ),
                            Color( 0.533333333333 , 0.423529411765 , 0.0 ),
                            Color( 1.0 , 0.717647058824 , 0.623529411765 ),
                            Color( 0.521568627451 , 0.521568627451 , 0.403921568627 ),
                            Color( 0.63137254902 , 0.0117647058824 , 0.0 ),
                            Color( 0.078431372549 , 0.976470588235 , 1.0 ),
                            Color( 0.0 , 0.278431372549 , 0.619607843137 ),
                            Color( 0.862745098039 , 0.36862745098 , 0.576470588235 ),
                            Color( 0.576470588235 , 0.83137254902 , 1.0 ),
                            Color( 0.0 , 0.298039215686 , 1.0 ),
                            Color( 0.0 , 0.258823529412 , 0.313725490196 ),
                            Color( 0.223529411765 , 0.654901960784 , 0.41568627451 ),
                            Color( 0.933333333333 , 0.439215686275 , 0.996078431373 ),
                            Color( 0.0 , 0.0 , 0.392156862745 ),
                            Color( 0.670588235294 , 0.960784313725 , 0.8 ),
                            Color( 0.63137254902 , 0.572549019608 , 1.0 ),
                            Color( 0.643137254902 , 1.0 , 0.450980392157 ),
                            Color( 1.0 , 0.807843137255 , 0.443137254902 ),
                            Color( 0.278431372549 , 0.0 , 0.0823529411765 ),
                            Color( 0.83137254902 , 0.678431372549 , 0.772549019608 ),
                            Color( 0.98431372549 , 0.462745098039 , 0.435294117647 ),
                            Color( 0.670588235294 , 0.737254901961 , 0.0 ),
                            Color( 0.458823529412 , 0.0 , 0.843137254902 ),
                            Color( 0.650980392157 , 0.0 , 0.603921568627 ),
                            Color( 0.0 , 0.450980392157 , 0.996078431373 ),
                            Color( 0.647058823529 , 0.364705882353 , 0.682352941176 ),
                            Color( 0.38431372549 , 0.517647058824 , 0.0078431372549 ),
                            Color( 0.0 , 0.474509803922 , 0.658823529412 ),
                            Color( 0.0 , 1.0 , 0.513725490196 ),
                            Color( 0.337254901961 , 0.207843137255 , 0.0 ),
                            Color( 0.623529411765 , 0.0 , 0.247058823529 ),
                            Color( 0.258823529412 , 0.176470588235 , 0.258823529412 ),
                            Color( 1.0 , 0.949019607843 , 0.733333333333 ),
                            Color( 0.0 , 0.364705882353 , 0.262745098039 ),
                            Color( 0.988235294118 , 1.0 , 0.486274509804 ),
                            Color( 0.623529411765 , 0.749019607843 , 0.729411764706 ),
                            Color( 0.654901960784 , 0.329411764706 , 0.0745098039216 ),
                            Color( 0.290196078431 , 0.152941176471 , 0.423529411765 ),
                            Color( 0.0 , 0.0627450980392 , 0.650980392157 ),
                            Color( 0.56862745098 , 0.305882352941 , 0.427450980392 ),
                            Color( 0.811764705882 , 0.58431372549 , 0.0 ),
                            Color( 0.764705882353 , 0.733333333333 , 1.0 ),
                            Color( 0.992156862745 , 0.266666666667 , 0.250980392157 ),
                            Color( 0.258823529412 , 0.305882352941 , 0.125490196078 ),
                            Color( 0.41568627451 , 0.00392156862745 , 0.0 ),
                            Color( 0.709803921569 , 0.513725490196 , 0.329411764706 ),
                            Color( 0.517647058824 , 0.913725490196 , 0.576470588235 ),
                            Color( 0.376470588235 , 0.850980392157 , 0.0 ),
                            Color( 1.0 , 0.435294117647 , 0.827450980392 ),
                            Color( 0.4 , 0.294117647059 , 0.247058823529 ),
                            Color( 0.996078431373 , 0.392156862745 , 0.0 ),
                            Color( 0.894117647059 , 0.0117647058824 , 0.498039215686 ),
                            Color( 0.0666666666667 , 0.780392156863 , 0.682352941176 ),
                            Color( 0.823529411765 , 0.505882352941 , 0.545098039216 ),
                            Color( 0.356862745098 , 0.462745098039 , 0.486274509804 ),
                            Color( 0.125490196078 , 0.23137254902 , 0.41568627451 ),
                            Color( 0.705882352941 , 0.329411764706 , 1.0 ),
                            Color( 0.886274509804 , 0.0313725490196 , 0.823529411765 ),
                            Color( 0.0 , 0.00392156862745 , 0.078431372549 ),
                            Color( 0.364705882353 , 0.517647058824 , 0.266666666667 ),
                            Color( 0.650980392157 , 0.980392156863 , 1.0 ),
                            Color( 0.380392156863 , 0.482352941176 , 0.788235294118 ),
                            Color( 0.38431372549 , 0.0 , 0.478431372549 ),
                            Color( 0.494117647059 , 0.745098039216 , 0.227450980392 ),
                            Color( 0.0 , 0.235294117647 , 0.717647058824 ),
                            Color( 1.0 , 0.992156862745 , 0.0 ),
                            Color( 0.0274509803922 , 0.772549019608 , 0.886274509804 ),
                            Color( 0.705882352941 , 0.654901960784 , 0.223529411765 ),
                            Color( 0.580392156863 , 0.729411764706 , 0.541176470588 ),
                            Color( 0.8 , 0.733333333333 , 0.627450980392 ),
                            Color( 0.21568627451 , 0.0 , 0.192156862745 ),
                            Color( 0.0 , 0.156862745098 , 0.00392156862745 ),
                            Color( 0.588235294118 , 0.478431372549 , 0.505882352941 ),
                            Color( 0.152941176471 , 0.533333333333 , 0.149019607843 ),
                            Color( 0.807843137255 , 0.509803921569 , 0.705882352941 ),
                            Color( 0.588235294118 , 0.643137254902 , 0.76862745098 ),
                            Color( 0.705882352941 , 0.125490196078 , 0.501960784314 ),
                            Color( 0.43137254902 , 0.337254901961 , 0.705882352941 ),
                            Color( 0.576470588235 , 0.0 , 0.725490196078 ),
                            Color( 0.780392156863 , 0.188235294118 , 0.239215686275 ),
                            Color( 0.450980392157 , 0.4 , 1.0 ),
                            Color( 0.0588235294118 , 0.733333333333 , 0.992156862745 ),
                            Color( 0.674509803922 , 0.643137254902 , 0.392156862745 ),
                            Color( 0.713725490196 , 0.458823529412 , 0.980392156863 ),
                            Color( 0.847058823529 , 0.862745098039 , 0.996078431373 ),
                            Color( 0.341176470588 , 0.552941176471 , 0.443137254902 ),
                            Color( 0.847058823529 , 0.333333333333 , 0.133333333333 ),
                            Color( 0.0 , 0.76862745098 , 0.403921568627 ),
                            Color( 0.952941176471 , 0.647058823529 , 0.411764705882 ),
                            Color( 0.847058823529 , 1.0 , 0.713725490196 ),
                            Color( 0.00392156862745 , 0.0941176470588 , 0.858823529412 ),
                            Color( 0.203921568627 , 0.258823529412 , 0.211764705882 ),
                            Color( 1.0 , 0.603921568627 , 0.0 ),
                            Color( 0.341176470588 , 0.372549019608 , 0.00392156862745 ),
                            Color( 0.776470588235 , 0.945098039216 , 0.309803921569 ),
                            Color( 1.0 , 0.372549019608 , 0.521568627451 ),
                            Color( 0.482352941176 , 0.674509803922 , 0.941176470588 ),
                            Color( 0.470588235294 , 0.392156862745 , 0.192156862745 ),
                            Color( 0.635294117647 , 0.521568627451 , 0.8 ),
                            Color( 0.411764705882 , 1.0 , 0.862745098039 ),
                            Color( 0.776470588235 , 0.321568627451 , 0.392156862745 ),
                            Color( 0.474509803922 , 0.101960784314 , 0.250980392157 ),
                            Color( 0.0 , 0.933333333333 , 0.274509803922 ),
                            Color( 0.905882352941 , 0.811764705882 , 0.270588235294 ),
                            Color( 0.850980392157 , 0.501960784314 , 0.913725490196 ),
                            Color( 1.0 , 0.827450980392 , 0.819607843137 ),
                            Color( 0.819607843137 , 1.0 , 0.552941176471 ),
                            Color( 0.141176470588 , 0.0 , 0.0117647058824 ),
                            Color( 0.341176470588 , 0.639215686275 , 0.756862745098 ),
                            Color( 0.827450980392 , 0.905882352941 , 0.788235294118 ),
                            Color( 0.796078431373 , 0.435294117647 , 0.309803921569 ),
                            Color( 0.243137254902 , 0.0941176470588 , 0.0 ),
                            Color( 0.0 , 0.458823529412 , 0.874509803922 ),
                            Color( 0.439215686275 , 0.690196078431 , 0.345098039216 ),
                            Color( 0.819607843137 , 0.0941176470588 , 0.0 ),
                            Color( 0.0 , 0.117647058824 , 0.419607843137 ),
                            Color( 0.411764705882 , 0.78431372549 , 0.772549019608 ),
                            Color( 1.0 , 0.796078431373 , 1.0 ),
                            Color( 0.913725490196 , 0.760784313725 , 0.537254901961 ),
                            Color( 0.749019607843 , 0.505882352941 , 0.180392156863 ),
                            Color( 0.270588235294 , 0.164705882353 , 0.56862745098 ),
                            Color( 0.670588235294 , 0.298039215686 , 0.760784313725 ),
                            Color( 0.0549019607843 , 0.458823529412 , 0.239215686275 ),
                            Color( 0.0 , 0.117647058824 , 0.0980392156863 ),
                            Color( 0.462745098039 , 0.286274509804 , 0.498039215686 ),
                            Color( 1.0 , 0.662745098039 , 0.78431372549 ),
                            Color( 0.36862745098 , 0.21568627451 , 0.850980392157 ),
                            Color( 0.933333333333 , 0.901960784314 , 0.541176470588 ),
                            Color( 0.623529411765 , 0.211764705882 , 0.129411764706 ),
                            Color( 0.313725490196 , 0.0 , 0.580392156863 ),
                            Color( 0.741176470588 , 0.564705882353 , 0.501960784314 ),
                            Color( 0.0 , 0.427450980392 , 0.494117647059 ),
                            Color( 0.345098039216 , 0.874509803922 , 0.376470588235 ),
                            Color( 0.278431372549 , 0.313725490196 , 0.403921568627 ),
                            Color( 0.00392156862745 , 0.364705882353 , 0.623529411765 ),
                            Color( 0.388235294118 , 0.188235294118 , 0.235294117647 ),
                            Color( 0.0078431372549 , 0.807843137255 , 0.580392156863 ),
                            Color( 0.545098039216 , 0.325490196078 , 0.145098039216 ),
                            Color( 0.670588235294 , 0.0 , 1.0 ),
                            Color( 0.552941176471 , 0.164705882353 , 0.529411764706 ),
                            Color( 0.333333333333 , 0.325490196078 , 0.580392156863 ),
                            Color( 0.588235294118 , 1.0 , 0.0 ),
                            Color( 0.0 , 0.596078431373 , 0.482352941176 ),
                            Color( 1.0 , 0.541176470588 , 0.796078431373 ),
                            Color( 0.870588235294 , 0.270588235294 , 0.78431372549 ),
                            Color( 0.419607843137 , 0.427450980392 , 0.901960784314 ),
                            Color( 0.117647058824 , 0.0 , 0.266666666667 ),
                            Color( 0.678431372549 , 0.298039215686 , 0.541176470588 ),
                            Color( 1.0 , 0.525490196078 , 0.63137254902 ),
                            Color( 0.0 , 0.137254901961 , 0.235294117647 ),
                            Color( 0.541176470588 , 0.803921568627 , 0.0 ),
                            Color( 0.435294117647 , 0.792156862745 , 0.61568627451 ),
                            Color( 0.882352941176 , 0.294117647059 , 0.992156862745 ),
                            Color( 1.0 , 0.690196078431 , 0.301960784314 ),
                            Color( 0.898039215686 , 0.909803921569 , 0.223529411765 ),
                            Color( 0.447058823529 , 0.0627450980392 , 1.0 ),
                            Color( 0.435294117647 , 0.321568627451 , 0.396078431373 ),
                            Color( 0.525490196078 , 0.537254901961 , 0.188235294118 ),
                            Color( 0.388235294118 , 0.149019607843 , 0.313725490196 ),
                            Color( 0.411764705882 , 0.149019607843 , 0.125490196078 ),
                            Color( 0.78431372549 , 0.43137254902 , 0.0 ),
                            Color( 0.819607843137 , 0.643137254902 , 1.0 ),
                            Color( 0.776470588235 , 0.823529411765 , 0.337254901961 ),
                            Color( 0.309803921569 , 0.403921568627 , 0.301960784314 ),
                            Color( 0.682352941176 , 0.647058823529 , 0.650980392157 ),
                            Color( 0.666666666667 , 0.176470588235 , 0.396078431373 ),
                            Color( 0.780392156863 , 0.317647058824 , 0.686274509804 ),
                            Color( 1.0 , 0.349019607843 , 0.674509803922 ),
                            Color( 0.572549019608 , 0.4 , 0.305882352941 ),
                            Color( 0.4 , 0.525490196078 , 0.721568627451 ),
                            Color( 0.435294117647 , 0.596078431373 , 1.0 ),
                            Color( 0.360784313725 , 1.0 , 0.623529411765 ),
                            Color( 0.674509803922 , 0.537254901961 , 0.698039215686 ),
                            Color( 0.823529411765 , 0.133333333333 , 0.38431372549 ),
                            Color( 0.780392156863 , 0.811764705882 , 0.576470588235 ),
                            Color( 1.0 , 0.725490196078 , 0.117647058824 ),
                            Color( 0.980392156863 , 0.580392156863 , 0.552941176471 ),
                            Color( 0.192156862745 , 0.133333333333 , 0.305882352941 ),
                            Color( 0.996078431373 , 0.317647058824 , 0.380392156863 ),
                            Color( 0.996078431373 , 0.552941176471 , 0.392156862745 ),
                            Color( 0.266666666667 , 0.211764705882 , 0.0901960784314 ),
                            Color( 0.788235294118 , 0.635294117647 , 0.329411764706 ),
                            Color( 0.780392156863 , 0.909803921569 , 0.941176470588 ),
                            Color( 0.266666666667 , 0.596078431373 , 0.0 ),
                            Color( 0.576470588235 , 0.674509803922 , 0.227450980392 ),
                            Color( 0.0862745098039 , 0.294117647059 , 0.109803921569 ),
                            Color( 0.0313725490196 , 0.329411764706 , 0.474509803922 ),
                            Color( 0.454901960784 , 0.176470588235 , 0.0 ),
                            Color( 0.407843137255 , 0.235294117647 , 1.0 ),
                            Color( 0.250980392157 , 0.160784313725 , 0.149019607843 ),
                            Color( 0.643137254902 , 0.443137254902 , 0.843137254902 ),
                            Color( 0.811764705882 , 0.0 , 0.607843137255 ),
                            Color( 0.462745098039 , 0.00392156862745 , 0.137254901961 ),
                            Color( 0.325490196078 , 0.0 , 0.345098039216 ),
                            Color( 0.0 , 0.321568627451 , 0.909803921569 ),
                            Color( 0.16862745098 , 0.360784313725 , 0.341176470588 ),
                            Color( 0.627450980392 , 0.850980392157 , 0.572549019608 ),
                            Color( 0.690196078431 , 0.101960784314 , 0.898039215686 ),
                            Color( 0.113725490196 , 0.0117647058824 , 0.141176470588 ),
                            Color( 0.478431372549 , 0.227450980392 , 0.623529411765 ),
                            Color( 0.839215686275 , 0.819607843137 , 0.811764705882 ),
                            Color( 0.627450980392 , 0.392156862745 , 0.411764705882 ),
                            Color( 0.41568627451 , 0.61568627451 , 0.627450980392 ),
                            Color( 0.6 , 0.858823529412 , 0.443137254902 ),
                            Color( 0.752941176471 , 0.219607843137 , 0.811764705882 ),
                            Color( 0.490196078431 , 1.0 , 0.349019607843 ),
                            Color( 0.58431372549 , 0.0 , 0.133333333333 ),
                            Color( 0.835294117647 , 0.635294117647 , 0.874509803922 ),
                            Color( 0.0862745098039 , 0.513725490196 , 0.8 ),
                            Color( 0.650980392157 , 0.976470588235 , 0.270588235294 ),
                            Color( 0.427450980392 , 0.411764705882 , 0.380392156863 ),
                            Color( 0.337254901961 , 0.737254901961 , 0.305882352941 ),
                            Color( 1.0 , 0.427450980392 , 0.317647058824 ),
                            Color( 1.0 , 0.0117647058824 , 0.972549019608 ),
                            Color( 1.0 , 0.0 , 0.286274509804 ),
                            Color( 0.792156862745 , 0.0 , 0.137254901961 ),
                            Color( 0.262745098039 , 0.427450980392 , 0.0705882352941 ),
                            Color( 0.917647058824 , 0.666666666667 , 0.678431372549 ),
                            Color( 0.749019607843 , 0.647058823529 , 0.0 ),
                            Color( 0.149019607843 , 0.172549019608 , 0.2 ),
                            Color( 0.333333333333 , 0.725490196078 , 0.0078431372549 ),
                            Color( 0.474509803922 , 0.713725490196 , 0.619607843137 ),
                            Color( 0.996078431373 , 0.925490196078 , 0.83137254902 ),
                            Color( 0.545098039216 , 0.647058823529 , 0.349019607843 ),
                            Color( 0.552941176471 , 0.996078431373 , 0.756862745098 ),
                            Color( 0.0 , 0.235294117647 , 0.16862745098 ),
                            Color( 0.247058823529 , 0.0666666666667 , 0.156862745098 ),
                            Color( 1.0 , 0.866666666667 , 0.964705882353 ),
                            Color( 0.0666666666667 , 0.101960784314 , 0.572549019608 ),
                            Color( 0.603921568627 , 0.258823529412 , 0.329411764706 ),
                            Color( 0.58431372549 , 0.61568627451 , 0.933333333333 ),
                            Color( 0.494117647059 , 0.509803921569 , 0.282352941176 ),
                            Color( 0.227450980392 , 0.0235294117647 , 0.396078431373 ),
                            Color( 0.741176470588 , 0.458823529412 , 0.396078431373 )],
                            tuple(np.arange(256)/255.),
                            **keys)



