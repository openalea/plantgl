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


###########################################################
# ----------------------- ADDONS -----------------------  #
###########################################################
from OpenGL.GL import *
from math import pow,log, exp, e
import weakref
from PyQt4 import QtCore



class EditorAddOn(object):
    __specials__ = {"recomputeDelay" : lambda x: x._reconfigure_timer()}
    __gl_pushed_attribs__ = 0
    __hidden_vars__ = []

    def __init__(self, editor):
        assert editor is not None
        editor._addons.append(self)
        self._profile     = None
        self._callbacks        = {}
        self._recomputeTimout  = QtCore.QTimer()
        self._recomputeTimout.timeout.connect(self._differed_compute)
        self._editor            = weakref.proxy(editor)
        self.enabled           = False
        self.recomputeDelay    = 0
        self.init()

    def init(self, renderer):
        raise NotImplementedError

    def name(self):
        return self.__class__.__name__[:-5]

    def variable_dict(self):
        return dict( (k, (type(v), v)) for k, v, in self.__dict__.iteritems() if \
                     not k.startswith("_") )

    def __nonzero__(self):
        return self.enabled

    def add_callback(self, key, call):
        self._callbacks.setdefault(key, []).append(call)

    def set(self, k, v):
        object.__setattr__(self, k, v)
        if k in self.__specials__:
            actions = self.__specials__[k]
            if not hasattr(actions, "__getitem__"):
                actions(self)
            else:
                for act in actions:
                    act(self)
        self._editor.update_displayed_geometry()
        if k in self._callbacks:
            calls = self._callbacks[k]
            path = self.name()+"."+k
            for c in calls:
                c(path, v)

    #################################################
    # Calls used by the editor, not to be overriden #
    #################################################
    def _draw(self, renderer):
        if not self.enabled:
            return
        glPushAttrib(GL_CURRENT_BIT|GL_ENABLE_BIT|self.__gl_pushed_attribs__)
        self.draw(renderer)
        glPopAttrib(GL_CURRENT_BIT|GL_ENABLE_BIT|self.__gl_pushed_attribs__)

    def _compute(self, profile):
        if not self.enabled:
            return
        self._profile = profile
        if self.recomputeDelay == 0:
            self.compute()
        else:
            if self._recomputeTimout.isActive():
                self._recomputeTimout.stop()
            self._recomputeTimout.start()

    def _differed_compute(self):
        self._recomputeTimout.stop()
        self.compute()
        self._editor.update()

    def _init_from_profile(self, profile):
        object.__setattr__(self, "_profile", profile)
        self.init_from_profile()

    def _interpolation_changed(self):
        self.interpolation_changed()

    def _reconfigure_timer(self):
        self._recomputeTimout.setInterval(self.recomputeDelay)
    ##################
    # Override these #
    ##################
    def draw(self, renderer):
        pass

    def compute(self):
        pass

    def clear_caches(self):
        pass

    def init_from_profile(self):
        pass

    def interpolation_changed(self):
        pass


class UserSlicesAddOn(EditorAddOn):
    def init(self):
        self.__userSlices = {}
        self.highlightEps = 1e-3 #normalised

    def draw(self, renderer):
        glDisable(GL_LINE_SMOOTH)
        glLineWidth(2)
        cursor = self._editor.position()
        pos = self._editor.normalised_parameter(cursor)
        for k, s in self.__userSlices.iteritems():
            if abs(k - pos) < self.highlightEps:
                glColor4f(1., 0.0, 0.0, 1.0)
            else:
                glColor4f(0.9, 0.9, 1.0, 0.5)
            s.apply(renderer)

    def compute(self):
        # -- recompute the user slices --
        profile = self._profile
        for k, v in profile.iteritems():
            self.__userSlices[k] = profile.Method.make_section(v)

    def clear_caches(self):
        self.__userSlices.clear()

    def init_from_profile(self):
        self.highlightEps = self._editor.normalised_parameter(self._editor.increment,
                                                              is_distance=True)



class VisualCrossSectionsAddOn(EditorAddOn):


    __specials__ = EditorAddOn.__specials__.copy()
    __specials__["rate"] = lambda x:x.clear_caches()
    __specials__["width"] = lambda x:x.fix_width()

    @staticmethod
    def linIntensityF(x, range):
        assert x>=-1 and x<=1
        return range*(-(abs(x))+1)

    @staticmethod
    def gauIntensityF(x, range):
        b = 0
        c = (1/e)
        return range*exp(-((x-b)**2)/(c**2))

    def init(self):
        self.width     = 0.15 #normalised parameter
        self.rate      = 0.05 #real parameter
        self._sections  = {}
        self.baseAlpha = 0.2
        self.infColor  = (0.,1.,1.)
        self.supColor  = (0.,1.,1.)

    def draw(self, renderer):
        glLineWidth(0.5)

        position = self._editor.position()
        _min, _max = self._editor.range()
        diff     = float(_max-_min)
        if diff == 0:
            return
        alphaRange = 1.0-self.baseAlpha

        for p, s in self._sections.iteritems():
            delta = self._editor.normalised_parameter(p-position, is_distance=True)
            alpha = self.baseAlpha
            if abs(delta) < self.width:
                if delta == 0.0:
                    alpha = 1.0
                else:
                    d= delta/self.width
                    alphaBoost = VisualCrossSectionsAddOn.gauIntensityF(d,alphaRange)
                    alpha += alphaBoost
            if delta >=0:
                r,g,b = self.supColor
            else:
                r,g,b = self.infColor
            glColor4f(r, g, b, alpha)
            s.apply(renderer)


    def compute(self):
        profile = self._profile
        if profile and len(self._sections)==0:
            min_, max_ = self._editor.range()
            samples = int((max_-min_)/self.rate)
            for i in range(1, samples):
                par = min_+i*self.rate
                crv = profile(par)
                self._sections[par] =  crv

    def clear_caches(self):
        self._sections.clear()

    def init_from_profile(self):
        min_, max_ = self._editor.range()
        self.rate  = self._editor.increment*2
        self.width = self._editor.normalised_parameter( (max_-min_)/4,
                                                       is_distance=True)

    def fix_width(self):
        self.width = min(max(self.width, 1e-4), 1.0) #prevent zero division


