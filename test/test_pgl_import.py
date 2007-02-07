import os
import sys

exists = os.path.exists

def addbinpath(dir):
	if not exists(dir):
		raise "'"+dir+"' does not exist."
	dir = os.path.abspath(dir)
	os.environ['PATH'] += ';'+dir

def addpybinpath(dir):
	if not exists(dir):
		raise "'"+dir+"' does not exist."
	dir = os.path.abspath(dir)
	sys.path.append(dir)

if exists('../bin'):
	"Visual 8 Binary path added"
	addbinpath('../bin')
else:
	"'build_scons' Binary path added"
	addbinpath('../build_scons/bin')
	addbinpath('../build_scons/lib')

addpybinpath('../src')

import plantgl as pgl

v = pgl.math.Vector3.ORIGIN
print('Vector3 : '+str(v))
pgl.gui.Viewer.start()


raw_input("Appuyer sur une touche")




