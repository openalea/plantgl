# -*-python-*-

from openalea.sconsx import config, environ
import os, sys

pj= os.path.join
ALEASolution = config.ALEASolution

options = Options( ['../options.py', 'options.py'], ARGUMENTS )
options.Add(EnumOption('QT_VERSION','Qt major version to use','4',allowed_values=('3','4')))
options.Add(BoolOption('WITH_CGAL','Use CGAL',False))
options.Add(BoolOption('USE_DOUBLE','Use Double Floating Precision',True))

# Create an environment to access qt option values
env = Environment(options=options, tools=[])
qt_env = env
qt_version = int(qt_env['QT_VERSION'])

cpp_tools = ['bison', 'flex', 'bison','opengl','readline', 'install', 'qhull','boost_python','cgal']
if qt_version == 3:
    qt_tools = ['qt']
else:
    qt_tools = ['qt4']

tools = cpp_tools + qt_tools

env = ALEASolution(options, tools)

env.Prepend( CPPPATH = pj( '$build_includedir','plantgl' ) )
if qt_version == 4:
    env.AppendUnique( CPPPATH = ['$QT4_CPPPATH/Qt'] )
if env['USE_DOUBLE']:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_DOUBLE'] )
else:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_FLOAT'] )

# Build stage
prefix= env['build_prefix']
SConscript( pj(prefix,"src/cpp/plantgl/SConscript"),
            exports={"env":env} )

SConscript( pj(prefix,"src/wrapper/SConscript"),
            exports={"env":env} )

Default("build")
