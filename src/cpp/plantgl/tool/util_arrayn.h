/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2009 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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

#ifndef __util_arrayn_h__
#define __util_arrayn_h__

#include <list>
#include <vector>
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/errormsg.h>


TOOLS_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

template<class ArrayT>
struct const_partial_iterator {
public:
    // friend class  ArrayT;
	typedef typename ArrayT::Index Index;
	typedef typename ArrayT::element_type element_type;
protected:
     const ArrayT * __array;
     Index  __current;
     Index  __minindex;
     Index  __maxindex;

	void increment() {
			assert(!atEnd()); 
			for (size_t j = 0; j < ArrayT::NbDimension; ++j){
				size_t i = ArrayT::NbDimension - 1 - j;
				__current[i]+=1;
				if(__current[i] > __maxindex[i]) {
					if (i != 0) __current[i] = __minindex[i];
					else if(__current[i] > __maxindex[i]+1) 
						assert(false && "const_partial_iterator out of scope");
				}
				else break;
			}
			// printf("%i,%i,%i, %i\n",index()[0],index()[1],index()[2],cellId());
		}

public:
	const_partial_iterator(const Index& minindex, const Index& maxindex, const ArrayT * array):
			__current(minindex), __minindex(minindex), __maxindex(maxindex), __array(array) {}

    const_partial_iterator<ArrayT>& operator++() { increment(); return *this; }
    const_partial_iterator<ArrayT> operator++(int i) { const_partial_iterator<ArrayT> t = *this; for (int j =0; j < i; j++)increment(); return t; }

	const element_type& value() const { return __array->getAt(__current); }
	const Index& index() const { return __current; }
	typename ArrayT::CellId cellId() const { return __array->cellId(__current); }

    const element_type& operator*( ) const { return value(); }
    const element_type * operator->( ) const { return &value(); }

		// element_type& value() { return __array->getAt(__current); }
        // element_type& operator*( ) { return value(); }
        // element_type * operator->( ) { return &value(); }

    const element_type& next( ) { const element_type& v = value(); increment(); return v; }

	bool atEnd() const {
		if(__current[0] > __maxindex[0]+1) return true;
		if(__current[0] <= __maxindex[0]) return false; 
		for (size_t j = 1; j < ArrayT::NbDimension; ++j)
			if(__current[j] != __minindex[j]) return false;
		// printf("atEnd\n");
		return true;
	}
	bool atBegin() const { return __current == __minindex; }
	void toBegin() { __current = __minindex; }

};

template <class T, int N>
class ArrayN : public RefCountObject
{
public:

	static const int NbDimension  = N;

	typedef T element_type;
	typedef std::vector<T> Container;
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;
	typedef const_partial_iterator<ArrayN<T,N> > const_partial_iterator;
	typedef Tuple<size_t,N> Index;
	typedef size_t CellId;

	explicit ArrayN(const Index& size):
	  __dimensions(size),
	  __values(product(size)){
	}

	ArrayN():
	  __dimensions(),
	  __values(){
	}

	CellId cellId(const Index& coord) const {
		size_t offset = 1;
		CellId v = 0;
		for (size_t i = 0; i < N; ++i){
			size_t j = N-1-i;
			v += coord[j] * offset;
			offset *= __dimensions[j];
		}
		return v;		
	}

	 Index index(CellId cid) const {
	    Index _offsets = offsets(__dimensions);
		Index v;
		for (size_t i = 0; i < N; ++i){
			v[i] = cid / _offsets[i];
			cid -=  v[i] * _offsets[i];
		}
		return v;		
	}

	inline bool isValidId(const CellId& cid) const { return cid < __values.size(); }

	inline bool isValidIndex(const Index& ind) const {
		for (size_t i = 0; i < N; ++i){
			if (ind[i] >= __dimensions[i]) return false;
		}
		return true;
	}

	inline const T& getAt(const Index& ind) const{
		return __values[cellId(ind)];
	}

	inline const T& getAt(const CellId& cid) const{
		return __values[cid];
	}

	inline T& getAt(const Index& ind) {
		return __data[cellId(ind)];
	}

	inline T& getAt(const CellId& cid) {
		return __values[cid];
	}

	inline void setAt(const Index& ind, const T& value ) {
		__values[cellId(ind)] = value;
	}

	inline void getAt(const CellId& cid, const T& value) {
		__values[cid] = value;
	}

  /// Returns whether \e self is empty.
  inline bool empty( ) const { return __values.empty(); }

  /// Clear \e self.
  inline void clear( ) { __values.clear(); __dimensions = Index(0); }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator begin( ) const { return __values.begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline iterator begin( ) { return __values.begin(); }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator end( ) const { return __values.end(); }

  /// Returns a const iterator at the end of \e self.
  inline iterator end( ) { return __values.end(); }


	const_partial_iterator getSubArray(const Index& first, const Index dimension) const {
		for (size_t i = 0; i < N; ++i)
			assert(first[i]+dimension[i] < __dimensions[i] && "invalid dimension for subarray.");
		return const_partial_iterator(first,add(first,dimension), this);
	}

	static Index offsets(const Index& value) { 
		Index offsets;
		size_t offsetcount = 1;
		size_t NbDimension = value.size();
		for (size_t i = 0; i < NbDimension; ++i){
			size_t j = NbDimension-1-i;
			offsets[j] = offsetcount;
			offsetcount *= value[j];
		}
		return offsets;
	} 


	inline Index dimensions() const { return __dimensions; }
	inline size_t size() const { return __values.size(); }
protected:
	void initialize(const Index& dim) { 
		__values = Container(product(dim));
		__dimensions = dim;
	}

	inline void setDimensions(const Index& dim) { __dimensions = dim; }

	Index  __dimensions;
	Container __values;
};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE
#endif
