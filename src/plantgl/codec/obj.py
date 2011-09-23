# -*- utf-8 -*-
#
#       VPlants.PlantGL
#
#       Copyright 2011-2011 INRIA - CIRAD - INRA
#
#       File author(s): Christophe Pradal <christophe.pradal@cirad.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
#
#       OpenAlea WebSite: http://openalea.gforge.inria.fr
#
###############################################################################
""" `OBJ`_ codec for PlantGL

This module provide a codec for OBJ file format.
`OBJ`_ is a file format for 3D geometry defined by the Wavefront company.

This codec allow to read and write `OBJ`_ file format. 

.. _OBJ: http://en.wikipedia.org/wiki/Wavefront_.obj_file
"""

__license__ = "Cecill-C"
__revision__ = " $Id: actor.py 2242 2010-02-08 17:03:26Z cokelaer $ "

import warnings
import openalea.plantgl.math as mt
import openalea.plantgl.scenegraph as sg
import openalea.plantgl.algo as alg
from random import randint
#from openalea.plantgl.ext import color

def random_material(): 
    r, g, b = randint(0,255), randint(0,255), randint(0,255)
    return sg.Material(sg.Color3(r, g, b))

class Group(object):
    def __init__(self, name):
        self.name = name
        self.vindex = []
        self.nindex = []
        self.tindex = []
        self.material = sg.Material()

    def add_face(self, vindex, nindex=None, tindex=None):
        vi = self.vindex 
        vi.append(vindex)
            
        if nindex:
            ni = self.nindex 
            ni.append(nindex)
        if tindex:
            ti = self.tindex 
            ti.append(tindex)


    def shape(self, vertices, normals, textures):
        # Create tset, qset and FaceSet
        # TODO: Add QuadSet
        pointList = vertices
        indexList = self.vindex
        normalList = normals if normals else None
        normalIndexList = self.nindex if self.nindex else None
        textCoordList = textures if textures else None
        textCoordIndexList = self.tindex if self.tindex else None

        tset = None
        try:
            if normalIndexList and textCoordIndexList:
                tset = sg.FaceSet(pointList=pointList, indexList=indexList,
                normalList=normalList, normalIndexList=normalIndexList,
                textCoordList=textCoordList, textCoordIndexList=textCoordIndexList)
            elif normalIndexList:
                tset = sg.FaceSet(pointList=pointList, indexList=indexList,
                normalList=normalList, normalIndexList=normalIndexList)
            else:
                tset = sg.FaceSet(pointList=pointList, indexList=indexList)
        except:
            return 

        _shape =  sg.Shape(tset, self.material)
        _shape.name = self.name
        return _shape

    def __len__(self):
        return len(self.vindex) 

