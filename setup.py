# -*- coding: utf-8 -*-
__revision__ = "$Id$"

import os, sys
from setuptools import setup, find_namespace_packages
from hexversion import *
pj = os.path.join

def getMetaInfo():
    metainfo = {}
    exec(open(pj('src', 'openalea','plantgl', '__init__.py')).read(), metainfo, metainfo)
    return metainfo['__metainfo__']

globals().update(getMetaInfo())

namespace = 'openalea'

# Setup script
version = HexVersion.from_cpp_define('PGL_VERSION',pj('src','cpp','plantgl','version.h')).to_string()
# version = '3.13.1'
#print(pkg_name + ' : version = ' + version)

currentdir = os.path.dirname(__file__)

# Scons build directory
build_prefix= "build-cmake"

if 'CONDA_PREFIX' in os.environ or 'PREFIX' in os.environ :
    deploy_args = dict() # Do not declare namespace_package : see https://github.com/conda/conda-build/issues/649
else:
    deploy_args = dict(
        # Specific options of openalea.deploy
        lib_dirs = {'lib' : pj(currentdir, build_prefix, 'lib'),},
        bin_dirs = {'bin':  pj(currentdir, build_prefix, 'bin'),},
        inc_dirs = { 'include' : pj(currentdir, build_prefix, 'include') },
        share_dirs = { 'share' : 'share'},
        postinstall_scripts = ['pgl_postinstall',],
        #namespace_packages = [namespace],
        create_namespaces = False,
    )

# Main setup
setup(
    name = name,
    version = version,
    description = description,
    long_description = long_description,
    author = authors,
    author_email = authors_email,
    url = url,
    license = license,

    # pure python packages
    #packages = find_namespace_packages(where='src',include=['openalea','openalea.*'])
    packages = [ 
        namespace,
        pkg_name, 
        pkg_name + '.math', 
        pkg_name + '.scenegraph', 
        pkg_name + '.algo',
        pkg_name + '.oaplugins', 
        pkg_name + '.gui', 
        pkg_name + '.gui.qt', 
        pkg_name + '.light', 
        pkg_name + '.wralea',
        pkg_name + '.wralea.demos', 
        pkg_name + '.wralea.edition', 
        pkg_name + '.wralea.objects', 
        pkg_name + '.wralea.operator', 
        pkg_name + '.wralea.visualization', 
        pkg_name + '.wralea.dresser',
        pkg_name + '.ext',
        pkg_name + '.codec'
    ],
    
    # python packages directory
    package_dir = {
        '' : 'src'
    },

    # Add package platform libraries if any
    include_package_data = True,
    package_data = { '': ['*.pyd', '*.so', '*.dylib'] },
    zip_safe = False,

    # Scripts
    entry_points = {
        'console_scripts': ['pgl2pov = openalea.plantgl.codec.translation:pgl2pov',],
        "wralea": [
            'plantgl.edition = openalea.plantgl.wralea.edition', 
            'plantgl.objects = openalea.plantgl.wralea.objects', 
            'plantgl.visualization = openalea.plantgl.wralea.visualization', 
            'plantgl.operator = openalea.plantgl.wralea.operator', 
            'plantgl.dresser = openalea.plantgl.wralea.dresser',
            'plantgl.demos = openalea.plantgl.wralea.demos'
        ],
        'oalab.qt_control': [
            'PluginColorListWidget = openalea.plantgl.oaplugins:PluginColorListWidget',
            'PluginMaterialListWidget = openalea.plantgl.oaplugins:PluginMaterialListWidget',
            'PluginCurve2DWidget = openalea.plantgl.oaplugins:PluginCurve2DWidget',
            'PluginQuantisedFunctionWidget = openalea.plantgl.oaplugins:PluginQuantisedFunctionWidget',
            'PluginPatchWidget = openalea.plantgl.oaplugins:PluginPatchWidget'
        ],
        'oalab.interface': [
            'PlantGLOAInterfacePlugin = openalea.plantgl.oaplugins.interfaces:PlantGLOAInterfacePlugin'
        ]
    },

    **deploy_args
)
