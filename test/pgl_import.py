import os
import sys

exists = os.path.exists

def addbinpath(dir_name):
	if not exists(dir_name):
		raise "'"+dir_name+"' does not exist."
	dir_name = os.path.abspath(dir_name)
	os.environ['PATH'] = dir_name+';'+os.environ['PATH']

def addpybinpath(dir_name):
	if not exists(dir_name):
		raise "'"+dir_name+"' does not exist."
	dir_name = os.path.abspath(dir_name)
	sys.path.insert(0,dir_name)

if exists('../bin'):
	"Visual 8 Binary path added"
	addbinpath('../bin')
else:
	"'build_scons' Binary path added"
	addbinpath('../build-scons/bin')
	addbinpath('../build-scons/lib')

addpybinpath('../src')

import openalea.plantgl.all as pgl

v = pgl.Vector3.ORIGIN
print('Vector3 : '+str(v))
c =pgl.Cylinder()
print(c)
d = pgl.Discretizer()
print(d)




