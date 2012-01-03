/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon
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
/*! \file indexarray_iterator.h
    \brief custom iterator for indexarray.
*/

/* ----------------------------------------------------------------------- */


#ifndef __indexarray_iterator_h__
#define __indexarray_iterator_h__

#include "indexarray.h"
#include <stack>

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class PreOrderConstIterator
    \brief IndexArray is supposed to represent parent-children relationship of a tree graph.
    The iterator iterate through all nodes in a pre-order
*/
template<class ListOfNodeList, class ListOfNodeListPtr = const ListOfNodeList*>
class NodeListConstIterator  {
public:
        typedef typename ListOfNodeList::element_type NodeList;
        typedef typename NodeList::const_iterator NodeIterator;

        typedef typename NodeList::element_type value_t ;
        typedef const value_t * pointer_t;

protected:
        // A TreeGraph representation as a root id and list of children for each node.
        value_t __root;
        ListOfNodeListPtr __children;
        bool __root_given;

        // pointer to the actual node id in the graph
        NodeIterator __current;
        // pointer to end of list of actual node id in the graph
        NodeIterator __current_end;

        // type definition of a pair of pointer and a stack of pairs
        typedef std::pair<NodeIterator,NodeIterator> NodeIteratorPair;
        typedef std::stack<NodeIteratorPair> IteratorStack;

        // stack of  higher value of pointers in the graph hierarchy
        IteratorStack __stack;

        // Pop pointers value from the stack
        inline void pop() 
        {  
            NodeIteratorPair p = __stack.top(); 
            __current = p.first;  __current_end = p.second; 
            __stack.pop();
        }

        // Push pointers value into the stack
        inline void push() 
        { 
            __stack.push( NodeIteratorPair(__current, __current_end)); 
        }

        // advance in the traversal
        virtual void increment() {}

        void multi_increment(int i) { 
            for (int j = 0 ; j < i; ++j) increment();
        }

public:

        NodeListConstIterator(ListOfNodeListPtr children, value_t root):
            __children(children), 
            __root(root),
            __root_given(false),
            __current((*children)[root].begin()), 
            __current_end((*children)[root].end()), 
            __stack()
            {
            }

        virtual ~NodeListConstIterator() { 
        }

        // Check if we are at the end of the iteration
        inline bool atEnd() const 
        { 
            return __root_given &&  __atEnd(); 
        }

        // value access operator for iterator
        inline value_t operator*() const 
        { 
            return *__current; 
        }

        // return pointer to node
        inline pointer_t operator->() const
		{	
            return __current.operator->(); 
        }

                // compare two iterators
        inline bool operator==(NodeListConstIterator other) {
            return __children == other.__children && __current == other.__current;
        }

        // compare two iterators
        inline bool operator!=(NodeListConstIterator other) {
            return __children != other.__children || __current != other.__current;
        }

protected:
        // Check if we are at the end of the recursion
        inline bool __atEnd() const 
        { 
            return __current == __current_end && __stack.empty(); 
        }


};

template<class ListOfNodeList, class ListOfNodeListPtr = const ListOfNodeList*>
class PreOrderConstIterator  : public NodeListConstIterator<ListOfNodeList,ListOfNodeListPtr> {
public: 
        typedef NodeListConstIterator<ListOfNodeList,ListOfNodeListPtr>  BaseType;
        typedef typename BaseType::value_t value_t;
        typedef typename BaseType::pointer_t pointer_t;
protected:
        typedef typename BaseType::NodeList NodeList;

protected:
        // advance in the traversal
        virtual void increment() {
            if (!BaseType::__root_given) {
                BaseType::__root_given = true;
            }
            else {
                value_t currentid = *BaseType::__current;
                const NodeList& childlist = (*BaseType::__children)[currentid];
                if (!childlist.empty()){
                    BaseType::push();
                    BaseType::__current = childlist.begin();
                    BaseType::__current_end = childlist.end();
                }
                else {
                    if (BaseType::__current != BaseType::__current_end) ++BaseType::__current;
                    while (BaseType::__current == BaseType::__current_end && !BaseType::__stack.empty()) {
                        BaseType::pop(); 
                        if (BaseType::__current != BaseType::__current_end) ++BaseType::__current;
                    }
                }
            }
        }

public:

