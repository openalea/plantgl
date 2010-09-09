# -*- python -*-
#
#       OpenAlea.Visualea: OpenAlea graphical user interface
#
#       Copyright 2010 INRIA - CIRAD - INRA
#
#       File author(s): Daniel Barbeau <daniel.barbeau@sophia.inria.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
#
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
###############################################################################
#  ----------------------------------------------------------------------------
#
#       PlantGL: The Plant Graphic Library
#
#       Copyright 2006-2010 UMR CIRAD/INRIA/INRA DAP
#
#       File author(s): D. Barbeau et al.
#
#  ----------------------------------------------------------------------------
#
#                      GNU General Public Licence
#
#       This program is free software; you can redistribute it and/or
#       modify it under the terms of the GNU General Public License as
#       published by the Free Software Foundation; either version 2 of
#       the License, or (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public
#       License along with this program; see the file COPYING. If not,
#       write to the Free Software Foundation, Inc., 59
#       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
#  ----------------------------------------------------------------------------


from openalea.plantgl.scenegraph import _pglsg as sg
from openalea.plantgl.math import Vector4, Vector3
from openalea.plantgl.all import Viewer, Scene, NurbsCurve
import math, copy


class NurbsMethod:
    class Patch:
        def __init__(self, curves, udegree=2, vdegree=2, **kargs):
            self.nPatch = sg.NurbsPatch(curves,udegree, vdegree)
        def getVSection(self, v):
            return self.nPatch.getVSection(v).ctrlPointList
        def getUSection(self, u):
            return self.nPatch.getUSection(u).ctrlPointList
        def real(self):
            return self.nPatch
    @staticmethod
    def make_interpolation(curves, *args, **kargs):
        curves  = map(sg.Point4Array, curves)
        return NurbsMethod.Patch(curves, *args, **kargs)
    @staticmethod
    def make_section(pts, **kargs):
        return NurbsCurve(pts, degree=len(pts)-1)


class CSplineMethod:
    class Patch:
        def __init__(self, curves, normalisedVDistances=None, normalisedUDistances=None, **kwargs):
            self._vs = [NurbsCurve.CSpline(pts, is_linear=False, distances=normalisedVDistances) for pts in curves]
            us = zip(*curves)
            self._us = [NurbsCurve.CSpline(pts, is_linear=False, distances=normalisedUDistances[i]) for i, pts in enumerate(us)]
        def getVSection(self, v):
            return [ x.getPointAt(v) for x in self._vs ]
        def getUSection(self, u):
            return [ x.getPointAt(u) for x in self._us ]
        def real(self):
            return self
    @staticmethod
    def make_interpolation(curves, *args, **kargs):
        curves  = map(sg.Point3Array,
                      ((Vector3(p[0], p[1], p[2]) for p in c) for c in curves))
        return CSplineMethod.Patch(curves, *args, **kargs)
    @staticmethod
    def make_section(pts, distances=None, **kargs):
        pts = map(lambda p: Vector3(p[0],p[1], p[2]), pts)
        return NurbsCurve.CSpline(pts, is_linear=False, distances=None)



