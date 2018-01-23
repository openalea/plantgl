
def getPGLVersionString():
    from openalea.sconsx.util.versionreader import read_variable, strversion_from_hex
    p = os.path.join('src', 'cpp', 'plantgl', 'version.h')
    return strversion_from_hex(read_variable('PGL_VERSION',p))

if __name__ == '__main__':
    print getPGLVersionString()
