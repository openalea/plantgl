import platform

system = platform.system()
if system is "Linux":
    name, version, id = plaform.dist()
    
    QTDIR="/usr"
    QT_VERSION=4
    if name == 'fedora' and version =='17':
        ann_libs='ANN'
	boost_libs_suffix='-mt'
