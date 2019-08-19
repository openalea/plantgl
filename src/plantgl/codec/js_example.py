import random

def js_wrap(js, js_libraries):
    """
    Wrap javascript commands in code that preloads needed libraries
    """
    lines = ['function getMultipleScripts(scripts, callback) {',
             '  if(scripts.length == 0)',
             '    callback();',
             '  jQuery.getScript(scripts[0], function () {',
             '    getMultipleScripts(scripts.slice(1), callback);',
             '  });',
             '}',
             'var scripts = [']
    lines += ['"%s",' % script for script in js_libraries]
    lines += ['];',
              'getMultipleScripts(scripts, function() {',
              js,
              '});']
    return '\n'.join(lines)

def polygons2js(polygons):
    # Find the size of the 3D model. We use this later to make the default camera and rotation point work
    xmin, xmax, ymin, ymax, zmin, zmax = polygons.extents()
    length = max([xmax-xmin, ymax-ymin, zmax-zmin])
    xmean = (xmin + xmax)/2
    ymean = (ymin + ymax)/2
    zmean = (zmin + zmax)/2

    # Generate the Javascript code
    # see http://kovacsv.github.io/JSModeler/documentation/tutorial/tutorial.html for details
    canvas_id = 'js3dcanvas_%d' % random.randint(0, 1e10)
    canvas_style = 'style="border: 1px solid black;" width="800" height="600"'
    js = ['var widget = jQuery(\'<canvas id="%s" %s></canvas>\');' % (canvas_id, canvas_style),
          'element.append(widget);',
          'container.show();',
          'var viewerSettings = {',
          '  cameraEyePosition : [-2.0, -1.5, 1.0],',
          '  cameraCenterPosition : [0.0, 0.0, 0.0],',
          '  cameraUpVector : [0.0, 0.0, 1.0]',
          '};',
          'var viewer = new JSM.ThreeViewer();',
          'viewer.Start(widget, viewerSettings);',
          'var body = new JSM.Body();']

    # Add node coordinates
    coords = []
    for coord in polygons.nodes:
        coords.append('[%f, %f, %f]' % ((coord[0]-xmean)/length,
                                        (coord[1]-ymean)/length,
                                        (coord[2]-ymean)/length))
        #print coord, coords[-1]
    js.append('var coords = [%s];' % ', '.join(coords))
    js.append('for (var i = 0, len = coords.length; i < len; i++) {')
    js.append('  body.AddVertex(new JSM.BodyVertex('
              'new JSM.Coord(coords[i][0], coords[i][1], coords[i][2])));')
    js.append('}')

    # Add elements
    polys = [repr(list(poly)) for poly in polygons.polygons]
    js.append('var elems = [%s];' % ', '.join(polys))
    js.append('for (var i = 0, len = elems.length; i < len; i++) {')
    js.append('  body.AddPolygon(new JSM.BodyPolygon(elems[i]));')
    js.append('}')
    js.append('var meshes = JSM.ConvertBodyToThreeMeshes(body);')
    js.append('for (var i = 0; i < meshes.length; i++) { viewer.AddMesh (meshes[i]); }')
    js.append('viewer.Draw();')

    # Wrap final js code in a library loader to make sure JSModeler and three.js are available
    js = '\t'+'\n\t'.join(js)+'\n'
    libraries =  ['https://rawgit.com/kovacsv/JSModeler/master/build/lib/three.min.js',
                  'https://rawgit.com/kovacsv/JSModeler/master/build/jsmodeler.js',
                  'https://rawgit.com/kovacsv/JSModeler/master/build/jsmodeler.ext.three.js']
    return js_wrap(js, libraries)

class PolygonCollection(object):
    def __init__(self, nodes, polygons):
        """
        A collection of plane polygons in 3D

        Nodes is a list of 3-tuples of coordinates
        Polygons is a nested list of node indices for each polygon, one
        list of indices for each polygon
        """
        self.nodes = nodes
        self.polygons = polygons

    def extents(self):
        """
        Calculate the size of the 3D polygon model
        """
        xmin, ymin, zmin = 1e100, 1e100, 1e100
        xmax, ymax, zmax = -1e100, -1e100, -1e100
        for x, y, z in self.nodes:
            xmin = min(x, xmin)
            ymin = min(y, ymin)
            zmin = min(z, zmin)
            xmax = max(x, xmax)
            ymax = max(y, ymax)
            zmax = max(z, zmax)
        return xmin, xmax, ymin, ymax, zmin, zmax
