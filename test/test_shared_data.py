

from openalea.plantgl.all import get_shared_data
import os


def test_shared_data():
    assert os.path.isfile(get_shared_data('amapsymbols/feumod.smb')) == True