class InterpolatedProfile( dict ):
    """An object that allows to describe a curve at
    different frames and to interpolate the curve
    between each key frame using NURBS interpolation.

    The interpolation is a nurbs patch that blends the different
    cross sections specified by the user.

    The patch is lazily rebuilt:
     - add_cross_section -> rebuilds the patch unless autoUpdate=False
     - add_cross_sections -> rebuilds the patch after inserting cross sections.

    Modification of cross sections control points does NOT refresh the patch.
    An explicit call to update_interpolation is necessary.

    Results of user interpolation requests (by using the call operator or
    get_interpolated_section are cached to speed up subsequent requests (starts to become
    significant for more that 10000 calls :) ).
    This cache is cleared when the patch is refreshed.

    Usage::

        crsSect1 = CrossSection((0., 0.), (0.5, 0.), (1., 0.))
        crsSect2 = CrossSection((0., 2.), (0.5, 1.), (1., 0.))
        crsSect3 = CrossSection((0., 0.), (0.5, 0.), (1., 0.))
        tc = InterpolatedProfile()
        tc.add_cross_sections(0.0, crsSect1,
                              0.5, crsSect2,
                              1.0, crsSect3)

    """
    __param_idx = 2 #the index of the parameter(time, ...) in Vector4 is Z

    def __init__(self, interpolator=NurbsMethod):
        dict.__init__(self)
        self.Method            = interpolator
        self.__updtCbk         = []
        self.__patch           = None
        self.__param_range     = [0.0,1.0]
        self.__abscissa_range  = [0.0,1.0]
        # -- if different from None used to check parameter equality :
        self.__epsilon         = None
        self.__cachedSections  = {}
        self.__normToReal      = {}
        self.__sortedTuples    = []
        self.update_interpolation()

    @staticmethod
    def make_default():
        interpolator = InterpolatedProfile()
        interpolator.add_cross_sections(0.0, CrossSection((0.0,0.0),(1.0,0.0)),
                                        1.0, CrossSection((0.0,0.0),(1.0,0.0)))
        return interpolator

    def real(self):
        real = dict((self.__normToReal[k],v) for k,v in self.iteritems())
        return real

    def as_sorted_tuples(self):
        return self.__sortedTuples

    ############
    # Dict API #
    ############
    def __setitem__(self, parameter, value):
        if isinstance(value, (int, float)): #move curve at position parameter to position value
            if parameter not in self:
                return
            crv = self[parameter]
            dict.__delitem__(self, self.normalised_parameter(parameter))
            self[value] = crv
        else: #parameter is the position and value is the curve
            self.add_cross_section(parameter, value)

    def __getitem__(self, parameter):
        # -- normalize parameter --
        parameter = self.normalised_parameter(parameter)
        epsParam  = self.__epsilonised_parameter(self, parameter)
        pts = dict.get(self, epsParam)
        if pts is not None:
            return pts
        else:
            raise Exception("No user-defined cross section at %s." % (str(parameter),)+\
                            "Use the call operator.")

    def __delitem__(self, parameter):
        if parameter in self.__param_range:
            return
        parameter = self.normalised_parameter(parameter)
        parameter = self.__epsilonised_parameter(self, parameter)
        dict.__delitem__(self, parameter)
        self.update_interpolation()
        self.__notify()

    def __contains__(self, parameter):
        # -- normalize parameter --
        if parameter is None:
            return False
        parameter = self.normalised_parameter(parameter)
        k = self.__epsilonised_parameter(self, parameter)
        return False if k is None else True

    ###########
    # Queries #
    ###########
    def check_control_point_count(self, section):
        """Tests if section is compatible in number of
        control points with this profile."""
        sectionLen = len(section)
        if len(self)>0:
            count = len( self.itervalues().next() )
            diff = sectionLen - count
            return diff
        return 0

    def update_interpolation(self):
        """ Discard previous interpolation a recompute a new
        one from the list of cross sections.
        """
        numSections = len(self)
        if numSections>=2:
            # -- clear cached sections --
            self.__cachedSections.clear()
            self.__normToReal = dict( (k, self.unnormalised_parameter(k)) for k in self.iterkeys() )

            # -- make a list of tuples out of the items,
            self.__sortedTuples = map(list, self.iteritems())
            # -- sort according to key,
            self.__sortedTuples.sort()
            curves  = map(lambda x: x[1], self.__sortedTuples)
            unorm = self._get_u_distances
            normalisedUDistances=[unorm(c) for c in curves]
            # -- and transpose since we are specifying rows, not columns
            curveLen = len( curves[0] )
            curves  = zip(*curves)
            # -- create point arrays
            curvesAr  = map(sg.Point4Array, curves)

            vsdiffs  = self._get_v_distances()
            patch  = self.Method.make_interpolation(curvesAr,
                                                    udegree = curveLen-1,
                                                    vdegree=numSections-1,
                                                    normalisedUDistances=normalisedUDistances,
                                                    normalisedVDistances=vsdiffs)
            self.__patch = patch
            self.__notify()

    ###########
    # Getters #
    ###########
    @property
    def interpolating_surface(self):
        """Returns the NurbsPatch used for interpolation"""
        return self.__patch.real()

    def get_interpolated_section(self, parameter, curve=True):
        """Returns a Curve corresponding to a V section
        of the patch at v=parameter. If curve=False, returns
        the control point list."""
        if self.__patch:
            # -- normalize parameter --
            parameter = self.normalised_parameter(parameter)
            epsParam  = self.__epsilonised_parameter(self.__cachedSections, parameter)
            ptList    = [] #TODO: put sensible defaults maybe?
            if epsParam is not None:
                ptList = self.__cachedSections[epsParam]
            else:
                ptList = self.__patch.getVSection(parameter)
                self.__cachedSections[parameter] = ptList

            return self.Method.make_section(ptList) if curve else ptList

    def get_param_range(self):
        return self.__param_range

    def get_abscissa_range(self):
        return self.__abscissa_range

    def _get_u_distances(self, crv):
        norm = self.normalised_abscissa
        if len(crv) < 2:
            return []
        diffPairs = zip(crv[1:], crv[:-1])
        return map(norm, map(sum, ( (x[0],-y[0]) for x,y in diffPairs)))

    def _get_v_distances(self):
        steps = self.as_sorted_tuples()
        if len(steps) < 2:
            return []
        diffPairs = zip(steps[1:], steps[:-1])
        return map(sum, ( (x[0],-y[0]) for x,y in diffPairs))

    #################
    # Functor style #
    #################
    __call__ = get_interpolated_section

    ###########
    # Setters #
    ###########
    def add_cross_section(self, parameter, section, autoUpdate=True, normalised=False):
        assert parameter>=self.__param_range[0] and parameter<=self.__param_range[1]

        section = copy.deepcopy(section)
        if self.check_control_point_count(section) != 0:
            raise Exception("Wrong control point count")

        # -- normalise parameter if not already normalised--
        nparameter = parameter #or else not defined
        if not normalised:
            nparameter = self.normalised_parameter(parameter)
        else:
            nparameter = parameter #or else not defined
        # -- if collision with existing cuve, replace it --
        epsParam = self.__epsilonised_parameter(self, nparameter)
        if epsParam is not None:
            nparameter = epsParam
        dict.__setitem__(self, nparameter, section)
        # -- fix nparameter, aka time, angle... --
        for point in section:
            point[InterpolatedProfile.__param_idx] = nparameter
        # -- refresh the model --
        if autoUpdate:
            self.update_interpolation()

    def add_cross_sections(self, *parameters_and_sections, **kargs):
        for i in xrange(0,len(parameters_and_sections),2):
            t, s = parameters_and_sections[i], parameters_and_sections[i+1]
            self.add_cross_section(t, s,
                                   autoUpdate=False, normalised=kargs.get("normalised"))
        self.update_interpolation()

    def set_param_range(self, min, max):
        self.__param_range = [min, max]

    def set_abscissa_range(self, min, max):
        self.__abscissa_range = [min, max]

    def add_update_callback(self, callback):
        self.__updtCbk.append(callback)

    #############
    # Modifiers #
    #############
    def create_control_point(self, abscissa):
        """Inserts an interpolated control point
        to every defining section at position = abscissa.

        The abscissa is expressed in the get_abscissa_range() range.
        """
        abscissa = self.normalised_abscissa(abscissa)
        if abscissa == 1.0 or abscissa == 0.0:
            return
        if self.__patch:
            rows = self.itervalues()
            afterId  = InterpolatedProfile.__find_index_in_polyline(abscissa,
                                                                    self.itervalues().next(),
                                                                    is_abscissa=True)
            beforeId = afterId-1
            ptPairs = [(c[afterId], c[beforeId]) for c in rows]

            ptList = [ Vector4(abscissa, self.__lin_interpolation(abscissa, before, after), after[2], 1) \
                       for after, before in ptPairs ]
            self.insert_column_before(afterId, ptList)

    def insert_column_before(self, index, controlPoints):
        """Inserts a control point from controlPoints
        to every defining cross section before
        column given by index and recomputes the iterpolation.
        The number of items in controlPoints must be the same as
        the number of defining sectons (len(self)).
        """
        assert len(controlPoints) == len(self)
        if self.__patch:
            newSections = []
            for lineCount, (t, xsection) in enumerate(self.iteritems()):
                xsection.insert(index, controlPoints[lineCount])
                newSections += [t,xsection]
            self.add_cross_sections(*newSections, normalised=True)

    def create_cross_section(self, parameter):
        """Adds a interpolated cross section to the set of cross
        sections that define the NURBS patch. The parameter is expressed
        in the get_param_range() range.

        The added cross section can be retreived
        with __getitem__(parameter).
        """
        if self.__patch:
            nparameter = self.normalised_parameter(parameter)
            if dict.__contains__(self, nparameter):
                return
            columns = zip(*self.itervalues())
            afterId  = InterpolatedProfile.__find_index_in_polyline(nparameter,
                                                                    iter(columns).next(),
                                                                    is_abscissa=False)
            beforeId = afterId-1
            ptPairs = ((c[afterId], c[beforeId]) for c in columns)
            section = CrossSection(*[ (after[0], self.__lin_interpolation(nparameter, before,
                                                                          after, xid=2)) \
                                      for after, before in ptPairs ])
            self.add_cross_section(parameter, section)

    def __lin_interpolation(self, normAbs, p1, p2, xid=0, yid=1):
        Xb, Yb = p2[xid], p2[yid]
        Xa, Ya = p1[xid], p1[yid]
        assert Xb != Xa
        return ((Yb-Ya)/(Xb-Xa))*(normAbs-Xb) + Yb

    #############################
    # Private methods come next #
    #############################
    @staticmethod
    def __line_length(line, xid=0, yid=1):
        """ Computes the cartesian length of a sequence of 2D control points
        as if it was a polyline.

        Silliness alert : we compute the length of a line
        by considering it as a polyline, not nurbs: I don't kown how to
        do that, though I'm not sure it's useful.
        """
        if len(line)>0:
            # l = sg.Polyline( sg.Point3Array( Vector3(i[xid], i[yid], 0.) for i in line ) )
            # return len(l)
            return sum( math.sqrt( (b[xid]-a[xid])**2 + (b[yid]-a[yid])**2 ) \
                        for a, b in zip(line[:-1], line[1:]) )
        return 0.0

    @staticmethod
    def __find_index_in_polyline(value, line, is_abscissa=True):
        """Tries to locate the index of the control point on line
        before which abscissa (expressed between get_param_range()) is located
        in line space and returns that index. Ugh? How clear is that?
        """
        # are we looking for row indices or column indices?
        xid, yid = (0,1) if is_abscissa else (2,1)
        l = InterpolatedProfile.__line_length(line, xid=xid, yid=yid)
        if l == 0.0:
            raise Exception("I'm too tired to know what I should do here")
        lineItemCount = len(line)
        for i in xrange(1, lineItemCount):
            res = InterpolatedProfile.__line_length(line[:i+1],xid,yid)/l
            if res > value:
                return i
        return lineItemCount

    @staticmethod
    def __find_column_for_abscissa(abscissa, line):
        """Tries to locate the index of the control point on line
        before which abscissa (expressed between get_param_range()) is located
        in line space and returns that index. Ugh? How clear is that?
        """
        l = InterpolatedProfile.__line_length(line)
        if l == 0.0:
            raise Exception("I'm too tired to know what I should do here")
            return 0 #?

        lineItemCount = len(line)
        for i in xrange(1, lineItemCount):
            res = InterpolatedProfile.__line_length(line[:i+1])/l
            if res > abscissa:
                return i
        return lineItemCount

    def normalised_parameter(self, parameter, is_distance=False):
        """Converts a parameter expressed in a range (set_param_range),
        to the [0,1] interval used as v by NURBS."""
        min_, max_ = self.__param_range
        return InterpolatedProfile.normalise(parameter, min_, max_, is_distance)

    def normalised_abscissa(self, abscissa, is_distance=False):
        """Converts an abscissa expressed in a range (set_abscissa_range),
        to the [0,1] interval used as u by NURBS."""
        min_, max_ = self.__abscissa_range
        return InterpolatedProfile.normalise(abscissa, min_, max_, is_distance)

    def unnormalised_parameter(self, parameter, is_distance=False):
        """Converts a parameter expressed in [0,1],
        to the get_param_range interval."""
        min_, max_ = self.__param_range
        return InterpolatedProfile.unnormalise(parameter, min_, max_, is_distance)

    def unnormalised_abscissa(self, abscissa, is_distance=False):
        """Converts a parameter expressed in [0,1],
        to the get_abscissa_range interval."""
        min_, max_ = self.__abscissa_range
        return InterpolatedProfile.unnormalise(abscissa, min_, max_, is_distance)

    @staticmethod
    def normalise(p, min, max, is_distance=False):
        if not is_distance:
            return (p - min)/float(max-min)
        else:
            return (p)/float(max-min)

    @staticmethod
    def unnormalise(pn, min, max, is_distance=False):
        if not is_distance:
            return pn*max - min*(pn-1)
        else:
            return pn*max - min*pn

    def __epsilonised_parameter(self, mapping, key):
        """Returns 'key' or something close to it depending on epsilon.
        If an epsilon has been defined it returns the first curve key that
        matches abs(curve_key-key)<epsilon.
        If epsilon is None and the key is already
        set it returns key.
        If epsilon is None and the key does not exist, returns None
        """
        if self.__epsilon:
            for k in mapping.iterkeys():
                if abs(key-k)<=self.__epsilon:
                    return k #return the rounded key :)
            return None
        elif key in mapping.keys():
            return key
        return None

    def __notify(self):
        for c in self.__updtCbk:
            c()



