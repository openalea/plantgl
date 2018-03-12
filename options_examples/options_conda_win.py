#scons parameters file
#use this file to pass custom parameter to SConstruct script

compiler = 'msvc'
qhull_libs_suffix = 'static'

#EXTRA_LINKFLAGS ='/NODEFAULTLIB:boost_python-vc140-mt-1_65_1'
#EXTRA_LINKFLAGS +=' /MACHINE:'+('X86' if  sys.maxsize.bit_length() < 63 else 'X64')
