#scons parameters file
#use this file to pass custom parameter to SConstruct script

build_prefix="build-scons"

import sys
if('win' in sys.platform):
    
    compiler='msvc'
    boost_lib= '$openalea_lib'
    boost_includes= '$openalea_includes'
    qhull_lib= '$openalea_lib'
    qhull_includes= '$openalea_includes'
    if compiler == 'mingw':
        boost_libs_suffix='-mgw'
        qhull_libs_suffix='-mgw'
        gl_includes = r'C:\MinGW\include\GL'
        gl_lib = r'C:\MinGW\lib' 
    else: 
        boost_libs_suffix='-vc80'
        qhull_libs_suffix='-vc80'
        gl_includes= r'C:\Program Files\Microsoft Platform SDK\Include'
        gl_lib= r'C:\Program Files\Microsoft Platform SDK\Lib'
        EXTRA_LIBS = 'advapi32.lib user32.lib gdi32 shell32.lib'

