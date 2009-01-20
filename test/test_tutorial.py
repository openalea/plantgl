

from openalea.core.pkgmanager import PackageManager
import os, sys
from PyQt4 import QtGui

def test_tutorial():
    #app = QtGui.QApplication(sys.argv)
    
    path = os.path.join( os.path.abspath(os.path.dirname(__file__)), "tutorial_standbuilder")
    image = os.path.join(path, "stand.csv.png")
    try:
        os.remove(image)
    except:
        pass


    pm = PackageManager()
    pm.init()

    pm.load_directory(path)
    n = pm.get_node("tutorial_standbuilder", "gen_images")

    print path
    n.actor(4).set_input(0, path)

# the following call create a segmentation fault
    #assert False
    #n()
    #assert os.path.exists(image)
    

