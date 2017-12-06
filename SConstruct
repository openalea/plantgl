# -*-python-*-

from openalea.sconsx import config, environ
import os, sys
#from sconsx_ext.mpfr import create as mpfr_create


pj= os.path.join
ALEASolution = config.ALEASolution


options = Variables( ['../options.py', 'options.py'], ARGUMENTS )
options.Add(EnumVariable('QT_VERSION','Qt major version to use','4',allowed_values=('4','5','None')))
options.Add(BoolVariable('WITH_CGAL','Use CGAL',True))
options.Add(BoolVariable('USE_DOUBLE','Use Double Floating Precision',True))

# Create an environment to access qt option values
qt_env = Environment(options=options, tools=[])

qt_version = qt_env['QT_VERSION']
if qt_version in '45':   qt_version = int(qt_version)
else:                    qt_version = None
qt_env['QT_VERSION'] = qt_version

tools = ['bison', 'flex', 'opengl', 'qhull','boost_python','cgal','eigen', 'mpfr','ann']
if qt_version :
    tools += ['qt'+str(qt_version)]

env = ALEASolution(options, tools)

env.Prepend( CPPPATH = pj( '$build_includedir','plantgl' ) )
if qt_version:
    env.AppendUnique( CPPPATH = ['$QT'+str(qt_version)+'_CPPPATH/Qt'] )
    env.AppendUnique( CPPPATH = ['$QT'+str(qt_version)+'_CPPPATH'] )
else:
    env.AppendUnique( CPPDEFINES = ['PGL_WITHOUT_QT'] )


if env['USE_DOUBLE']:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_DOUBLE'] )
else:
    env.AppendUnique( CPPDEFINES = ['PGL_USE_FLOAT'] )

if env['WITH_CGAL']:
    env.AppendUnique( CPPDEFINES = ['WITH_CGAL'] )

try:
    # Test the whether SconsX provides FLEX and BISON flags
    env['WITH_FLEX'] and env['WITH_BISON']
except KeyError, ke:
    env['WITH_FLEX']  = not env['LEX'] is None and os.path.exists(env['LEX'])
    if env['WITH_FLEX']: env.Append(CPPDEFINES =["WITH_FLEX"])

    env['WITH_BISON'] = not env['YACC'] is None and os.path.exists(env['YACC'])
    if env['WITH_BISON']: env.Append(CPPDEFINES =["WITH_BISON"])

if 'linux' in sys.platform:
    env.AppendUnique( LIBS = ['z'] )


# Build stage
prefix = env['build_prefix']

SConscript( pj(prefix,"src/cpp/plantgl/SConscript"),
            exports={"env":env} )

if env.get('WITH_BOOST',True) :
    SConscript( pj(prefix,"src/wrapper/SConscript"),
                exports={"env":env} )

Default("build")

def generate_qtbuilddir():
    standartprefix = 'build-scons'
    if os.path.basename(prefix) != standartprefix:
        if os.path.exists(standartprefix):
            if os.path.isdir(standartprefix) and not os.path.islink(standartprefix): 
                import shutil
                shutil.rmtree(standartprefix)
            else: os.remove(standartprefix)
        os.symlink(prefix, standartprefix)

if os.name == 'posix' and qt_version and not 'CONDA_BUILD' in os.environ:
    generate_qtbuilddir()