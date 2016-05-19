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

#ifndef __dijkstra_h__
#define __dijkstra_h__

#include "../algo_config.h"
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/tool/util_array.h>

#include <memory>
#include <boost/version.hpp>

#if BOOST_VERSION >= 104900
    #include <boost/heap/fibonacci_heap.hpp>
    #define PGL_USE_FIBONACCI_HEAP
#else
    #include <queue>
    #include <vector>
    #define PGL_USE_PRIORITY_QUEUE
#endif
    

PGL_BEGIN_NAMESPACE

/**
  my priority queue for comparing node
*/


struct Node {
    uint32_t id;
    uint32_t parent;
    real_t distance;
    Node(uint32_t _id, uint32_t _parent, real_t _distance) 
        : id(_id), parent(_parent), distance(_distance) {}
} ;

typedef std::vector<Node> NodeList;


struct nodecompare {
       TOOLS(RealArrayPtr) __distances;

       nodecompare(const TOOLS(RealArrayPtr)& distances) : __distances(distances) {}
       bool operator()(const uint32_t& a,const uint32_t& b) const { return __distances->getAt(a) > __distances->getAt(b); }
};


enum color { black, grey, white };

typedef std::vector<std::pair<uint32_t, real_t> > NodeDistancePairList;

template<class EdgeWeigthEvaluation>
NodeList  dijkstra_shortest_paths_in_a_range(const IndexArrayPtr& connections, 
                                             uint32_t root, 
                                             EdgeWeigthEvaluation& distevaluator,
                                             real_t maxdist = REAL_MAX,
                                             uint32_t maxnbelements = UINT32_MAX,
                                             const NodeDistancePairList& precomputed = NodeDistancePairList())
 {

     NodeList result;

     size_t nbnodes = connections->size();
     size_t nbprocessednodes = 0;

     TOOLS(RealArrayPtr) distances(new TOOLS(RealArray)(nbnodes,REAL_MAX));
     distances->setAt(root,0);
 
     uint32_t * parents = new uint32_t[nbnodes];
     parents[root] = root;
 
     color * colored = new color[nbnodes];
     for (color * it = colored ; it != colored+nbnodes ; ++it) *it = black;

#ifdef PGL_USE_FIBONACCI_HEAP
     typedef boost::heap::fibonacci_heap<uint32_t, boost::heap::compare<nodecompare> > heap;
     typedef typename heap::handle_type handle;
#else
     typedef std::priority_queue<uint32_t, std::vector<uint32_t>, nodecompare> heap;
#endif
     struct nodecompare comp(distances);
     heap Q(comp);

#ifdef PGL_USE_FIBONACCI_HEAP
     handle * handles = new handle[nbnodes];
     handles[root] = Q.push(root);
#endif

     for(NodeDistancePairList::const_iterator itdist = precomputed.begin(); itdist != precomputed.end(); ++itdist){
        if (itdist->second < maxdist){
            distances->setAt(itdist->first,itdist->second);
            parents[itdist->first] = root;
#ifdef PGL_USE_FIBONACCI_HEAP
            handles[itdist->first] = Q.push(itdist->first);
#endif
        }
     }

     while((!Q.empty()) && (nbprocessednodes < maxnbelements)){
         uint32_t current = Q.top(); Q.pop();
#ifdef PGL_USE_PRIORITY_QUEUE
         if(colored[current] == white) continue;
#endif
         result.push_back(Node(current, parents[current], distances->getAt(current)));
         colored[current] = white;

         nbprocessednodes += 1;

         const Index& nextchildren = connections->getAt(current);
         for (Index::const_iterator itchildren = nextchildren.begin();
             itchildren != nextchildren.end(); ++itchildren)
         {
             uint32_t v = *itchildren;

             real_t weigthuv = distevaluator(current,v); 
             real_t distance = weigthuv + distances->getAt(current);

             if (distance <= maxdist && distance < distances->getAt(v)) {
                distances->setAt(v,distance);
                parents[v] = current;

                if (colored[v] == black) {
                    colored[v] = grey;
#ifdef PGL_USE_FIBONACCI_HEAP
                    handles[v] = Q.push(v);
#else
                    Q.push(v);
#endif
                }
                else if (colored[v] == grey){
#ifdef PGL_USE_FIBONACCI_HEAP
                    Q.decrease(handles[v], v);
#else
                    Q.push(v);
#endif
                }

             }
         }
     }
#ifdef PGL_USE_FIBONACCI_HEAP
     delete [] handles;
#endif
     delete [] colored;
     delete [] parents;
     return result;
 }


