# -*-python-*-
#--------------------------------------------------------------------------------
#
#       OpenAlea.SConsX: SCons extension package for building platform
#                        independant packages.
#
#       Copyright 2006 INRIA - CIRAD - INRA  
#
#       File author(s): Christophe Pradal <christophe.prada@cirad.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
#--------------------------------------------------------------------------------

__doc__ = """ CGAL configure environment. """
__license__ = "Cecill-C"
__revision__ = "$Id:  $"

import os, sys
from openalea.sconsx.config import *


class CGAL:
   def __init__(self, config):
      self.name = 'cgal'
      self.config = config
      self._default = {}


   def depends(self):
      deps = ['gmp']

      return deps


   def default(self):

      #self._default['libs_suffix'] = '$compiler_libs_suffix'

      if isinstance(platform, Win32):
      
         self._default['flags'] = ''
         self._default['defines'] = ''

         try:
            cgalroot = os.environ['CGALROOT']
            self._default['include'] = [pj(cgalroot,'include'), pj(cgalroot,'include','CGAL','config','msvc')]
            self._default['libpath'] = pj(cgalroot,'lib')
            self._default['libs'] = 'CGAL-vc80-mt'            
         except:
            try:
               import openalea.config as conf
               self._default['include'] = conf.include_dir
               self._default['libpath'] = conf.lib_dir
            except ImportError, e:
               self._default['include'] = 'C:' + os.sep
               self._default['libpath'] = 'C:' + os.sep
               
            self._default['libs'] = 'CGAL'

      elif isinstance(platform, Posix):
         self._default['include'] = '/usr/include'
         self._default['libpath'] = '/usr/lib'
         self._default['libs'] = 'CGAL'
         self._default['flags'] = ''
         self._default['defines'] = ''


   def option( self, opts):

      self.default()

      opts.AddOptions(('cgal_includes', 
                     'CGAL include files', 
                     self._default['include']),

         PathOption('cgal_libpath', 
                     'CGAL libraries path', 
                     self._default['libpath']),

         ('cgal_libs', 
           'CGAL libraries', 
           self._default['libs']),
           
         ('cgal_flags', 
           'CGAL compiler flags', 
           self._default['flags']),

         ('cgal_defines', 
           'CGAL defines', 
           self._default['defines']),

         BoolOption('WITH_CGAL', 
           'Specify whether you want to compile your project with CGAL', True)
     )


   def update(self, env):
      """ Update the environment with specific flags """
      cgal_inc = env['cgal_includes']
      if type(cgal_inc) == str:
        cgal_inc = cgal_inc.split()
      cgal_inc = cgal_inc[0]
      if not os.path.exists(os.path.join(cgal_inc,'CGAL')):
        import warnings
        warnings.warn("Error: CGAL headers not found. CGAL disabled ...")
        env['WITH_CGAL'] = False      
      if env['WITH_CGAL']:
        env.AppendUnique(CPPPATH=[env['cgal_includes']])
        env.AppendUnique(LIBPATH=[env['cgal_libpath']])
        env.Append(CPPDEFINES='$cgal_defines')
        env.Append(CPPDEFINES='WITH_CGAL')
        env.Append(CPPFLAGS='$cgal_flags')

        env.AppendUnique(LIBS=env['cgal_libs'])


   def configure(self, config):
      if not config.conf.CheckCXXHeader('CGAL/version.h'):
        print "Error: CGAL headers not found."
        exit()
        
         


def create(config):
   " Create cgal tool "
   cgal = CGAL(config)

   deps= cgal.depends()
   for lib in deps:
      config.add_tool(lib)

   return cgal

