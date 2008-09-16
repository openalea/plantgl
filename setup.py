# -*- coding: iso-8859-15 -*-


import os, sys
from setuptools import setup
from openalea.deploy.binary_deps import binary_deps

pj = os.path.join

import versionreader

# Setup script

name = 'plantgl'
namespace = 'openalea'
pkg_name = 'openalea.plantgl'
version = versionreader.getPGLVersionString()
print pkg_name,': version =',version

description= 'PlantGL package for OpenAlea.' 
long_description= '''
Plant Geometric Library is a powerfull library to create and display vegetal 3D scene.
'''

author= 'Frederic Boudon, Christophe Pradal'
author_email= 'frederic.boudon@cirad.fr, christophe.pradal@cirad.fr'
url= 'http://openalea.gforge.inria.fr/dokuwiki/doku.php?id=packages:visualization:plantgl:plantgl'

license= 'Cecill-C' 

# Scons build directory
build_prefix= "build-scons"

if("win" in sys.platform):
    install_requires = ["boostpython", "qhull", "qt4"]
    setup_requires = install_requires + ["bisonflex"]
    install_requires = [ binary_deps(i) for i in install_requires ]
else:
    install_requires = []
    setup_requires = []
    

# Main setup
setup(
    name="VPlants.PlantGL",
    version=version,
    description=description,
    long_description=long_description,
    author=author,
    author_email=author_email,
    url=url,
    license=license,
    
    # Define what to execute with scons
    # scons is responsible to put compiled library in the write place
    # ( lib/, package/, etc...)
    scons_scripts = ['SConstruct'],
    scons_parameters = ["build_prefix="+build_prefix],

    namespace_packages = ["openalea"],
    create_namespaces = True,

    py_modules = ["pgl_postinstall",],
    # pure python  packages
    packages= [ "vplants.plantgl",
                pkg_name, 
                pkg_name+'.math', 
                pkg_name+'.scenegraph', 
                pkg_name+'.algo',
                pkg_name+'.gui', 
                pkg_name+'.gui3',
                pkg_name+'.wralea',
                pkg_name+'.wralea.objects', 
                pkg_name+'.wralea.visualization', 
                pkg_name+'.wralea.operator', 
                pkg_name+'.ext',
                pkg_name+'.codec'
                ],
    
    # python packages directory
    package_dir= { pkg_name : pj('src',name),
                   '' : 'src',
                   },

    # Add package platform libraries if any
    include_package_data = True,
    package_data = {'' : ['*.pyd', '*.so'],},
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
        "wralea": ['plantgl.objects = openalea.plantgl.wralea.objects', 
                   'plantgl.visualization = openalea.plantgl.wralea.visualization', 
                   'plantgl.operator = openalea.plantgl.wralea.operator', 
                   'plantgl.ext = openalea.plantgl.ext',
                   ]
              },

 
    # Dependencies
    setup_requires = setup_requires + ['openalea.deploy'],
    install_requires = install_requires,
    dependency_links = ['http://openalea.gforge.inria.fr/pi'],
   )



    
