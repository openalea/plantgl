import openalea.plantgl.math as mt
import openalea.plantgl.scenegraph as sg
import openalea.plantgl.algo as alg
import sys

class PlyCodec (sg.SceneCodec):
    """ Ply Point File Format 

    """

    def __init__(self):
        """
        Initialisation of the codec info
        """
        sg.SceneCodec.__init__(self,"PLY",sg.SceneCodec.Mode.Read)

    def formats(self):
        """ return formats """
        return [ sg.SceneFormat("Ply Codec",["ply"],"The ply format") ]

    def read(self,fname):
        """ read an ascii point file """
        
        import warnings
        import struct

        stream = file(fname,"r")
        header = stream.readline()
        assert header.strip() == 'ply'
        def nextline():
            line = stream.readline()
            while line.startswith('comment'):
                line = stream.readline()
            return line.strip()

        format = nextline()
        fkeyword, fcoding, fversion = format.split()
        fversion = float(fversion)
        assert fcoding in ['binary_little_endian','binary_big_endian','ascii']
        if fcoding != 'ascii':
            reversebytes = (fcoding.split('_')[1] != sys.byteorder)


        nline = nextline()
        spec = []
        asciitypes = { 'float' : float ,'int' : int , 'uchar' :int, 'char' : int, 'short' : int, 'ushort' : int, 'double' : float }
        unpacktypes = { 'float' : 'f' ,'int' : 'i', 'uchar' : 'B', 'char' : 'b', 'short' : 'h', 'ushort' : 'H', 'double' : 'd' }
        while nline != 'end_header':
            if nline.startswith('element'):
                elementkwd, elementname, elementnb = nline.split()
                spec.append((elementname, int(elementnb), []))
            elif nline.startswith('property'):
                propval = nline.split()
                if len(propval) == 3:
                    propkwd, proptype, propname = propval
                    spec[-1][2].append((propname,proptype))
                    assert proptype in unpacktypes
                elif len(propval) == 5:
                    propkwd, proplist, propsizetype, proptype, propname = propval
                    spec[-1][2].append((propname,(propsizetype,proptype)))
            nline = nextline()
        colorprops = []
        knowncolortypes = ['red','green','blue','diffuse_red','diffuse_green','diffuse_blue','alpha']
        if spec[0][0] == 'vertex':
            for propname, proptype in spec[0][2]:
                if propname in knowncolortypes:
                    colorprops .append(propname)
        if len(colorprops) == 0 : colorprops = None
        else : 
            colorprops.sort(cmp=lambda c1,c2:cmp(knowncolortypes.index(c1),knowncolortypes.index(c2)))
            assert len(colorprops) in [3,4]

        points = []
        colors = []
        faces = []
        for element in spec:
            elemname, elementnb, elemspec = element
            for ielem in xrange(elementnb):
                ielement = {}
                if fcoding == 'ascii':
                    linevalues = nextline().split()
                    itv = 0
                    for elemprop in elemspec:
                        propname,proptype = elemprop
                        if type(proptype) == tuple:
                            propsizetype, proptype = proptype
                            tr = asciitypes[propsizetype]
                            nbpropv = tr(linevalues[itv]) ; itv += 1
                            value = []
                            tr = asciitypes[proptype]
                            for ival in xrange(nbpropv):
                                value.append(tr(linevalues[itv])); itv += 1
                        else:
                            tr = asciitypes[proptype]
                            value = tr(linevalues[itv]); itv += 1
                        ielement[propname] = value
                else:
                    def readnextval(stream, ptype):
                        flag = unpacktypes[ptype]
                        psize = struct.calcsize(flag)
                        val = stream.read(psize)
                        if reversebytes:
                            val = ''.join(list(reversed(val)))
                        return struct.unpack(flag, val)[0]

                    for elemprop in elemspec:
                        propname,proptype = elemprop
                        if type(proptype) == tuple:
                            propsizetype, proptype = proptype
                            nbpropv = readnextval(stream, propsizetype)
                            value = []
                            for ival in xrange(nbpropv):
                                value.append(readnextval(stream, proptype))
                        else:
                            value = readnextval(stream, proptype)
                        ielement[propname] = value
                if elemname == 'vertex':
                    points.append((ielement['x'],ielement['y'],ielement['z']))
                    if colorprops:
                        val = [ielement[cni] for cni in colorprops]
                        if colorprops[3] == 'alpha':
                            val[3] = 255 - val[3]
                        else:
                            val.append(0)
                        colors.append(tuple(val))
                elif elemname == 'face':
                    faces.append(ielement['vertex_indices'])
        if colors == []: colors = None
        if len(faces) == 0:
            return sg.Scene([sg.Shape(sg.PointSet(points, colors))])
        else:
            return sg.Scene([sg.Shape(sg.FaceSet(points, faces, colorList=colors))])

#codec = PlyCodec()
#sg.SceneFactory.get().registerCodec(codec)