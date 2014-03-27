import platform

system = platform.system()
if system == "Linux":
    name, version, id = platform.dist()
    
    QTDIR="/usr"
    QT_VERSION=4
    if name == 'fedora' and \
        (version =='17' or version =='19'):
        ann_libs='ANN'
    boost_libs_suffix='-mt'