class ObjCodec (sg.SceneCodec):
    """ OBJ File Format 

    The OBJ file format is a simple data-format that represents 3D geometry alone: 
        - the position of each vertex, 
        - the UV position of each texture coordinate vertex, 
        - normals, 
        - the faces that make each polygon defined as a list of vertices, 
        - and texture vertices. 

    File example::

        # List of Vertices, with (x,y,z[,w]) coordinates, w is optional.
        v 0.123 0.234 0.345 1.0
        ...
        
        # Texture coordinates, in (u,v[,w]) coordinates, w is optional.
        vt 0.500 -1.352 [0.234]
        ...
        
        # Normals in (x,y,z) form; normals might not be unit.
        vn 0.707 0.000 0.707
        vn ...
        
        # Face Definitions (see below)
        f 1 2 3
        f 3/1 4/2 5/3
        f 6/4/1 3/5/3 7/6/5
        f ...
    """
    
    def __init__(self):
        """
        Initialisation of the codec info
        """
        sg.SceneCodec.__init__(self,"OBJ",sg.SceneCodec.Mode.Read)

    def formats(self):
        """ return formats """
        return [ sg.SceneFormat("Obj Codec",["obj"],"The Obj  file format") ]


    def read(self,fname):
        """ read an obj file """

        parser = {}
        parser['#'] = parser[''] = self._comment
        parser['v'] = self._vertex
        parser['vt'] = self._vertex_texture
        parser['vn'] = self._vertex_normal
        parser['f'] = self._face
        parser['mtllib'] = self._material
        parser['o'] = self._object_name
        parser['g'] = self._group_name
        parser['usemtl'] = self._material_name

        # define data
        self.vertices = []
        self.normals = []
        self.textures= []
        self.groups = [Group('')]
        self.current_group = self.groups[0]
        self.current_material = random_material()
        self.line = 0


        # read the obj file
        with open(fname,"r") as f:
            for l in f:
                l = l.strip()
                fields = l.split()
                if not fields or len(fields)<2:
                    continue
                key = fields[0]
                if key in parser:
                    parser[key](fields[1:])
                else:
                    warnings.warn("Type "+key+" in file '"+fname+"' is not take into account")
                self.line += 1

        # Build the scene
        scene = sg.Scene()
        for g in self.groups:
            if g:
                s = g.shape(self.vertices, self.normals, self.textures)
                if s and s.geometry: scene.add(s)

        return scene

    def read_material(self, fname):
        """ Read mtl format, and convert it into PlantGL materials.

        """
        pass

    # Parser functions
    def _comment(self, args):
        pass

    def _vertex(self, args):
        x, y, z = map(float, args)[:3]
        self.vertices.append((x,y,z))

    def _vertex_texture(self, args):
        l = map(float,args)
        self.textures.append(l[:2])

    def _vertex_normal(self, args):
        """ Parse a vertex normal."""
        x, y, z = map(float, args)
        self.normals.append((x,y,z))

    def _face(self, args):
        """ Parse a face set."""
        arg = args[0]
        indices = normals = texts = None
        case = 'vertex' # f v1 v2 v3
        if '//' in arg:
            case ="vertex/normal"
            l = [s.split('//') for s in args]
            indices = tuple(int(v[0]) -1 for v in l)
            normals = tuple(int(v[1])-1 for v in l)
        elif arg.count('/') == 1:
            case ="vertex/texture"
            l = [s.split('/') for s in args]
            indices = tuple(int(v[0]) -1 for v in l)
            texts= tuple(int(v[1])-1 for v in l)
        elif arg.count('/') == 2:
            case ="vertex/texture/normal"
            l = [s.split('/') for s in args]
            indices = tuple(int(v[0]) -1 for v in l)
            texts= tuple(int(v[1])-1 for v in l)
            normals = tuple(int(v[2])-1 for v in l)
        else:
            case = "vertex"
            indices = tuple(int(s)-1 for s in args)

        self.current_group.add_face(vindex=indices, nindex=normals, tindex=texts)

    def _material(self, args):
        # modify the path for local file
        self.read_material(args[0])

    def _object_name(self, args):
        pass

    def _group_name(self, args):
        name = args[0]
        self.current_group = Group(name)
        self.current_group.material = self.current_material
        self.groups.append(self.current_group) 

    def _material_name(self, args):
        name = args[0]
        self.current_material = random_material()
        self.current_material.name = name
        self.current_group.material = self.current_material


    #############################################################################
    #############################################################################
    # PlantGL -> OBJ codec
    def write(self,fname,scene):
        """ write an ascii point file """
        print("Write "+fname)
        d = alg.Discretizer()
        f = file(fname,'w')

        for i in scene:
            if i.apply(d):
                p = d.discretization
                if isinstance(p,sg.PointSet) :
                    hasColor = not p.colorList is  None and len(p.colorList) > 0
                    col = i.appearance.ambient
                    for i,pt in enumerate(p.pointList):
                        f.write(str(pt.x)+' '+str(pt.y)+' '+str(pt.z)+' ')                        
                        if not isxyz and not ispwnfile:
                            if hasColor:                          
                                col = p.colorList[i]
                            if isptsfile:
                                f.write(str(rgb2intensity(col)))
                            f.write(str(col.red)+' '+str(col.green)+' '+str(col.blue)+'\n')
                        else: f.write('\n')
        f.close()
    

codec = ObjCodec()
sg.SceneFactory.get().registerCodec(codec)
