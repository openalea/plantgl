
import sys, os

if('win32' in sys.platform):   
    #compiler = 'msvc'
    compiler = 'mingw'

elif('posix' in os.name):
    QTDIR="/usr"
    QT_VERSION=4
    #WITH_CGAL = True