        PreOrderConstIterator(ListOfNodeListPtr children, uint32_t root):
            NodeListConstIterator<ListOfNodeList,ListOfNodeListPtr>(children,root)
            {
            }

        virtual ~PreOrderConstIterator() { 
        }

        // value access operator for iterator
        inline value_t operator*() const 
        { 
            if (!BaseType::__root_given) return BaseType::__root;
            else return *BaseType::__current; 
        }

        // return pointer to node
        inline pointer_t operator->() const
		{	
            if (!BaseType::__root_given) return &BaseType::__root;
            else return BaseType::__current.operator->(); 
        }

        // pre increment operator for iterator
        inline PreOrderConstIterator& operator++() 
        { 
            increment(); 
            return *this; 
        }

        // post incremental operator for iterator
        inline PreOrderConstIterator operator++(int i) 
        { 
            PreOrderConstIterator original = *this; 
            BaseType::multi_increment(i); 
            return original; 
        }


        inline PreOrderConstIterator& operator+=(int i)
		{	// increment by an integer
            BaseType::multi_increment(i);
            return *this;
        }

        inline PreOrderConstIterator operator+(int i) const
		{	// increment by an integer
            PreOrderConstIterator newiter = *this;
            newiter.multi_increment(i);
            return newiter;
        }


};
typedef PreOrderConstIterator<IndexArray,IndexArrayPtr> IndexArrayPreOrderConstIterator;

template<class ListOfNodeList, class ListOfNodeListPtr = const ListOfNodeList*>
class PostOrderConstIterator  : public NodeListConstIterator<ListOfNodeList,ListOfNodeListPtr> {
public: 
        typedef NodeListConstIterator<ListOfNodeList,ListOfNodeListPtr>  BaseType;
        typedef typename BaseType::value_t value_t;
        typedef typename BaseType::pointer_t pointer_t;
protected:
        typedef typename BaseType::NodeList NodeList;
protected:
        // find first ascendant
        void first_ascendant() {
            if (BaseType::__current != BaseType::__current_end) {
                while (true) {
                    const NodeList& childlist = (*BaseType::__children)[*BaseType::__current];
                    // if a node has some children, register it in the stack and point to the first children
                    if (!childlist.empty()) {
                        BaseType::push();
                        BaseType::__current = childlist.begin();
                        BaseType::__current_end = childlist.end();
                    }
                    else break;
                }
            }
        }

        // advance in the traversal
        virtual void increment() {
            if (BaseType::__current != BaseType::__current_end) ++BaseType::__current;
            // if end of stack, go to ultimate parent : root
            else if (BaseType::__stack.empty()) BaseType::__root_given = true;

            if (BaseType::__current == BaseType::__current_end) {
                // if node was last of the sibling, go to the parent
                if (!BaseType::__stack.empty()) BaseType::pop();
             }
            // if node has children, go to its first ascendant
            else first_ascendant();
        }

public:

        PostOrderConstIterator(ListOfNodeListPtr children, uint32_t root):
            NodeListConstIterator<ListOfNodeList,ListOfNodeListPtr>(children,root)
            {
                first_ascendant();
            }

        virtual ~PostOrderConstIterator() { 
        }

        // value access operator for iterator
        inline value_t operator*() const 
        { 
            if (BaseType::__atEnd()) return BaseType::__root;
            else return *BaseType::__current; 
        }

        // return pointer to node
        inline pointer_t operator->() const
		{	
            if (BaseType::__atEnd()) return &BaseType::__root;
            else return BaseType::__current.operator->(); 
        }

         // pre increment operator for iterator
        inline PostOrderConstIterator& operator++() 
        { 
            increment(); 
            return *this; 
        }

        // post incremental operator for iterator
        inline PostOrderConstIterator operator++(int i) 
        { 
            PostOrderConstIterator original = *this; 
            BaseType::multi_increment(i); 
            return original; 
        }


        inline PostOrderConstIterator& operator+=(int i)
		{	// increment by an integer
            BaseType::multi_increment(i);
            return *this;
        }

        inline PostOrderConstIterator operator+(int i) const
		{	// increment by an integer
            PostOrderConstIterator newiter = *this;
            newiter.multi_increment(i);
            return newiter;
        }





};
typedef PostOrderConstIterator<IndexArray,IndexArrayPtr> IndexArrayPostOrderConstIterator;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

#endif
