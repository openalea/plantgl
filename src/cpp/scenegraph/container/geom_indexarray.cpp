/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier 
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
 *               
 *  ----------------------------------------------------------------------------
 * 
 *                      GNU General Public Licence
 *           
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */				




#include "geom_indexarray.h"
GEOM_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Index3Array::Index3Array( uint32_t size ) :
  Array1<Index3>(size) {
}

Index3Array::~Index3Array( ) {
}

bool Index3Array::isValid( ) const {
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
    if (! _i->isUnique()) return false;
  return true;
}

uint32_t * Index3Array::data( ) const {
  if(__A.empty())return NULL;
  uint32_t * res = new uint32_t[__A.size()*3];
  size_t _j = 0;
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
    { res[_j] = _i->getAt(0); _j++;
	  res[_j] = _i->getAt(1); _j++; 
	  res[_j] = _i->getAt(2); _j++;  }
  return res;
}

/* ----------------------------------------------------------------------- */


Index4Array::Index4Array( uint32_t size ) :
  Array1<Index4>(size) {
}

Index4Array::~Index4Array( ) {
}

bool Index4Array::isValid( ) const {
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
    if (! _i->isUnique()) return false;
  return true;
}


Index3ArrayPtr Index4Array::triangulate( ) const {
  Index3ArrayPtr _index3List(new Index3Array(__A.size() * 2));
 
  Index3Array::iterator _i3 = _index3List->getBegin();
  for (const_iterator _i4 = __A.begin(); _i4 != __A.end(); _i4++) {
    *(_i3++) = Index3(_i4->getAt(0),_i4->getAt(1),_i4->getAt(2));
    *(_i3++) = Index3(_i4->getAt(0),_i4->getAt(2),_i4->getAt(3));
  };

  return _index3List;
};  

uint32_t * Index4Array::data( ) const {
  if(__A.empty())return NULL;
  uint32_t * res = new uint32_t[__A.size()*4];
  size_t _j = 0;
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
    { res[_j] = _i->getAt(0); _j++;
	  res[_j] = _i->getAt(1); _j++; 
	  res[_j] = _i->getAt(2); _j++; 
	  res[_j] = _i->getAt(3); _j++;  }
  return res;
}

/* ----------------------------------------------------------------------- */


IndexArray::IndexArray( uint32_t size ) :
  Array1<Index>(size,Index(3)) {
}

/** Constructs an IndexArray from an Index3Array.
    \post
    - \e self is valid. */
IndexArray::IndexArray( const Index3Array& array ) :
  Array1<Index>(array.getSize(),Index(3)) {
	  Array1<Index>::iterator it = getBegin();
	  for(Index3Array::const_iterator it2 = array.getBegin(); it2 != array.getEnd(); ++it,++it2)
	  {
		  it->setAt(0,it2->getAt(0));
		  it->setAt(1,it2->getAt(1));
		  it->setAt(2,it2->getAt(2));
	  }
}

/** Constructs an IndexArray from an Index4Array.
    \post
    - \e self is valid. */
IndexArray::IndexArray( const Index4Array& array ) :
  Array1<Index>(array.getSize(),Index(4)) {
	  Array1<Index>::iterator it = getBegin();
	  for(Index4Array::const_iterator it2 = array.getBegin(); it2 != array.getEnd(); ++it,++it2)
	  {
		  it->setAt(0,it2->getAt(0));
		  it->setAt(1,it2->getAt(1));
		  it->setAt(2,it2->getAt(2));
		  it->setAt(3,it2->getAt(3));
	  }
}

IndexArray::~IndexArray( ) {
}

bool IndexArray::isValid( ) const {
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
    if (! _i->isUnique()) return false;
  return true;
}


Index3ArrayPtr IndexArray::triangulate( ) const {
  uint32_t _size = 0;

  for (const_iterator _i4f = __A.begin(); _i4f != __A.end(); _i4f++)
    if(_i4f->getSize() >= 3)_size += _i4f->getSize() - 2;
  
  Index3ArrayPtr _index3List(new Index3Array(_size));
 
  Index3Array::iterator _i3 = _index3List->getBegin();
  for (const_iterator _i4 = __A.begin(); 
		_i4 != __A.end(); 
		_i4++) {
	_size = _i4->getSize();
	if(_size >= 3)
    for (Index::const_iterator _i = _i4->getBegin() + 1; 
	 _i != _i4->getEnd() - 1; 
	 _i++){
      (*_i3) = Index3(_i4->getAt(0), (*_i), (*(_i + 1)));_i3++;
	 }
  };

  return _index3List;
};  

void IndexArray::setAt(uint32_t i, const Index3& t ) {
    Index t2(3);
    t2.setAt(0,t.getAt(0));
    t2.setAt(1,t.getAt(1));
    t2.setAt(2,t.getAt(2));
    Array1<Index>::setAt(i,t2);
}

void IndexArray::setAt(uint32_t i, const Index4& t ) {
    Index t2(4);
    t2.setAt(0,t.getAt(0));
    t2.setAt(1,t.getAt(1));
    t2.setAt(2,t.getAt(2));
    t2.setAt(3,t.getAt(3));
    Array1<Index>::setAt(i,t2);
}

void IndexArray::setAt(uint32_t i, const Index& t ) {
    Array1<Index>::setAt(i,t);
}

uint32_t * IndexArray::data( ) const {
  size_t _size = 0;
  for (const_iterator _i4f = __A.begin(); _i4f != __A.end(); _i4f++)
    _size += _i4f->getSize();
  if(__A.empty())return NULL;

  uint32_t * res = new uint32_t[__A.size()];
  size_t _k = 0;
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
	for (Index::const_iterator _j = _i->getBegin(); _j != _i->getEnd(); _j++)
    { res[_k] = *_j; _k++; }
  return res;
}

/* ----------------------------------------------------------------------- */

