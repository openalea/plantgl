from openalea.plantgl.scenegraph import Scene

def pgl2pov():
    import sys, os
    pglfname = sys.argv[1]
    output = sys.argv[2] if len(sys.argv) >= 3 else os.path.splitext(pglfname)[0]+'.pov'
    s = Scene(pglfname)
    s.save(output,'POV')