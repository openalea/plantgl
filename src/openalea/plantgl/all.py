from .math import *
from .scenegraph import *
from .algo import *
if not pgl_support_extension('PGL_NO_QT_GUI'):
    from .gui import *

from . import codec

from os.path import join as pj


def get_shared_data(file, share_path=pj('share','plantgl', 'database')):
    import os
    from os.path import pardir, exists, abspath
    import openalea.plantgl
    for prefix in ['PREFIX', 'LIBRARY_PREFIX', 'CONDA_PREFIX']:
        if prefix in os.environ:
            shared_data_path = pj(os.environ[prefix], share_path)
            break
    else:
        try:
            from openalea.deploy.shared_data import get_shared_data_path
            shared_data_path = get_shared_data_path(openalea.plantgl.__path__, share_path=share_path)
        except ModuleNotFoundError as me:
            # Standard installation
            shared_data_path = pj(openalea.plantgl.__path__[0], pardir, pardir, share_path )
            if not exists(shared_data_path):
                # Develop installation
                shared_data_path = pj(openalea.plantgl.__path__[0], pardir, pardir, pardir, share_path )
    return abspath(pj(shared_data_path, file))


