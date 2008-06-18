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

__doc__ = """ GMP configure environment. """
__license__ = "Cecill-C"
__revision__ = "$Id:  $"

import os, sys
from openalea.sconsx.config import *


class GMP:
   def __init__(self, config):
      self.name = 'gmp'
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
            self._default['libs'] = 'gmp-vc80-mt'            
         except:
            try:
               import openalea.config as conf
               self._default['include'] = conf.include_dir
               self._default['libpath'] = conf.lib_dir
            except ImportError, e:
               self._default['include'] = 'C:' + os.sep
               self._default['libpath'] = 'C:' + os.sep
               
            self._default['libs'] = 'gmp'

      elif isinstance(platform, Posix):
         self._default['include'] = '/usr/include'
         self._default['libpath'] = '/usr/lib'
         self._default['libs'] = 'gmp'
         self._default['flags'] = ''
         self._default['defines'] = ''


   def option( self, opts):

      self.default()

      opts.AddOptions(PathOption('gmp_includes', 
                     'GMP include files', 
                     self._default['include']),

         PathOption('gmp_libpath', 
                     'GMP libraries path', 
                     self._default['libpath']),

         ('gmp_libs', 
           'GMP libraries', 
           self._default['libs']),
           
         ('gmp_flags', 
           'GMP compiler flags', 
           self._default['flags']),

         ('gmp_defines', 
           'GMP defines', 
           self._default['defines']),

         BoolOption('WITH_GMP', 
           'Specify whether you want to compile your project with GMP', True)
     )


   def update(self, env):
      """ Update the environment with specific flags """
      if env['WITH_GMP']:
        env.AppendUnique(CPPPATH=[env['gmp_includes']])
        env.AppendUnique(LIBPATH=[env['gmp_libpath']])
        env.Append(CPPDEFINES='$gmp_defines')
        env.Append(CPPDEFINES='WITH_GMP')
        env.Append(CPPFLAGS='$gmp_flags')

        env.AppendUnique(LIBS=env['gmp_libs'])


   def configure(self, config):
      if not config.conf.CheckCXXHeader('gmp.h'):
        print "Error: GMP headers not found."
        exit()
        
         


def create(config):
   " Create gmp tool "
   gmp = GMP(config)

   deps= gmp.depends()
   for lib in deps:
      config.add_tool(lib)

   return gmp

