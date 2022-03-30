import openalea.plantgl.scenegraph as sg
import openalea.plantgl.algo.jsonrep as jr
import json

class JsonCodec (sg.SceneCodec):
    """ Json File Format """

    def __init__(self):
        """
        Initialisation of the codec info
        """
        sg.SceneCodec.__init__(self,"JSON",sg.SceneCodec.Mode.ReadWrite)

    def formats(self):
        """ return formats """
        return [ sg.SceneFormat("Json Codec",["json"],"The json format") ]

    def read(self,fname):
        """ read a json file """
        with open(fname, 'r') as json_file:
            json_schema = json.load(json_file)
            return jr.from_json_rep(json_schema)

    def write(self, fname, scene):
        """ Write a JSON file from a plantGL scene graph.

        This method will convert a PlantGL scene graph into an JSON file.
        """
        with open(fname, 'w') as json_file:
            json.dump(jr.to_json_rep(scene), json_file)

codec = JsonCodec()
sg.SceneFactory.get().registerCodec(codec)   