class GridAddOn(EditorAddOn):

    __specials__ = {"gridStep": lambda x:x.grid_step_changed(),
                    "gridCount": lambda x:x.grid_count_changed()}
    __gl_pushed_attribs__ = GL_LINE_BIT

    __max_grid_levels__ = 4

    def init(self):
        self.xyGrid    = False
        self.xzGrid    = False
        self.yzGrid    = False
        self.axes      = True

        self.xScale    = False
        self.yScale    = False
        self.zScale    = False
        self.gridStep  = 1.0 #normalised
        self.gridCount = 1
        self.minimum   = 0.
        self.maximum   = 1.
        self._range     = 1.0

    def draw(self, renderer):
        glLineWidth(1)

        self._draw_grids()
        self._draw_axis()

    def _draw_grids(self):
        rate = self.gridStep
        cameraVec = self._editor.camera().viewDirection()
        levels = 1
        m0 = abs(glGetIntegerv(GL_PROJECTION_MATRIX)[0][0])
        levels = min(levels+int(log(m0+1,10)), GridAddOn.__max_grid_levels__)

        for l in range(levels, 0, -1): #multilevel grid
            #draw scales for this level?
            #doScale = self.scales #l==levels and self.scales

            # -- find the line width --
            lw = float(l)
            glLineWidth(lw)

            # -- level has a stepping that is gridStep*10**-(level-1)
            # -- and number of lines is function of this stepping
            levelStep  = rate/(10**(levels-l))
            levelCount = int(round((1+levelStep)/levelStep))

            for i in range(levelCount):
                pos = levelStep*i

                x, xp = (0., 1.) if cameraVec[0] < 0 else (1., 0.)
                y, yp = (0., 1.) if cameraVec[1] < 0 else (1., 0.)
                z, zp = (0., 1.) if cameraVec[2] < 0 else (1., 0.)

                glDisable(GL_DEPTH_TEST)
                glColor4f(0.8,0.8,0.8,1.0)
                if self.xScale:
                    pass
                if self.yScale:
                    pass
                if self.zScale:
                    zstr = str(self.minimum+self._range*pos)
                    self._editor.renderText(x, 0., pos, zstr)
                    self._editor.renderText(xp, 0., pos, zstr)
                glEnable(GL_DEPTH_TEST)

                glColor4f(0.2,0.2,0.2,1.0)
                if self.xzGrid:
                    glBegin(GL_LINES)
                    glVertex3f(0., y, pos)
                    glVertex3f(1., y, pos)
                    glVertex3f(pos, y, 0.)
                    glVertex3f(pos, y, 1.)
                    glEnd()
                if self.xyGrid:
                    glBegin(GL_LINES)
                    glVertex3f(0., pos, z)
                    glVertex3f(1., pos, z)
                    glVertex3f(pos, 0., z)
                    glVertex3f(pos, 1., z)
                    glEnd()
                if self.yzGrid:
                    glBegin(GL_LINES)
                    glVertex3f(x, 0., pos)
                    glVertex3f(x, 1., pos)
                    glVertex3f(x, pos, 0.)
                    glVertex3f(x, pos, 1.)
                    glEnd()

    def _draw_axis(self):
        glDisable(GL_DEPTH_TEST)
        if self.axes:
            glLineWidth(3)

            # x axis
            glColor4f(1.0,0.,0.,1.)
            self._editor.renderText(0.2,0.,0.,"x")
            glBegin(GL_LINES)
            glVertex3f(0.,  0., 0)
            glVertex3f(0.2, 0., 0)
            glEnd()

            # y axis
            glColor4f(0.,1.0,0.,1.)
            self._editor.renderText(0.,0.2,0.,"y")
            glBegin(GL_LINES)
            glVertex3f(0., 0.,  0)
            glVertex3f(0., 0.2, 0)
            glEnd()

            # z axis
            glColor4f(0.,0.,1.0,1.)
            self._editor.renderText(0.,0.,0.2,"z")
            glBegin(GL_LINES)
            glVertex3f(0., 0., 0.0)
            glVertex3f(0., 0., 0.2)
            glEnd()

        glEnable(GL_DEPTH_TEST)

    def grid_count_changed(self):
        min_, max_ = self._editor.range()
        diff = max(max_-min_, 1e-4) #prevent zero division down the line
        self.gridCount = max(self.gridCount, 2) #prevent zero division
        self.gridStep = float(self._editor.normalised_parameter( diff / (self.gridCount-1),
                                                                is_distance=True))

    def grid_step_changed(self):
        self.gridStep = max(1e-4, self.gridStep) #prevent zero division
        # note : range is one as grid step is in normalised space.
        self.gridCount  = int(round((1+self.gridStep) / self.gridStep))


    def init_from_profile(self):
        min_, max_ = self._editor.range()
        self.minimum = min_
        self.maximum = max_
        self._range     = max(max_ - min_, 1e-4) #prevent zero division down the line
        self.gridStep  = self._editor.normalised_parameter(self._range/ 10,
                                                          is_distance=True)
        self.gridCount = 11
