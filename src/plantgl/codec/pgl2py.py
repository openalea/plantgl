
def is_pgl_class(name):
    import openalea.plantgl.all as pgl
    return name in pgl.__dict__ or name in ['Transfo']

def read(fname, macro):
    import os
    dir = os.path.dirname(fname)
    basename = os.path.basename(fname)
    cwd = os.getcwd()
    if dir:
        os.chdir(dir)
    try:
        txt = file(basename,'r').read()
        txt = preprocess(txt, macro)
    except Exception, e:
        os.chdir(cwd)
        raise e
    os.chdir(cwd)
    return txt

def preprocess_ifdef(txt, macro):
    def apply_lexer(txts, pattern, withname = True):
        from itertools import product, chain
        res = []
        for txt in txts:
            if type(txt) == str:
                subtxts = txt.split(pattern)
                if len(subtxts) > 1:
                    if not withname:
                        if subtxts[0] != '': res.append(subtxts[0]) 
                        res += sum([[(pattern,''),sb] for sb in subtxts[1:]],[])
                        if res[-1] == '':
                            del res[-1]
                    else:
                        def maketuple(subtxt):
                            a,b = subtxt.split('\n',1)
                            return [(pattern,a.strip()),b]
                        if subtxts[0] != '': res.append(subtxts[0]) 
                        res += sum([maketuple(sb) for sb in subtxts[1:]],[])
                else:
                    res += subtxts
            else:
                res.append(txt)

        return res

    txt = [txt]
    for kwd in [':Ifdef', ':Ifndef', ':Undef', ':Def', ':Define', ':include', ':Include']:
        txt = apply_lexer(txt,kwd)
    txt = apply_lexer(txt,':Else', False)
    txt = apply_lexer(txt,':Endif', False)

    res = []
    adding = True
    recdef = 0
    addstack = []

    for txtcmd in txt:
        if type(txtcmd) == str:
            if adding : res.append(txtcmd)
        else:
            cmd, val = txtcmd
            if cmd == ':Define' or cmd == ':Def':
                if adding :
                    macro[val] = True
            elif cmd == ':Ifdef':
                if adding :
                    addstack.append(adding)
                    adding = val in macro
                else:
                    recdef += 1
            elif cmd == ':Ifndef':
                if adding :
                    addstack.append(adding)
                    adding = not val in macro
                else:
                    recdef += 1
            elif cmd == ':Else':
                if adding :
                    adding = not adding
            elif cmd == ':Endif':
                if recdef > 0:
                    recdef -= 1
                else:
                    adding = addstack.pop(-1)
            elif cmd == ':include' or cmd == ':Include':
                if adding:
                    res.append(read(val,macro))
            else:
                raise ValueError(cmd)
    return ''.join(res)

def preprocess(txt, macro):
    a = txt.find('(#')
    while a >= 0:
        b = txt.find('#)')
        txt = txt[:a]+txt[b+2:]
        a = txt.find('(#')

    a = txt.find('#')
    while a >= 0:
        b = txt.find('\n',a)
        txt = txt[:a]+txt[b+1:]
        a = txt.find('#')

    txt = preprocess_ifdef(txt, macro)
    #print txts

    return txt


def lex(txt):
    def buildvector(cmds): return eval('('+''.join(cmds)+')')

    def buildlist(cmds): 
        if type(cmds[0]) == str:
            if is_pgl_class(cmds[0]):
                return cmds
            else : 
                return [eval(cmd) for cmd in cmds if cmd != ',']
        else:
            return [cmd for cmd in cmds if cmd != ',']

    markers = { '{' : '}', '<' : '>', '(' : ')', '[' : ']'}
    builders = { '<' : buildvector, '[' : buildlist }
    for bm, em in markers.items():
        txt = txt.replace(bm,' '+bm+' ')
        txt = txt.replace(em,' '+em+' ')
    txt = txt.replace(',',' , ')
    print txt
    tokens = txt.split()
    nbtokens = len(tokens)


    def process(currentid, stopcondition):
        res = []
        while  not stopcondition(currentid):
            current = tokens[currentid]
            if current in markers:
                lres, currentid = process(currentid+1, lambda currentid : tokens[currentid] == markers[current])
                if current in builders:
                    res.append(builders[current](lres))
                else:
                    res.append([current]+lres+[tokens[currentid]])
            else:
                res.append(current)
            currentid += 1 
        return res, currentid
    res, currentid  = process(0, lambda currentid : currentid >= nbtokens)
    return res

