from openalea.plantgl.scenegraph import Scene
from glob import glob

def pgl2pov():
    import sys, os
    skipnext = False
    for i,fnamepattern in enumerate(sys.argv[1:],1):
        if skipnext:
            skipnext = False
        elif os.path.exists(fnamepattern):
            if len(sys.argv) > i+1 and os.path.splitext(sys.argv[i+1])[1] == '.pov':
                output = sys.argv[i+1]  
                skipnext = True
            else:
                output = os.path.splitext(fnamepattern)[0]+'.pov'
            s = Scene(fnamepattern)
            if len(s) > 0:
                print('Convert',repr(fnamepattern),'into',repr(output))
                s.save(output,'POV')
        else:
            for fname in glob(fnamepattern):
                output = os.path.splitext(fname)[0]+'.pov'
                s = Scene(fname)
                if len(s) > 0:
                    print('Convert',repr(fname),'into',repr(output))
                    s.save(output,'POV')
