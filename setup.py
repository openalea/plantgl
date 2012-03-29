# -*- coding: utf-8 -*-
__revision__ = "$Id$"

import os, sys
from setuptools import setup, find_packages
from openalea.deploy.binary_deps import binary_deps
pj = os.path.join

import versionreader

from openalea.deploy.metainfo import read_metainfo
metadata = read_metainfo('metainfo.ini', verbose=True)
for key,value in metadata.iteritems():
    exec("%s = '%s'" % (key, value))


# Setup script
meta_version = version 
version = versionreader.getPGLVersionString()
if meta_version != version:
    print 'Warning:: Update the version in metainfo.ini !!'
print pkg_name,': version =',version

# Scons build directory
build_prefix= "build-scons"

if sys.platform.startswith('win'):
    install_requires = ["boost", "qhull"]#, "qt4"]
    #setup_requires = install_requires +['qt4-dev'] 
    install_requires = [ binary_deps(i) for i in install_requires ]
#else:
install_requires = []
setup_requires = []
#if sys.platform.startswith('win'): 
    #setup_requires.append(["bisonflex"])

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
    scons_parameters = ["build_prefix="+build_prefix],

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
                pkg_name+'.gui', 
                pkg_name+'.gui3',
                pkg_name+'.wralea',
                pkg_name+'.wralea.demos', 
                pkg_name+'.wralea.edition', 
                pkg_name+'.wralea.objects', 
                pkg_name+'.wralea.operator', 
                pkg_name+'.wralea.visualization', 
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
                   'plantgl.ext = openalea.plantgl.ext',
                   'plantgl.demos = openalea.plantgl.wralea.demos',
                   ]
              },

 
    # Dependencies
    setup_requires = setup_requires + ['openalea.deploy'],
    install_requires = install_requires,
    dependency_links = ['http://openalea.gforge.inria.fr/pi'],
    pylint_packages = [ pylint_dir + os.sep + x.replace('.', os.sep) for x in find_packages(pylint_dir) if x not in ['gui']],

   )



    