def rename_attribute(oname, newname, attdict):
    if oname in attdict:
        attdict[newname] = attdict[oname]
        del attdict[oname]

def apply_attribute(name, attdict, func):
    if name in attdict:
        attdict[name] = func(attdict[name])


class PglObj:
    def __init__(self):
        self.name = None
        self.class_ = None
        self.attributes = {}
        self.custombuild = {'Transfo','Shape'}
    def create(self, objmap):
        if self.class_ in self.custombuild:
            result = self.customcreate(objmap)
        else:
            result = self.stdcreate(objmap)

        if self.name : result.name = self.name
        return result

    def stdcreate(self,objmap):
        import traceback, sys
        import openalea.plantgl.all as pgl
        attributes = self.get_attributes(objmap)
        self.fix(attributes)
        print attributes
        try:
            result = pgl.__dict__[self.class_](**attributes)
        except:
            traceback.print_exception(*sys.exc_info())
            raise ValueError(self.class_, attributes)
        return result

    def get_attributes(self, objmap):
        res = {}
        for aname, aval in self.attributes.items():
            aname = aname[0].lower()+aname[1:]
            if type(aval) == str:
                try :
                    aval = eval(aval)
                except: 
                    try: 
                        aval = objmap[aval]
                    except:
                        pass
            res[aname] = aval
        return res

    def fix(self, attributes):
        import math
        if self.class_ == 'ElevationGrid':
            rename_attribute('xSpacing','xspacing', attributes)
            rename_attribute('ySpacing','yspacing', attributes)
        elif self.class_ == 'TriangleSet':
            rename_attribute('cCW','ccw', attributes)
        elif self.class_ == 'EulerRotated':
            apply_attribute('Azimuth',attributes, math.radians)
            apply_attribute('Elevation',attributes, math.radians)
            apply_attribute('Roll',attributes, math.radians)
        elif self.class_ == 'AxisRotated':
            apply_attribute('Angle',attributes, math.radians)
        if self.class_ == 'BezierCurve2D' or self.class_ == 'NurbsCurve2D':
            ctrlPointList = attributes['ctrlPointList']
            def todim3(pt):
                if len(pt) == 2 : return (pt[0], pt[1], 1)
            attributes['ctrlPointList'] = map(todim3, ctrlPointList)
        if self.class_ == 'BezierCurve' or self.class_ == 'NurbsCurve':
            ctrlPointList = attributes['ctrlPointList']
            def todim4(pt):
                if len(pt) == 3 : return (pt[0], pt[1], pt[2], 1)
            attributes['ctrlPointList'] = map(todim4, ctrlPointList)
        if self.class_ == 'BezierCurve2D' or self.class_ == 'BezierCurve':
            if 'degree' in attributes:
                assert attributes['degree'] == len(attributes['ctrlPointList'])-1
                del attributes['degree']

    def customcreate(self, objmap):
        return getattr(self,self.class_)(objmap)

    def Transfo(self, objmap):
        import openalea.plantgl.all as pgl
        import math
        m = pgl.Matrix4()
        print self.attributes
        def fixtransfoname(t):
            if t == 'Scale' : return 'Scaling'
            else : return t

        for t,p in self.attributes.items():
            print '*',t,p
            if t == 'EulerRotation':
                p = parse_attributes(p, {})
                p = (p.get('Elevation',0.),p.get('Azimuth',0.),p.get('Roll',0.))
                p = map(lambda v : math.radians(float(v)), p) 
                transf = pgl.EulerRotation(*p)
            else:
                transf = pgl.__dict__[fixtransfoname(t)]( p)
            m *= transf.getMatrix()
        return pgl.Transform4(m)

    def Shape(self, objmap):
        if type(self.attributes) == list:
            return pgl.__dict__[self.class_](*self.attributes)
        else:
            return self.stdcreate(objmap)


