from openalea.plantgl.all import Material, Geometry, Shape, Tesselator, BoundingBox, norm


    
template_code_begin = """
    var engine = new BABYLON.Engine(canvas, true);

    var createScene = function(){{
        var scene = new BABYLON.Scene(engine);
        scene.clearColor = new BABYLON.Color3(0.8, 0.8, 0.8);

        var camera = new BABYLON.ArcRotateCamera("Camera", 0, 0, {objsize}, new BABYLON.Vector3{objcenter}, scene);
        var distance = Math.abs( {objsize} / Math.sin( camera.fov / 2 ) );
        camera.setPosition(new BABYLON.Vector3(0, 0, distance));
        camera.attachControl(canvas, true);

        var light = new BABYLON.HemisphericLight("light1", new BABYLON.Vector3{lightposition}, scene);
        light.intensity = 1;

"""


template_code_end = """
        return scene;
    };

    var scene = createScene(); // Call the createScene function

    // Register a render loop to repeatedly render the scene
    engine.runRenderLoop(function () { 
            scene.render();
    });

    // Watch for browser/canvas resize events
    window.addEventListener("resize", function () { 
            engine.resize();
    });
"""

def col32js(col):
    return 'new BABYLON.Color3(%f, %f, %f)' % (col.clampedRed(),col.clampedGreen(),col.clampedBlue())

def mat2js(mat):
    check_name(mat) 
    code = """
        var mat = new BABYLON.StandardMaterial(\"""" +mat.name+"""\", scene);
        mat.ambientColor = """ + col32js(mat.ambient)+ """;
        mat.diffuseColor = """ + col32js(mat.diffuseColor())+ """;
        mat.specularColor = """ + col32js(mat.specular)+ """;
        mat.emissiveColor = """ + col32js(mat.emission)+ """;
        mat.backFaceCulling = false;

    """
    return code

def isiterable(obj):
    try:
        iter(obj)
        return True
    except TypeError:
        return False

def _array2js(a):
    if isiterable(a):
        return ','.join(map(_array2js,a))
    else:
        return str(a)

def array2js(a):
    return '[ '+ _array2js(a) + ' ]'

def check_name(obj):
    if obj.name == '':
        obj.name = 'obj_'+str(obj.getId())

def mesh2js(mesh):
    check_name(mesh) 
    code = """
        var customMesh = new BABYLON.Mesh(\""""+mesh.name+"""\", scene);

        var vertexData = new BABYLON.VertexData();

        vertexData.positions = """ + array2js(mesh.pointList)+ """;
        vertexData.indices = """ + array2js(mesh.indexList)+ """;

        vertexData.applyToMesh(customMesh);

        customMesh.material = mat;
    """
    return code

def sh2js(sh):
    code = ''
    app = sh.appearance
    if isinstance(app, Material):
        code += mat2js(app)
    else:
        code += mat2js(Material.DEFAULT_MATERIAL)
    t = Tesselator()
    sh.geometry.apply(t)
    code += mesh2js(t.result)
    return code


def to_js(scene):
    bbx = BoundingBox(scene)
    center = bbx.getCenter()
    objsize = norm(bbx.getSize())
    code = template_code_begin.format(objcenter=(center.x, center.y, center.z), objsize=objsize, lightposition = tuple(center+(0,0,5*objsize)))
    if isinstance(scene,Geometry):
        code += sh2js(Shape(scene,Material.DEFAULT_MATERIAL))
    elif isinstance(scene, Shape):
        code += sh2js(scene)
    else:
        for sh in scene:
            code += sh2js(sh)
    code += template_code_end
    return code

jslibs = ["https://cdn.babylonjs.com/babylon.js",
        "https://preview.babylonjs.com/loaders/babylonjs.loaders.min.js",
        "https://code.jquery.com/pep/0.4.3/pep.js"]

import os.path
#jslibs_static = [os.path.join(os.path.dirname(__file__),'babylon.custom.js')]
jslibs_static = ['./babylon.custom.js']

def get_jslibs():
    return jslibs_static

template_html = """
<!doctype html>
<html lang="en">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
        <title>PlantGL Plot</title>
        %s
    </head>
    
    <body>     
    <canvas id="renderCanvas"> </canvas>
    <script>
    canvas = document.getElementById("renderCanvas");
    canvas.width = document.body.clientWidth; //document.width is obsolete
    canvas.height = document.body.clientHeight; //document.height is obsolete

    function resizeCanvas() {
      canvas.style.width = window.innerWidth + "px";
      setTimeout(function() {
        canvas.style.height = window.innerHeight + "px";
      }, 0);
    };

    window.onresize = resizeCanvas;
    resizeCanvas();

    %s

    </script>
    </body>
</html>
"""

def generate_html(scene):
    scripts = '\n'.join(map(lambda l : '\t\t<script src="%s"></script' % l, get_jslibs()))
    return template_html % (scripts, to_js(scene))

def plot_js(scene):
    import tempfile, os
    handle, fname = tempfile.mkstemp(suffix='.html', text=True)
    file = os.fdopen(handle, 'w')
    file.write(generate_html(scene))
    print fname
    import webbrowser
    webbrowser.open(fname)
    return fname

canvas_jquery_creation = """
    var canvas = jQuery('<canvas id="%s" style="border: 1px solid black;" width="800" height="600"></canvas>');
    element.append(canvas);
    container.show();
"""


def ipy_plot(scene):
    from IPython.display import Javascript, display
    canvascode = canvas_jquery_creation % ('canvas_'+str(scene.getId()))
    j = Javascript(canvascode+to_js(scene),lib=get_jslibs())
    display(j)

