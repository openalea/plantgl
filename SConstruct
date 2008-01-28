# -*-python-*-

from openalea.sconsx import config, environ
import os

Config= config.Config
ALEAConfig= config.ALEAConfig
ALEAEnvironment= config.ALEAEnvironment

pj= os.path.join

name='plantgl'

SConsignFile()

options = Options( ['../options.py', 'options.py'], ARGUMENTS )
options.Add(EnumOption('QT_VERSION','Qt major version to use','4',allowed_values=('3','4')))

env = Environment(options=options)
qt_env = env
qt_version = int(qt_env['QT_VERSION'])

#wrapper_tools = ['boost_python', 'alea']
cpp_tools = ['alea', 'flex', 'bison','opengl','readline', 'install', 'qhull','boost_python']
if qt_version == 3:
    qt_tools = ['qt']
else:
    qt_tools = ['qt4']

#wrapper_conf= ALEAConfig(name,wrapper_tools+qt_tools)
cpp_conf= ALEAConfig(name, cpp_tools+qt_tools)
#opt_conf= ALEAConfig(name, wrapper_tools+cpp_tools+qt_tools)

# Set all the common options for the package
#opt_conf.UpdateOptions( options )
cpp_conf.UpdateOptions( options )

#opt_env= Environment( options=options )
options.Update(env)
#opt_conf.Update( opt_env )
cpp_conf.Update( env )

# Generate Help available with the cmd scons -h
Help(options.GenerateHelpText(env))

# Set build directory
prefix= env['build_prefix']
BuildDir( prefix, '.' )

#env= ALEAEnvironment( cpp_conf, options=options )
env.Prepend(CPPPATH='$build_includedir')
env.Prepend(LIBPATH='$build_libdir')
env.Prepend( CPPPATH = pj( '$build_includedir','plantgl' ) )
#cpp_env.Append( QT_VERSION = qt_version )
if qt_version == 4:
    env.AppendUnique( CPPPATH = ['$QT4_CPPPATH/Qt'] )

#wrapper_env= ALEAEnvironment( wrapper_conf, options=options )
#wrapper_env.Append( CPPPATH = pj( '$build_includedir','plantgl' ) )
#wrapper_env.Append( QT_VERSION = qt_version )
#if qt_version == 4:
#    wrapper_env.AppendUnique( CPPPATH = ['$QT4_CPPPATH/Qt'] )

# Build stage
SConscript( pj(prefix,"src/cpp/plantgl/SConscript"),
            exports={"env":env} )

SConscript( pj(prefix,"src/wrapper/SConscript"),
            exports={"env":env} )


Default("build")
