# -*-python-*-

from openalea.sconsx import config, environ
import os, sys
#from sconsx_ext.mpfr import create as mpfr_create


pj= os.path.join
ALEASolution = config.ALEASolution


options = Variables( ['../options.py', 'options.py'], ARGUMENTS )
options.Add(EnumVariable('QT_VERSION','Qt major version to use','4',allowed_values=('4','5')))
options.Add(BoolVariable('WITH_CGAL','Use CGAL',True))
options.Add(BoolVariable('USE_DOUBLE','Use Double Floating Precision',True))

# Create an environment to access qt option values
qt_env = Environment(options=options, tools=[])
qt_version = int(qt_env['QT_VERSION'])

tools = ['bison', 'flex', 'opengl', 'qhull','boost_python','boost_thread','cgal','eigen', 'mpfr','ann', 'qt'+str(qt_version)]

env = ALEASolution(options, tools)

env.Prepend( CPPPATH = pj( '$build_includedir','plantgl' ) )
env.AppendUnique( CPPPATH = ['$QT'+str(qt_version)+'_CPPPATH/Qt'] )
env.AppendUnique( CPPPATH = ['$QT'+str(qt_version)+'_CPPPATH'] )

if env['USE_DOUBLE']:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_DOUBLE'] )
else:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_FLOAT'] )

if env['WITH_CGAL']:
    env.AppendUnique( CPPDEFINES = ['WITH_CGAL'] )
    
#if 'linux' in sys.platform:
    # By default for linux, use unordered map
    # env.AppendUnique( CPPDEFINES = ['USING_UNORDERED_MAP'] )


# Build stage
prefix= env['build_prefix']
print prefix

SConscript( pj(prefix,"src/cpp/plantgl/SConscript"),
            exports={"env":env} )

SConscript( pj(prefix,"src/wrapper/SConscript"),
            exports={"env":env} )

Default("build")

standartprefix = 'build-scons'
if prefix != standartprefix:
    if os.path.exists(standartprefix):
        if os.path.isdir(standartprefix) and not os.path.islink(standartprefix): 
            import shutil
            shutil.rmtree(standartprefix)
        else: os.remove(standartprefix)
    os.symlink(prefix, standartprefix)
