#scons parameters file
#use this file to pass custom parameter to SConstruct script

#build_prefix="build_scons"

import sys
if('win' in sys.platform):
    
    compiler='mingw'
    # compiler= 'mingw'

    boost_lib= '$openalea_lib'
    boost_includes= '$openalea_includes'

    boost_libs_suffix='-mgw'
    #boost_libs_suffix='-vc80'


        
