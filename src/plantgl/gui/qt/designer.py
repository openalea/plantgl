#!/usr/bin/python
# -*- coding: utf-8 -*-

#This file is part of pyLot library.
#
# pyLot is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# pyLot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with pyLot.  If not, see <http://www.gnu.org/licenses/>.

__author__ = 'Pierre Puiseux, Guillaume Baty'
__copyright__ = "Copyright 2011-2012 (C) andheo, Université de Pau et des Pays de l'Adour"
__credits__ = ['Pierre Puiseux', 'Guillaume Baty']
__license__ = "GNU Lesser General Public License"

__all__ = [
    'generate_pyfile_from_uifile',
    'compile_ui_files'
]

import datetime
import pkg_resources
import sys
import os

from openalea.core.path import path as Path
from openalea.vpltk.qt import QT_MODULE_NAME


FORCE_UI_GENERATION = False


def get_data(name, path):
    """
    Eases access to module packed data.

    This function return an absolute path using
    :obj:`pkg_resources.resource_filename` function.

    :param name: __name__
    :param path: path relative to module "name"
    """
    path = pkg_resources.resource_filename(name, path)
    path = Path(path).abspath()
    return path


def mtime(path):
    """Last-modified time of the file."""
    return os.path.getmtime(path)


def mtime_datetime(path):
    return datetime.datetime.fromtimestamp(mtime(path))


def replaceext(self, ext, old_ext=None):
    """
    Changes current extension to ext.
    If extension contains more than one dot (ex: .tar.gz) you can specify
    it with "old_ext" argument.

    >>> from pyLot.core import Path
    >>> p = Path(u'path.py')
    >>> p.replaceext(u'.rst')
    Path(u'path.rst')

    >>> p = Path(u'archive.tar.gz')
    >>> p.replaceext(u'.tgz')
    Path(u'archive.tar.tgz')
    >>> p.replaceext(u'.tgz', u'.tar.gz')
    Path(u'archive.tgz')
    """
    if old_ext is None:
        path, old_ext = self.splitext()
        return self.__class__(path + ext)
    else:
        path_ext = self[-len(old_ext):]
        if path_ext != old_ext:
            raise ValueError('old_ext %(OLD_EXT)r and path ext %(PATH_EXT)r do not match.' % dict(
                OLD_EXT=old_ext, PATH_EXT=path_ext[1:]))
        else:
            return self.__class__(self[:-len(old_ext)] + ext)


from openalea.vpltk.qt.uic import compileUi, compile_args


def generate_pyfile_from_uifile(name, src=None, dest=None, uibasename=None, force=None):
    """
    Function searches ...

    if src is None, search in this order :
      - <moduledir>/designer/<modulename>.ui
      - <moduledir>/resources/<modulename>.ui
      - <moduledir>/<modulename>.ui
    else :
      - src

    File generated is
    if dest is None :
      - _<uifilebase>.py (Ex: mywdget.ui -> _mywidget.py)
    else :
      - dest

    .. warning ::

      To work, this function has to be called in an **imported** module.
      (__name__ must differ from __main__) else, nothing is done !

      Do not edit generated file because all data written here are lost.

    :param name:
    :type name: str

    :return: Qt class (corresponding to filename), Qt type class (type of first value)
    :rtype: couple
    """
    if force is None:
        force = FORCE_UI_GENERATION
    modulename = name
    if uibasename:
        name = uibasename
    else:
        name = name.split('.')[-1]
    if modulename == '__main__':
        return
    paths = []
    if src:
        filepath = Path(src)
        paths.append(filepath)
    else:
        path = 'designer/%s.ui' % name
        filepath = Path(get_data(modulename, path))
        paths.append(filepath)

        path = 'resources/%s.ui' % name
        filepath = Path(get_data(modulename, path))
        paths.append(filepath)

        path = '%s.ui' % name
        filepath = Path(get_data(modulename, path))
        paths.append(filepath)

    for path in paths:
        if path.isfile():
            break

#  tmpdir = mkdtempu()
    if dest is None:
        pyfilename = path.parent / '_' + path.name.replace('.ui', '.py')
    else:
        pyfilename = Path(dest)

    if not pyfilename.exists():
        generate = True
    else:
        mtime_py = mtime_datetime(pyfilename)
        mtime_ui = mtime_datetime(path)
        if mtime_py > mtime_ui:
            # If py file is more recent than ui, check user has not changed QT_API
            with open(pyfilename, 'r') as f:
                content = f.read()
                generate = QT_MODULE_NAME not in content
        else:
            generate = True

    if generate or force:
        module_dir = str(path.parent)
        if module_dir not in sys.path:
            sys.path.append(module_dir)

        if force:
            print('build %s from %s\n' % (pyfilename, path))
        else:
            print('%s has changed, build %s\n' % (path, pyfilename))

        pyfile = open(pyfilename, 'w')
        compileUi(path, pyfile, **compile_args)
        pyfile.close()


def compile_ui_files(module, import_instructions=None):
    """
    Reads recursively all *.py files in root directory looking for
    "generate_pyfile_from_uifile" calls.
    If this call is found, execute it in order to compile ui file.

    import_instructions : python code containing required imports.
    example :

    >>> import_instructions = 'from openalea.vpltk.qt.designer import generate_pyfile_from_uifile\n'

    if None, uses default imports : generate_pyfile_from_uifile, Path, hardbook and get_data
    """
    import ast
    from openalea.core import codegen

    if import_instructions is None:
        import_instructions = "from openalea.vpltk.qt.designer import generate_pyfile_from_uifile\n"

    module = __import__(module)
    paths = []
    for root in module.__path__:
        root = Path(root)
        for py in root.walkfiles('*.py'):
            paths.append((root, py))

    for root, py in paths:
        f = open(py)
        lines = f.readlines()
        f.close()

        code = ''.join(lines)
        try:
            r = ast.parse(code)
        except SyntaxError:
            print('SYNTAX ERROR: cannot read ...', py)
        else:
            for instr in r.body:
                if isinstance(instr, ast.Expr):
                    value = instr.value
                    if isinstance(value, ast.Call):
                        try:
                            func_name = value.func.id
                        except AttributeError:
                            pass
                        else:
                            if func_name == 'generate_pyfile_from_uifile':
                                true = ast.parse('True').body[0]

                                for keyword in value.keywords:
                                    if keyword.arg == 'force':
                                        keyword.value = true
                                        break
                                else:
                                    value.keywords.append(ast.keyword('force', true))
                                src = codegen.to_source(instr)

                                if py.startswith('./') or py.startswith('.\\'):
                                    py = Path(py[2:])
                                name = replaceext(root.parent.relpathto(py), '').replace(os.sep, '.')
                                src = src.replace('__name__', repr(name))
                                try:
                                    code = compile(import_instructions + src, "<string>", "exec")
                                    exec(code)
                                except Exception as e:
                                    print(repr(e))
                                    print('COMPILATION ERROR: cannot compile', py)
                                    print()

# try:
#     from openalea.vpltk.qt import QtCore
#     from openalea.vpltk.qt._test import QtCore as previous_QtCore
#     if QtCore.QObject != previous_QtCore.QObject:
#         raise ImportError
# except ImportError:
#     # First call, ui files are not generated or wrong implementation
#     FORCE_UI_GENERATION = True
#     generate_pyfile_from_uifile('openalea.vpltk.qt', uibasename='test')