def parse_attributes(cmds, objmap):
    assert cmds[0] == '{' and cmds[-1] == '}'
    print cmds
    state = 0
    attributes = {}
    value = []
    cname = None
    currentid = 0
    cmds = cmds[1:-1]
    if ',' in cmds : # The shape style
        return parse_objectlist(cmds, objmap)
    else:
        while currentid < len(cmds):
            att = cmds[currentid]
            if state == 0:
                cname = att
                state = 1
                currentid += 1
            else :
                if type(att) == str and is_pgl_class(att):
                    currentid, att = parse_object(cmds, currentid, objmap)
                elif type(att) == list and type(att[0]) == str and is_pgl_class(att[0]):
                    print att
                    att = parse_objectlist(att, objmap)
                    currentid += 1
                else:
                    currentid += 1
                attributes[cname] = att
                state = 0
        assert state == 0
        return attributes


def parse_object(cmds, currentid, objmap):
    currentobj = PglObj()
    state = 0
    while currentid < len(cmds):
        cmd = cmds[currentid]
        if state == 0:
            if not is_pgl_class(cmd) : raise ValueError(cmd)
            currentobj.class_ = cmd
            state = 1
            currentid += 1
        elif state == 1:
            if type(cmd) == str:
                currentobj.name = cmd
                state = 2
                currentid += 1
                continue
            else:
                state = 2
        if state == 2:
            print currentobj.class_, currentobj.name
            assert type(cmd) == list
            currentobj.attributes = parse_attributes(cmd, objmap)
            result = currentobj.create(objmap)
            if currentobj.name : objmap[currentobj.name] = result
            return currentid+1, result

def parse_objectlist(cmds, objmap):
    objects = []
    currentid = 0
    while currentid < len(cmds):
        if cmds[currentid] != ',':
            currentid, obj = parse_object(cmds, currentid, objmap)
            objects.append(obj)
        else:
            currentid += 1
    return objects

def parse(cmds):
    import openalea.plantgl.all as pgl
 
    objmap = {'Black' : pgl.Color3.BLACK, 'White' : pgl.Color3.WHITE, 'Red' : pgl.Color3.RED, 'Green' : pgl.Color3.GREEN, 'Blue' : pgl.Color3.BLUE, 'Cyan' : pgl.Color3.CYAN, 'Magenta' : pgl.Color3(255,0,255), 'Yellow' : pgl.Color3.YELLOW }
    shapes = []
    
    currentid = 0
    while currentid < len(cmds):
        currentid, obj = parse_object(cmds, currentid, objmap)
        if type(obj) == pgl.Shape:
            shapes.append(obj)

    if len(shapes) > 0 : return pgl.Scene(shapes)
    else : return pgl.Scene([pgl.Shape(obj) for obj in objmap if isinstance(obj, pgl.Geometry)])



def parse_file(fname):
    txt = read(fname, {})
    tree = lex(txt)
    print tree
    return parse(tree)



if __name__ == '__main__':
    import glob
    testfiles = glob.glob('../../../share/plantgl/database/exemples/*.geom')
    for f in testfiles:
        print f
        parse_file(f)

    from openalea.plantgl.all import Viewer
    Viewer.display(parse_file('../../../share/plantgl/database/exemples/gen_cylinder.geom'))
    #parse_file('../../../share/plantgl/database/exemples/colors.app')    