# -*- coding: utf-8 -*-
__revision__ = "$Id$"

import os, sys
from setuptools import setup, find_packages
from openalea.sconsx.util.env_check import is_conda
from versionmanager import getPGLVersion, getMetaInfo
pj = os.path.join


globals().update(getMetaInfo())

# Setup script
version = getPGLVersion().to_string()
print (pkg_name+' : version = '+version)

# Scons build directory
build_prefix= "build-scons"

install_requires = []
setup_requires = []

pylint_dir = os.path.join('src', 'plantgl')

# Main setup
setup(
    name=name,
    version=version,
    description=description,
    long_description=long_description,
    author=authors,
    author_email=authors_email,
    url=url,
    license=license,

    # Define what to execute with scons
    # scons is responsible to put compiled library in the write place
    # ( lib/, package/, etc...)
    scons_scripts = ['SConstruct'],
    #scons_parameters = ["build_prefix="+build_prefix],

    namespace_packages = ["openalea", "vplants"],
    create_namespaces = True,

    py_modules = ["pgl_postinstall",],
    # pure python  packages
    packages= [ "vplants", 
                "vplants.plantgl",
                pkg_name, 
                pkg_name+'.math', 
                pkg_name+'.scenegraph', 
                pkg_name+'.algo',
                pkg_name+'.oaplugins', 
                pkg_name+'.gui', 
                pkg_name+'.wralea',
                pkg_name+'.wralea.demos', 
                pkg_name+'.wralea.edition', 
                pkg_name+'.wralea.objects', 
                pkg_name+'.wralea.operator', 
                pkg_name+'.wralea.visualization', 
                pkg_name+'.wralea.dresser',
                pkg_name+'.ext',
                pkg_name+'.codec'
                ],
    
    # python packages directory
    package_dir= { pkg_name : pj('src','plantgl'),
                   '' : 'src',
                   },

    # Add package platform libraries if any
    include_package_data = True,
    package_data = {'' : ['*.pyd', '*.so', '*.dylib'],},
    zip_safe = False,

    # Specific options of openalea.deploy
    lib_dirs = {'lib' : pj(build_prefix, 'lib'),},
    bin_dirs = {'bin':  pj(build_prefix, 'bin'),},
    inc_dirs = { 'include' : pj(build_prefix, 'include') },
    share_dirs = { 'share' : 'share'},
    postinstall_scripts = ['pgl_postinstall',],


    # Scripts
    #entry_points = { 'gui_scripts': [ 'pglviewer = openalea.plantgl:start_viewer',]},
    entry_points = {
        "wralea": ['plantgl.edition = openalea.plantgl.wralea.edition', 
                   'plantgl.objects = openalea.plantgl.wralea.objects', 
                   'plantgl.visualization = openalea.plantgl.wralea.visualization', 
                   'plantgl.operator = openalea.plantgl.wralea.operator', 
                   'plantgl.dresser = openalea.plantgl.wralea.dresser',
                   'plantgl.demos = openalea.plantgl.wralea.demos',
                   ],

        'oalab.qt_control':[
            'PluginColorListWidget = openalea.plantgl.oaplugins:PluginColorListWidget',
            'PluginMaterialListWidget = openalea.plantgl.oaplugins:PluginMaterialListWidget',
            'PluginCurve2DWidget = openalea.plantgl.oaplugins:PluginCurve2DWidget',
            'PluginQuantisedFunctionWidget = openalea.plantgl.oaplugins:PluginQuantisedFunctionWidget',
            'PluginPatchWidget = openalea.plantgl.oaplugins:PluginPatchWidget',
        ],
        'oalab.interface': [
            'PlantGLOAInterfacePlugin = openalea.plantgl.oaplugins.interfaces:PlantGLOAInterfacePlugin',
            ],
              },

 
    # Dependencies
    setup_requires = setup_requires + ['openalea.deploy'],
    install_requires = install_requires,
    dependency_links = ['http://openalea.gforge.inria.fr/pi'],
    pylint_packages = [ pylint_dir + os.sep + x.replace('.', os.sep) for x in find_packages(pylint_dir) if x not in ['gui']],

   )



    
