# -*-python-*-
#--------------------------------------------------------------------------------
#
#       OpenAlea.SConsX: SCons extension package for building platform
#                        independant packages.
#
#       Copyright 2006-2009 INRIA - CIRAD - INRA  
#
#       File author(s): Frederic Boudon
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
#--------------------------------------------------------------------------------

__doc__ = """ MPFR configure environment. """
__license__ = "Cecill-C"
__revision__ = "$Id$"

import os, sys
from openalea.sconsx.config import *
from os.path import join as pj

class MPFR:
   def __init__(self, config):
      self.name = 'mpfr'
      self.config = config
      self._default = {}


   def depends(self):
      return []


   def default(self):

      if isinstance(platform, Win32):
      
         self._default['flags'] = ''
         self._default['defines'] = ''

         try:
            cgalroot = os.environ['CGALROOT']
            self._default['include'] = pj(cgalroot,'auxiliary','gmp','include')
            self._default['libpath'] = pj(cgalroot,'auxiliary','gmp','lib')
            self._default['libs'] = 'libmpfr-4'            
         except:
            try:
               import openalea.config as conf
               self._default['include'] = conf.include_dir
               self._default['libpath'] = conf.lib_dir
            except ImportError, e:
               try:
                  import pkg_resources as pkg
                  egg_env = pkg.Environment()
                  mingw_base = egg_env["mingw"][0].location
                  self._default['include'] = pj(mingw_base, "include")
                  self._default['libpath'] = pj(mingw_base, "lib")
               except Exception, e:
                  self._default['include'] = 'C:' + os.sep
                  self._default['libpath'] = 'C:' + os.sep
               
            self._default['libs'] = 'mpfr'

      elif isinstance(platform, Posix):
         self._default['include'] = '/usr/include'
         self._default['libpath'] = '/usr/lib'
         self._default['libs'] = 'mpfr'
         self._default['flags'] = ''
         self._default['defines'] = ''


   def option( self, opts):

      self.default()

      opts.AddVariables(PathVariable('mpfr_includes', 
                     'MPFR include files', 
                     self._default['include']),

         PathVariable('mpfr_libpath', 
                     'MPFR libraries path', 
                     self._default['libpath']),

         ('mpfr_libs', 
           'MPFR libraries', 
           self._default['libs']),
           
         ('mpfr_flags', 
           'MPFR compiler flags', 
           self._default['flags']),

         ('mpfr_defines', 
           'MPFR defines', 
           self._default['defines']),

         BoolVariable('WITH_MPFR', 
           'Specify whether you want to compile your project with MPFR', True)
     )


   def update(self, env):
      """ Update the environment with specific flags """
      if env['WITH_MPFR'] :
        mpfr_inc = env['mpfr_includes']
        # if type(mpfr_inc) == str:
          # mpfr_inc = mpfr_inc.split()
        # mpfr_inc = mpfr_inc[0]
        print 'Check',os.path.join(mpfr_inc,'mpfr.h'),os.path.exists(os.path.join(mpfr_inc,'mpfr.h'))
        if not os.path.exists(os.path.join(mpfr_inc,'mpfr.h')):
          import warnings
          warnings.warn("Error: MPFR headers not found. MPFR disabled ...")
          env['WITH_MPFR'] = False      
      if env['WITH_MPFR']:
        env.AppendUnique(CPPPATH=[env['mpfr_includes']])
        env.AppendUnique(LIBPATH=[env['mpfr_libpath']])
        env.Append(CPPDEFINES='$mpfr_defines')
        #env.Append(CPPDEFINES='WITH_MPFR')
        env.Append(CPPFLAGS='$mpfr_flags')

        env.AppendUnique(LIBS=env['mpfr_libs'])


   def configure(self, config):
      if not config.conf.CheckCXXHeader('mpfr.h'):
        print "Error: MPFR headers not found."
        exit()
        
         


def create(config):
   " Create mpfr tool "
   
   try:
        tool = MPFR(config)
        
        deps= tool.depends()
        for lib in deps:
                config.add_tool(lib)
        
        return tool
   except:
       print "Error creating MPFR Tool"
       raise Exception("Error in Tool Creation")

