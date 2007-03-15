from openalea.core import *
import objectgenerator_factory as ogf
 
def register_packages(pkgmanager):
    """ Initialisation function
    Return a list of package to include in the package manager.
    This function is called by the package manager when it is updated
    """
    metainfo={ 'version' : '0.0.1',
               'license' : 'CECILL-C',
               'authors' : 'F. Boudon',
               'institutes' : 'INRIA/CIRAD',
               'description' : 'PGL Object Generator nodes.',
               'url' : 'http://openalea.gforge.inria.fr'
               }
 
    package = Package("PlantGL Objects", metainfo)
    ogf.define_package(package)
    pkgmanager.add_package(package)	
	
    metainfo={ 'version' : '0.0.1',
               'license' : 'CECILL-C',
               'authors' : 'F. Boudon',
               'institutes' : 'INRIA/CIRAD',
               'description' : 'Vizualisation nodes.',
               'url' : 'http://openalea.gforge.inria.fr'
               }
 
 
    package = Package("PlantGL Visualization", metainfo)
    
    nf = Factory( name= "viewerDisplay", 
                  description= "Viewer Display", 
                  category = "Vizualisation", 
                  nodemodule = "viewernode",
                  nodeclass = "ViewerStart",
                  
                  widgetmodule = None,
                  widgetclass = None,
                 
                  )
    package.add_factory( nf )
    
    nf = Factory( name= "plot3D", 
                  description= "Viewer Display", 
                  category = "Vizualisation", 
                  nodemodule = "viewernode",
                  nodeclass = "Plot3D",
                  
                  widgetmodule = None,
                  widgetclass = None,
                 
                  )
    package.add_factory( nf )

    pkgmanager.add_package(package)