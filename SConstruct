# -*-python-*-

from openalea.sconsx import config, environ
from openalea.sconsx.util.qt_check import detect_installed_qt_version
from openalea.sconsx.util.buildprefix import fix_custom_buildprefix
import os, sys
#from sconsx_ext.mpfr import create as mpfr_create


pj= os.path.join
ALEASolution = config.ALEASolution


options = Variables( ['../options.py', 'options.py'], ARGUMENTS )
options.Add(EnumVariable('QT_VERSION', 'Qt major version to use',str(detect_installed_qt_version(4)), allowed_values=('4','5','None')))
options.Add(BoolVariable('WITH_CGAL','Use CGAL',True))
options.Add(BoolVariable('USE_DOUBLE','Use Double Floating Precision',True))


# Create an environment to access qt option values
test_env = Environment(options=options, tools=[])
qt_version = eval(test_env['QT_VERSION'])


tools = ['bison', 'flex', 'opengl', 'qhull', 'eigen', 'ann']

if not qt_version is None:
    tools += ['qt'+str(qt_version)]

if test_env['WITH_CGAL']:
    tools += ['cgal', 'mpfr']

env = ALEASolution(options, tools)

env.Prepend( CPPPATH = pj( '$build_includedir','plantgl' ) )
env.AppendUnique( CPPDEFINES = ['PGL_USE_DOUBLE' if env['USE_DOUBLE'] else 'PGL_USE_FLOAT']  )

if not qt_version:
    env.AppendUnique( CPPDEFINES = ['PGL_WITHOUT_QT'] )


try:
    # Test the whether SconsX provides FLEX and BISON flags
    env['WITH_FLEX'] and env['WITH_BISON']
except KeyError as ke:
    env['WITH_FLEX']  = not env['LEX'] is None and os.path.exists(env['LEX'])
    if env['WITH_FLEX']: env.Append(CPPDEFINES =["WITH_FLEX"])

    env['WITH_BISON'] = not env['YACC'] is None and os.path.exists(env['YACC'])
    if env['WITH_BISON']: env.Append(CPPDEFINES =["WITH_BISON"])

if 'linux' in sys.platform:
    env.AppendUnique( LIBS = ['z'] )

env.Append( ENV = {'PATH' : os.environ['PATH']} )

prefix = env['build_prefix']

from versionmanager import deployconfig
# create config files
deployconfig(env)

SConscript( pj(prefix,"src/cpp/plantgl/SConscript"), exports={"env":env} )

if env.get('WITH_BOOST',True) :
    print("IMPORTANT : Wrappers will be build. Boost.Python available.")
    SConscript( pj(prefix,"src/wrapper/SConscript"),
                exports={"env":env} )
else:
    print("IMPORTANT : Wrappers will not be build. Boost.Python not available.")

Default("build")

if isinstance(config.platform, config.Win32):
    print ('MSVC_VERSION',env['MSVC_VERSION'])
    print ('TARGET_ARCH',env['TARGET_ARCH'])


fix_custom_buildprefix(env)
