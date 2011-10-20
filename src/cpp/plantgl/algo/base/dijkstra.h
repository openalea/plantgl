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


#include <queue>
#include <memory>

PGL_BEGIN_NAMESPACE

/**
  my priority queue for comparing node
*/
class node_priority_queue {
public:
    struct nodecompare {
       const real_t * distances;
       nodecompare(const real_t * _distances) : distances(_distances) {}
       bool operator()(const uint32_t& a,const uint32_t& b) { return distances[a] > distances[b]; }
    };

    std::deque<uint32_t> c;
    struct nodecompare comp;

    node_priority_queue(const real_t * distances): comp(distances) { }

	bool empty() const { return (c.empty()); }
	size_t size() const { return (c.size()); }

	void push(uint32_t _Val)
		{	
 	    	c.push_back(_Val);
		    push_heap(c.begin(), c.end(), comp);
		}

	uint32_t pop()
	{
        uint32_t topvalue = c.front(); 
		pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
        return topvalue;
	}

    void update() { make_heap(c.begin(),c.end()); }
};

struct Node {
    uint32_t id;
    uint32_t parent;
    real_t distance;
    Node(uint32_t _id, uint32_t _parent, real_t _distance) 
        : id(_id), parent(_parent), distance(_distance) {}
} ;

typedef std::vector<Node> NodeList;

template<class EdgeWeigthEvaluation>
NodeList  dijkstra_shortest_paths_in_a_range(const IndexArrayPtr& connections, 
                                   uint32_t root, 
                                   EdgeWeigthEvaluation& distevaluator,
                                   real_t maxdist = REAL_MAX)
 {

     NodeList result;

     size_t nbnodes = connections->size();
     real_t * distances = new real_t [nbnodes];
     for(real_t * it = distances; it != distances+nbnodes; ++it) *it = REAL_MAX;
     distances[root] = 0;
     uint32_t * parents = new uint32_t[nbnodes];
     parents[root] = root;

     std::vector<bool> colored(nbnodes,false);

     node_priority_queue Q(distances);
     Q.push(root);

     while(!Q.empty()){
         uint32_t current = Q.pop();
         result.push_back(Node(current,parents[current],distances[current])); 
         const Index& nextchildren = connections->getAt(current);
         for (Index::const_iterator itchildren = nextchildren.begin();
             itchildren != nextchildren.end(); ++itchildren)
         {
             uint32_t v = *itchildren;
             real_t weigthuv = distevaluator(current,v); 
             real_t distance = weigthuv+distances[current];
             if (distance < distances[v] && distance < maxdist){
                 distances[v] = distance;
                 parents[v] = current;
                 if (!colored[v]){
                     colored[v] = true;
                     Q.push(v);
                 }
                 else {
                     Q.update();
                 }
             }
         }
     }
     return result;
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
