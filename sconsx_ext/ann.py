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

__doc__ = """ ANN configure environment. """
__license__ = "Cecill-C"
__revision__ = "$Id: ann.py 8407 2010-03-08 07:53:28Z pradal $"

import os, sys
from openalea.sconsx.config import *


class ANN:
   def __init__(self, config):
      self.name = 'ann'
      self.config = config
      self._default = {}


   def depends(self):
      return []


   def default(self):

      if isinstance(platform, Win32):
      
         self._default['flags'] = ''
         self._default['defines'] = ''

         try:
            annroot = os.environ['ANN_DIR']
            self._default['include'] = pj(annroot,'include')
            self._default['libpath'] = pj(annroot,'lib')
            self._default['libs'] = 'ANN'            
         except:
            try:
               import openalea.config as conf
               self._default['include'] = conf.include_dir
               self._default['libpath'] = conf.lib_dir
            except ImportError, e:
               self._default['include'] = 'C:' + os.sep
               self._default['libpath'] = 'C:' + os.sep
               
            self._default['libs'] = 'ANN'

      elif isinstance(platform, Posix):
         self._default['include'] = '/usr/include'
         self._default['libpath'] = '/usr/lib'
         self._default['libs'] = 'ann'
         self._default['flags'] = ''
         self._default['defines'] = ''


   def option( self, opts):

      self.default()

      opts.AddVariables(PathVariable('ann_includes', 
                     'ANN include files', 
                     self._default['include']),

         PathVariable('ann_libpath', 
                     'ANN libraries path', 
                     self._default['libpath']),

         ('ann_libs', 
           'ANN libraries', 
           self._default['libs']),
           
         ('ann_flags', 
           'ANN compiler flags', 
           self._default['flags']),

         ('ann_defines', 
           'ANN defines', 
           self._default['defines']),

         BoolVariable('WITH_ANN', 
           'Specify whether you want to compile your project with ANN', True)
     )


   def update(self, env):
      """ Update the environment with specific flags """
      if env['WITH_ANN'] :
        ann_inc = env['ann_includes']
        ann_header = os.path.join(ann_inc,'ANN','ANN.h')
        if not os.path.exists(ann_header):
          import warnings
          warnings.warn("Error: ANN headers not found. ANN disabled ...")
          env['WITH_ANN'] = False      
      if env['WITH_ANN']:
        env.AppendUnique(CPPPATH=[env['ann_includes']])
        env.AppendUnique(LIBPATH=[env['ann_libpath']])
        env.Append(CPPDEFINES='$ann_defines')
        env.Append(CPPDEFINES='WITH_ANN')
        env.Append(CPPFLAGS='$ann_flags')

        env.AppendUnique(LIBS=env['ann_libs'])


   def configure(self, config):
      if not config.conf.CheckCXXHeader('ANN/ANN.h'):
        print "Error: ANN headers not found."
        exit()
        
         


def create(config):
   " Create mpfr tool "
   
   try:
        tool = ANN(config)
        
        deps= tool.depends()
        for lib in deps:
                config.add_tool(lib)
        
        return tool
   except:
       print "Error creating ANN Tool"
       raise Exception("Error in Tool Creation")

