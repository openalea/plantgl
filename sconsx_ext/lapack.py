# -*-python-*-
#--------------------------------------------------------------------------------
#
#       OpenAlea.SConsX: SCons extension package for building platform
#                        independant packages.
#
#       Copyright 2006-2012 INRIA - CIRAD - INRA  
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

__doc__ = """ LAPACK configure environment. """
__license__ = "Cecill-C"
__revision__ = "$Id: lapack.py 8407 2010-03-08 07:53:28Z pradal $"

import os, sys
from openalea.sconsx.config import *


class LAPACK:
   def __init__(self, config):
      self.name = 'lapack'
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
            self._default['include'] = pj(cgalroot,'auxiliary','taucs','include')
            self._default['libpath'] = pj(cgalroot,'auxiliary','taucs','lib')
            self._default['libs'] = 'lapack'            
         except:
            try:
               import openalea.config as conf
               self._default['include'] = conf.include_dir
               self._default['libpath'] = conf.lib_dir
            except ImportError, e:
               self._default['include'] = 'C:' + os.sep
               self._default['libpath'] = 'C:' + os.sep
               
            self._default['libs'] = 'lapack'

      elif isinstance(platform, Posix):
         self._default['include'] = '/usr/include'
         self._default['libpath'] = '/usr/lib'
         self._default['libs'] = ['lapack','blas']
         self._default['flags'] = ''
         self._default['defines'] = ['LAPACK_USE_F2C','CGAL_USE_F2C','BLAS_USE_F2C']


   def option( self, opts):

      self.default()

      opts.AddVariables(PathVariable('lapack_includes', 
                     'LAPACK include files', 
                     self._default['include']),

         PathVariable('lapack_libpath', 
                     'LAPACK libraries path', 
                     self._default['libpath']),

         ('lapack_libs', 
           'LAPACK libraries', 
           self._default['libs']),
           
         ('lapack_flags', 
           'LAPACK compiler flags', 
           self._default['flags']),

         ('lapack_defines', 
           'LAPACK defines', 
           self._default['defines']),

         BoolVariable('WITH_LAPACK', 
           'Specify whether you want to compile your project with LAPACK', True)
     )


   def update(self, env):
      """ Update the environment with specific flags """
      if env['WITH_LAPACK'] :
        lapack_lib = env['lapack_libpath']
        if type(lapack_lib) == str:
          lapack_lib = lapack_lib.split()
        lapack_lib = lapack_lib[0]
        libnames = ['liblapack.so','liblapack.a', 'lapack.lib', 'liblapack.lib', 'liblapack.dylib']
        if sum( map(lambda x: os.path.exists(os.path.join(lapack_lib,x)),libnames) ) == 0 :
          import warnings
          warnings.warn("Error: LAPACK lib not found. LAPACK disabled ...")
          env['WITH_LAPACK'] = False      
      if env['WITH_LAPACK']:
        env.AppendUnique(CPPPATH=[env['lapack_includes']])
        env.AppendUnique(LIBPATH=[env['lapack_libpath']])
        for define in env['lapack_defines']:
            env.AppendUnique(CPPDEFINES=define)
        env.Append(CPPDEFINES='WITH_LAPACK')
        env.Append(CPPFLAGS='$lapack_flags')

        env.AppendUnique(LIBS=env['lapack_libs'])


   def configure(self, config):
      if not config.conf.CheckCXXHeader('lapack.h'):
        print "Error: LAPACK headers not found."
        exit()
        
         


def create(config):
   " Create lapack tool "
   
   try:
        tool = LAPACK(config)
        
        deps= tool.depends()
        for lib in deps:
                config.add_tool(lib)
        
        return tool
   except:
       print "Error creating LAPACK Tool"
       raise Exception("Error in Tool Creation")

