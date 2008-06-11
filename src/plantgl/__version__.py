__version_number__ = 0x020300
__revision_str__="$Revision: $"
__revision__ = __revision_str__.split(':')[1][1:-1]

PGL_VERSION_MAJOR = ((__version_number__ & 0xff0000) >> 16)
PGL_VERSION_MINOR = ((__version_number__ & 0x00ff00) >> 8)
PGL_VERSION_REV   = (__version_number__ & 0x0000ff)

PGL_VERSION_STR = str(PGL_VERSION_MAJOR)+'.'+str(PGL_VERSION_MINOR)+'.'+str(PGL_VERSION_REV)
if len(__revision__) > 0:
    PGL_VERSION_STR +='-r'+__revision__
