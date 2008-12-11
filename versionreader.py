import re
import os


pj = os.path.join


class VersionReader:

    def __init__(self, fname):
        stream = file(fname, 'r')
        self.text = stream.read()

    def find(self, varname):
        pattern = '#define\s+?'+varname+'\s+?(.+?)\s*?$'
        res = re.findall(pattern, self.text, re.M)
        if not res is None:
            res = res[0]
            try:
                res = eval(res)
            except:
                pass
        return res


class PGLVersionReader (VersionReader):

    def __init__(self):
        VersionReader.__init__(self, pj('src', 'cpp', 'plantgl', 'version.h'))

    def getVersion(self):
        return self.find('PGL_VERSION')

    def getRevision(self):
        return self.find('PGL_SVNREVISION')


def version_str(hex_version, str_revision = None):
    major = ((hex_version & 0xff0000) >> 16)
    minor = ((hex_version & 0x00ff00) >> 8)
    rev = (hex_version & 0x0000ff)
    vers_str = str(major)+'.'+str(minor)+'.'+str(rev)
    # if not str_revision is None :
        # str_revision = str_revision.split(':')[1][1:-1]
        # if len(str_revision) > 0:
            # vers_str +='-r'+str_revision
    return vers_str


def getPGLVersionString():
    p = PGLVersionReader()
    return version_str(p.getVersion(), p.getRevision())

if __name__ == '__main__':
    print getPGLVersionString()
