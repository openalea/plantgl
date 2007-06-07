from openalea.core import *
 
 
def generate_factory(name, **kargs):
	return Factory( name= name, 
                  description= "make "+name, 
                  category = "PGL Object Generator", 
                  nodemodule = "objectgenerator",
                  nodeclass = name,
		  **kargs
) 

def define_package(package):
    """ Initialisation function
    Return a list of package to include in the package manager.
    This function is called by the package manager when it is updated
    """
    nf = generate_factory("Material")
    package.add_factory( nf )
    nf = generate_factory("Shape")
    package.add_factory( nf )
    nf = generate_factory("Scene", lazy=False)
    package.add_factory( nf )
    nf = generate_factory("AddToScene")
    package.add_factory( nf )
    nf = generate_factory("Cone")
    package.add_factory( nf )
    nf = generate_factory("Cylinder")
    package.add_factory( nf )
    nf = generate_factory("Frustum")
    package.add_factory( nf )
    nf = generate_factory("Paraboloid")
    package.add_factory( nf )
    nf = generate_factory("Translated")
    package.add_factory( nf )
    nf = generate_factory("EulerRotated")
    package.add_factory( nf )
    nf = generate_factory("Sphere")
    package.add_factory( nf )

