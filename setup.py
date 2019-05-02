# -*- coding: utf-8 -*-
__revision__ = "$Id$"

import os, sys
from setuptools import setup, find_packages
pj = os.path.join

def getMetaInfo():
    metainfo = {}
    exec(open(pj('src', 'plantgl', '__init__.py')).read(), metainfo, metainfo)
    return metainfo['__metainfo__']

globals().update(getMetaInfo())

# Setup script
version = '2.4.0'
print(pkg_name + ' : version = ' + version)

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

    namespace_packages = ["openalea"],
    create_namespaces = True,

    # pure python packages
    packages = [ 
        pkg_name, 
        pkg_name + '.math', 
        pkg_name + '.scenegraph', 
        pkg_name + '.algo',
        pkg_name + '.oaplugins', 
        pkg_name + '.gui', 
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
        pkg_name: pj('src', 'plantgl'),
        '' : 'src'
    },

    # Scripts
    entry_points = {
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
    }
)