template<class EdgeWeigthEvaluation>
std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)>  dijkstra_shortest_paths(const IndexArrayPtr& connections, 
                                   uint32_t root, 
                                   EdgeWeigthEvaluation& distevaluator)
 {


     size_t nbnodes = connections->size();
     TOOLS(RealArrayPtr) distances(new TOOLS(RealArray)(nbnodes,REAL_MAX));
     distances->setAt(root,0);

     TOOLS(Uint32Array1Ptr) parents(new TOOLS(Uint32Array1)(nbnodes,UINT32_MAX));
     parents->setAt(root,root);

     std::vector<color> colored(nbnodes,black);

#ifdef PGL_USE_FIBONACCI_HEAP
     typedef boost::heap::fibonacci_heap<uint32_t, boost::heap::compare<nodecompare> > heap;
     typedef typename heap::handle_type handle;
#else
     typedef std::priority_queue<uint32_t, std::vector<uint32_t>, nodecompare> heap;
#endif

     struct nodecompare comp(distances);
     heap Q(comp);

#ifdef PGL_USE_FIBONACCI_HEAP
     handle * handles = new handle[nbnodes];
     handles[root] = Q.push(root);
#endif

     while(!Q.empty()){
         uint32_t current = Q.top(); Q.pop(); 
#ifdef PGL_USE_PRIORITY_QUEUE
         if(colored[current] == white) continue;
#endif
         colored[current] = white;
         const Index& nextchildren = connections->getAt(current);
         for (Index::const_iterator itchildren = nextchildren.begin();
             itchildren != nextchildren.end(); ++itchildren)
         {
             uint32_t v = *itchildren;
             real_t weigthuv = distevaluator(current,v); 
             real_t distance = weigthuv+distances->getAt(current);
             if (distance < distances->getAt(v)){
                 // printf("consider child %i %f %f %i\n", v, distance, (distances->getAt(v)!=REAL_MAX?distances->getAt(v):-1), int(colored[v]));
                 distances->setAt(v,distance);
                 parents->setAt(v,current);
                if (colored[v] == black) {
                    colored[v] = grey;
#ifdef PGL_USE_FIBONACCI_HEAP
                    handles[v] = Q.push(v);
#else
                    Q.push(v);
#endif
                }
                else if (colored[v] == grey){
#ifdef PGL_USE_FIBONACCI_HEAP
                    Q.decrease(handles[v], v);
#else
                    Q.push(v);
#endif
                }
             }
         }
     }
     return std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)>(parents,distances);
 }

 /*
 DIJKSTRA(G, s, w)
  for each vertex u in V
    d[u] := infinity 
    p[u] := u 
    color[u] := WHITE
  end for
  color[s] := GRAY 
  d[s] := 0 
  INSERT(Q, s)
  while (Q != Ø)
    u := EXTRACT-MIN(Q)
    S := S U { u }
    for each vertex v in Adj[u]
      if (w(u,v) + d[u] < d[v])
        d[v] := w(u,v) + d[u]
        p[v] := u 
        if (color[v] = WHITE) 
          color[v] := GRAY
          INSERT(Q, v) 
        else if (color[v] = GRAY)
          DECREASE-KEY(Q, v)
      else
        ...
    end for
    color[u] := BLACK
  end while
  return (d, p)
  */

PGL_END_NAMESPACE

#endif
