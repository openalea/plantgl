from openalea.core import *
from objectgenerator import PGLCLASS
 
def generate_factory(name, **kargs):
	return Factory( name= name, 
                  description= "Create a "+name, 
                  category = "PGL Object Generator", 
                  nodemodule = "objectgenerator",
                  nodeclass = name+'Node',
		  **kargs) 

def define_package(package):
    """ Initialisation function
    Return a list of package to include in the package manager.
    This function is called by the package manager when it is updated
    """
    nf = generate_factory("Scene", lazy=False)
    package.add_factory( nf )
    map(lambda x : package.add_factory(generate_factory(x.__name__)),PGLCLASS)