class CrossSection( list ):
    def __init__(self, *pointTuples):
        list.__init__(self, (Vector4(x[0], x[1], -1., 1) for x in pointTuples))

#ALIAS
TemporalCurve=InterpolatedProfile


def slice_patch(scene, tc, slices):
    steps = [i/float(slices-1) for i in range(slices)]
    for i in steps:
        curve      = tc(i)
        scene     += curve

if __name__ == "__main__":
    crsSect1 = CrossSection((0., 0.), (0.5, 0.), (1., 0.))
    crsSect2 = CrossSection((0., 2.), (0.5, 1.), (1., 0.))
    crsSect3 = CrossSection((0., 0.), (0.5, 0.), (1., 0.))
    crsSect4 = CrossSection((0., 2.), (0.5, 0.), (1., 0.))
    crsSect5 = CrossSection((0., 2.), (0.5, 0.), (0.6, 0.3), (1., 0.))

    tc = InterpolatedProfile()
    tc.add_cross_sections(0.0, crsSect1,
                          0.5, crsSect2,
                          1.0, crsSect3)

    from PyQt4 import QtGui
    app=QtGui.QApplication([])

    scene = Scene()
    Viewer.display(scene)


    scene += tc.interpolating_surface
    tc.create_control_point(0.25)
    scene += tc.interpolating_surface
    tc.create_cross_section(0.75)
    scene += tc.interpolating_surface

    Viewer.update()
    # slice_patch(scene, tc, slices = 11)

    #the following will add a new patch to the view, not update the existing one.
    #################################################################################
    # tc.create_control_point(0.25);scene+=tc.interpolating_surface;Viewer.update() #
    # tc.create_cross_section(0.75);scene+=tc.interpolating_surface;Viewer.update() #
    #################################################################################
