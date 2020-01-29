

def read_variable(varname, fname):
    import re
    stream = open(fname, 'r')
    text = stream.read()
    pattern = '#define\s+?'+varname+'\s+?(.+?)\s*?$'
    res = re.findall(pattern, text, re.M)
    if not res is None and len(res) > 0:
        res = res[0]
        try:
            res = eval(res)
        except:
            pass
    else : return None
    return res

def version_from_hex(hex_version):
    major = ((hex_version & 0xff0000) >> 16)
    minor = ((hex_version & 0x00ff00) >> 8)
    rev = (hex_version & 0x0000ff)
    return major, minor, rev

def hex_from_version(major, minor, revision):
    return (major << 16) + (minor << 8) + revision

class HexVersion (object):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return hex(self.value)

    def __repr__(self):
        return hex(self.value)

    def __int__(self):
        return self.value

    def __eq__(self, other):
        return self.value == int(other)

    def __ne__(self, other):
        return self.value != int(other)

    def __ge__(self, other):
        return self.value >= int(other)

    def __gt__(self, other):
        return self.value > int(other)

    def __le__(self, other):
        return self.value <= int(other)

    def __lt__(self, other):
        return self.value < int(other)

    def get_major(self): 
        return ((self.value & 0xff0000) >> 16)

    def set_major(self, value): 
        self.value = hex_from_version(value, self.get_minor(), self.get_revision())

    major = property(get_major, set_major)

    def get_minor(self): 
        return ((self.value & 0x00ff00) >> 8)

    def set_minor(self, value): 
        self.value = hex_from_version(self.get_major(), value, self.get_revision())

    minor = property(get_major, set_major)

    def get_revision(self): 
        return (self.value & 0x0000ff)

    def set_revision(self, value): 
        self.value = hex_from_version(self.get_major(), self.get_minor(), value)

    revision = property(get_revision, set_revision)

    def to_string(self):
        return str(self.get_major())+'.'+str(self.get_minor())+'.'+str(self.get_revision())

    @staticmethod
    def from_cpp_define(define, fname):
        return HexVersion(read_variable(define, fname))

    @staticmethod
    def from_py_variable(variable, fname):
        namespace = {}
        execfile(fname, namespace, namespace)
        return HexVersion(namespace[variable])

    @staticmethod
    def from_string(strvalue):
        numbers = map(int,strvalue.split('.'))
        if len(numbers) == 3:
            major,minor,revision = numbers
        elif len(numbers) == 2:
            major,minor = numbers
            revision = 0
        elif len(numbers) == 1:
            major = numbers
            minor, revision = 0, 0
        else :
            raise ValueError(strvalue)
        return HexVersion.from_numbers(major, minor, revision)

    @staticmethod
    def from_numbers(major, minor, revision):
        return HexVersion(hex_from_version(major,minor,revision))

