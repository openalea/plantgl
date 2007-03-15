# -*- coding: iso-8859-15 -*-

# Header

#Check dependencies

#####################
# Import dependencies

import os, sys
pj= os.path.join

try:
    from openalea import config
except ImportError:
    print """
ImportError : openalea.config not found. 
Please install the openalea package before.	
See http://openalea.gforge.inria.fr
"""
    sys.exit()

try:
    from openalea.distx import setup,Shortcut
except ImportError:
    print """
ImportError : openalea.distx package not found.
Please, first install the openalea.distx package.
See http://openalea.gforge.inria.fr
"""
    sys.exit()


##############
# Setup script

# Package name
name= 'plantgl'

#openalea namespace
namespace=config.namespace 

pkg_name= namespace + '.' + name

# Package version policy
# major.minor.patch
# alpha: patch + 'a'+ 'number'
# beta: patch= patch + 'b' + 'number'
major= '2'
minor= '0'
patch= '0'
version= '%s.%s.%s' % (major, minor, patch)

# Description of the package

# Short description
description= 'PlantGL package for OpenAlea.' 

long_description= '''
Plant Geometric Library is a powerfull library to create and display vegetal 3D scene.
'''

# Author
author= 'Frederic Boudon, Christophe Pradal'
author_email= 'frederic.boudon@cirad.fr, christophe.pradal@cirad.fr'

# URL
url= 'http://openalea.gforge.inria.fr/dokuwiki/doku.php?id=packages:visualization:plantgl:plantgl'

# License: License for the starter package.
# Please, choose an OpenSource license for distribution of your package.

# LGPL compatible INRIA license
license= 'Cecill-C' 

# Scons build directory
build_prefix= "build-scons"

# For other meta-information, please read the Python distutils documentation.

# Main setup
setup(
    # Meta data
    name="PlantGL",
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

    # scons parameters  
    scons_parameters = ["build","build_prefix="+build_prefix],
    
    namespace=[namespace],

    # pure python  packages
    packages= [ pkg_name, pkg_name+'.math', pkg_name+'.scenegraph', pkg_name+'.algo',
                pkg_name+'.gui', pkg_name+'.wralea', ],
    # python packages directory
    package_dir= { pkg_name : pj('src',name),
                   pkg_name+'.math' :pj( 'src', name, 'math' ),
                   pkg_name+'.scenegraph' :pj( 'src', name, 'scenegraph' ),
                   pkg_name+'.algo' :pj( 'src', name, 'algo' ),
                   pkg_name+'.gui' :pj( 'src', name, 'gui' ),
                   pkg_name+'.wralea' :pj( 'src', name, 'wralea' ),
                   },
                   
    # Add package platform libraries if any
    include_package_lib = True,

    # copy shared data in default OpenAlea directory
    # map of 'destination subdirectory' : 'source subdirectory'
    external_data={'share' : 'share' ,
                   pj('include', name) : pj(build_prefix, 'include', name),
                   pj('lib'):  pj(build_prefix,'lib'),
                   pj('bin'):  pj(build_prefix,'bin'),
                   },

    

    # Add shortcuts
    win_shortcuts = [Shortcut( name='PlantGLViewer', target=pj(config.bin_dir,'pglviewer.exe'), arguments='', group='OpenAlea', icon =pj(config.bin_dir,'pglviewer.exe')), ],
    freedesk_shortcuts = [Shortcut ( name = 'PlantGLViewer', target = pj(config.bin_dir,'pglviewer.exe'), arguments = '', group = 'OpenAlea', icon='' )],

    # Windows registery (list of (key, subkey, name, value))
    winreg = [ ('HKCR', '.smb',   '', 'PGLFile') ,
		       ('HKCR', '.lig',   '', 'PGLFile') ,
		       ('HKCR', '.geom',  '', 'PGLFile') ,
		       ('HKCR', '.bgeom', '', 'PGLFile') ,
		       ('HKCR', 'PGLFile', '', 'PlantGL File') ,
		       ('HKCR', 'PGLFile\\DefaultIcon', '', pj(config.bin_dir,'pglviewer.exe')+",1") ,
		       ('HKCR', 'PGLFile\\shell', '', "open") ,
		       ('HKCR', 'PGLFile\\shell\\open', '', "Open") ,
		       ('HKCR', 'PGLFile\\shell\\open\\command', '', '"'+pj(config.bin_dir,'pglviewer.exe')+'" "%1"') ,
		       ('HKCR', 'PGLFile\\shell\\add', '', "Add") ,
		       ('HKCR', 'PGLFile\\shell\\add\\command', '', '"'+pj(config.bin_dir,'pglviewer.exe')+'" -a "%1"') ,
		       # ('HKCR', 'PGLFile\\shell\\edit', '', "Edit") ,
		       # ('HKCR', 'PGLFile\\shell\\edit\\command', '', '"'+pj(config.bin_dir,'pgleditor.exe')+'" "%1"') ,
		       # ('HKCR', 'PGLFile\\shell\\geom2pov', '', "Translate To Povray") ,
		       # ('HKCR', 'PGLFile\\shell\\geom2pov\\command', '', '"'+pj(config.bin_dir,'pgl2pov.exe')+'" "%1"') ,
		       ]

    )


