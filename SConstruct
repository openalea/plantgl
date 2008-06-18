# -*-python-*-

from openalea.sconsx import config, environ
import os, sys

#Config= config.Config
ALEAConfig= config.ALEAConfig
ALEAEnvironment= config.ALEAEnvironment

pj= os.path.join

name='plantgl'

SConsignFile()

options = Options( ['../options.py', 'options.py'], ARGUMENTS )
options.Add(EnumOption('QT_VERSION','Qt major version to use','4',allowed_values=('3','4')))
options.Add(BoolOption('WITH_CGAL','Use CGAL',False))
options.Add(BoolOption('USE_DOUBLE','Use Double Floating Precision',True))

env = Environment(options=options)
qt_env = env
qt_version = int(qt_env['QT_VERSION'])

cpp_tools = ['alea', 'flex', 'bison','opengl','readline', 'install', 'qhull','boost_python','cgal']
if qt_version == 3:
    qt_tools = ['qt']
else:
    qt_tools = ['qt4']


cpp_conf= ALEAConfig(name, cpp_tools+qt_tools)

# Set all the common options for the package
cpp_conf.UpdateOptions( options )

#cgaltool = cpp_conf.find_tool('cgal')
#cgaltool.configure( options )

options.Update( env )
cpp_conf.Update( env )

# Generate Help available with the cmd scons -h
Help(options.GenerateHelpText(env))

# Set build directory
prefix= env['build_prefix']
BuildDir( prefix, '.' )

env.Prepend(CPPPATH='$build_includedir')
env.Prepend(LIBPATH='$build_libdir')
env.Prepend( CPPPATH = pj( '$build_includedir','plantgl' ) )
if qt_version == 4:
    env.AppendUnique( CPPPATH = ['$QT4_CPPPATH/Qt'] )
if env['USE_DOUBLE']:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_DOUBLE'] )
else:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_FLOAT'] )

# Build stage
SConscript( pj(prefix,"src/cpp/plantgl/SConscript"),
            exports={"env":env} )

SConscript( pj(prefix,"src/wrapper/SConscript"),
            exports={"env":env} )


Default("build")
