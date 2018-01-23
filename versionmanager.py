from openalea.sconsx.util.configdeploy import *

#################################################################

def getPGLVersion():
    p = os.path.join('src', 'cpp', 'plantgl', 'version.h')
    return HexVersion.from_cpp_define('PGL_VERSION',p)

def getMetaInfo():
    metainfo = {}
    execfile(os.path.join('src','plantgl','__init__.py'),metainfo,metainfo)
    return metainfo['__metainfo__']

def get_config_vars(env):
    from collections import OrderedDict

    result = OrderedDict()

    result['PGL_VERSION'] = getPGLVersion()
    result['PGL_VERSION_STR'] = result['PGL_VERSION'].to_string()

    qtversion = eval(env['QT_VERSION'])
    if qtversion is None:
        result['PGL_WITHOUT_QT'] = True
    else:
        result['PGL_WITHOUT_QT'] = False
        result['PGL_QT_VERSION'] = qtversion

    result['PGL_USE_DOUBLE'] = env['USE_DOUBLE']

    result = get_config_from_env(env, result, 'PGL')

    return result


def deployconfig(env):
    cconfig = get_config_vars(env)

    generate_config('PlantGL', cconfig, 
                    pyconfigfname = os.path.join('src', 'plantgl', 'config.py'),
                    cppconfigfname = os.path.join('src', 'cpp', 'plantgl', 'userconfig.h'))

    generate_conda_config(cconfig['PGL_VERSION'])

#################################################################

if __name__ == '__main__':
    print (getPGLVersion().to_string())
