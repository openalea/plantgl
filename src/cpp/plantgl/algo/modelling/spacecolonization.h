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

#ifndef __spacecolonization_h__
#define __spacecolonization_h__

#include "../algo_config.h"
#include <plantgl/math/util_math.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/algo/grid/regularpointgrid.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/function/function.h>

PGL_BEGIN_NAMESPACE


class ALGO_API SpaceColonization : public TOOLS(RefCountObject) {
    public:
        typedef Point3Grid::PointIndexList AttractorList;
        typedef TOOLS(Uint32Array1Ptr) Uint32ArrayPtr;

    protected:

      void register_attractors(const TOOLS(Vector3)& pos, Uint32ArrayPtr attlist);

        struct Bud {
            size_t pid;
            TOOLS(Vector3) direction;
            Uint32ArrayPtr attractors;
            real_t level;

            Bud(size_t _pid, const TOOLS(Vector3)& _direction, Uint32ArrayPtr _attractors):
                pid(_pid), direction(_direction), attractors(_attractors) {}

            Bud(size_t _pid, Uint32ArrayPtr _attractors, real_t _level):
                pid(_pid),  attractors(_attractors), level(_level){}
        };

        typedef std::vector<Bud> BudList;
        typedef std::vector<std::pair<Bud,uint32_t> > LatentBudList;

        Point3ArrayPtr attractors; 
        Point3GridPtr attractor_grid; 
        Point3ArrayPtr skeletonnodes; 
        Uint32ArrayPtr skeletonparents;
        IndexArrayPtr nodeattractors;
        Index active_nodes;

        
        BudList budlist;
        LatentBudList latentbudlist;

        typedef pgl_hash_map<size_t, std::pair<Uint32ArrayPtr, real_t> > AttractorMap;
        AttractorMap attractormap;

    public:

        static const size_t NOID;

        SpaceColonization(const Point3ArrayPtr _attractors,                           
                          real_t nodelength,
                          real_t kill_radius,
                          real_t perception_radius,
                          const Point3ArrayPtr initialskeletonnodes = Point3ArrayPtr(0), 
                          const Uint32ArrayPtr  initialskeletonparent = Uint32ArrayPtr(0),
                          const Index& _active_nodes = Index(0),
                          size_t spacetilingratio = 100);

        SpaceColonization(const Point3ArrayPtr _attractors,                           
                          real_t nodelength,
                          real_t kill_radius,
                          real_t perception_radius,
                          const TOOLS(Vector3)& rootnode, 
                          size_t spacetilingratio = 100);

    virtual ~SpaceColonization();

    size_t add_node(const TOOLS(Vector3)& position, size_t parent = NOID, const Index& attractors = Index(), bool active = true);

    inline TOOLS(Vector3) node_direction(size_t pid) const {
        size_t parent = skeletonparents->getAt(pid);
        if (parent == NOID || parent == pid) return TOOLS(Vector3)::OZ;
        return TOOLS(direction)(skeletonnodes->getAt(pid) - skeletonnodes->getAt(parent));

    }

    inline const TOOLS(Vector3)& node_position(size_t pid) const {
        return skeletonnodes->getAt(pid);
    }

    inline const Index& node_attractors(size_t pid) const {
        return nodeattractors->getAt(pid);
    }

    bool try_to_set_bud(size_t pid, const TOOLS(Vector3)& direction);

    void add_bud(size_t pid, const TOOLS(Vector3)& direction, const AttractorList& attractors);
    void add_bud(size_t pid, const AttractorList& attractors, real_t level);

    void add_latent_bud(size_t pid, const AttractorList& attractors, real_t level, uint32_t latency);


    inline void activate_node(size_t nid) {
        if (find(active_nodes.begin(),active_nodes.end(), nid) == active_nodes.end())
            _activate_node(nid);
    }

    inline void _activate_node(size_t nid) {
        active_nodes.push_back(nid);
    }

    inline void activate_all() {
        active_nodes = TOOLS(range<Index>)(0,skeletonnodes->size(),1);
    }

    void activate_leaves() ;

    inline void desactivate_node(size_t nid) {
        Index::iterator it = find(active_nodes.begin(),active_nodes.end(), nid);
        if (it != active_nodes.end()) active_nodes.erase(it);
    }

    /// compute a whorl of 'nb' buds at branching angles.
    std::vector<TOOLS(Vector3)> lateral_directions(const TOOLS(Vector3)& dir, real_t angle, int nb);

    virtual void generate_buds(size_t pid) ;
    virtual void process_bud(const Bud& bud);

    inline size_t nbLatentBud() const { return latentbudlist.size(); }

    void generate_all_buds() ;
    void growth() ;
    void step();
    void run();
    void iterate(size_t nbsteps);

    bool atEnd();

    const Index& get_active_nodes() const { return active_nodes; }

    inline Point3ArrayPtr get_nodes() const { return skeletonnodes; }
    inline Uint32ArrayPtr get_parents() const { return skeletonparents; }
    IndexArrayPtr get_children() const ;

    Point3GridPtr get_grid() const { return attractor_grid; }

    inline void setLengths(real_t _nodelength, real_t kill_radius_ratio = 0.9, real_t perception_radius_ratio = 2.0)
    { nodelength = _nodelength; kill_radius = _nodelength * kill_radius_ratio; perception_radius = _nodelength * perception_radius_ratio; }

    real_t nodelength;
    real_t kill_radius;
    real_t perception_radius;
    real_t coneangle;
    
    real_t insertion_angle;
    size_t nb_buds_per_whorl;

    size_t min_nb_pt_per_bud;
    size_t nbIteration;

    virtual void node_buds_preprocess(size_t pid) { }
    virtual void node_buds_postprocess(size_t pid) { }

    virtual void node_child_preprocess(size_t pid) { }
    virtual void node_child_postprocess(size_t pid, const Index& children) { }

    virtual void StartEach() { }
    virtual void EndEach()   { }

}; 


typedef RCPtr<SpaceColonization> SpaceColonizationPtr;


class ALGO_API GraphColonization : public SpaceColonization {

    public:

        GraphColonization(const Point3ArrayPtr _attractors,                           
                               real_t perception_radius,
                               const IndexArrayPtr graph,
                               uint32_t root, 
                               real_t powerdistance = 1,
                               size_t spacetilingratio = 100);


      virtual ~GraphColonization();

      virtual void generate_buds(size_t pid) ;
      virtual void process_bud(const Bud& bud);



      IndexArrayPtr graph;
      TOOLS(RealArrayPtr)  distances_from_root;
      uint32_t root;
      bool use_jonction_points;

      real_t powerdistance;

      IndexArrayPtr nodecomponents;
      TOOLS(RealArrayPtr) nodelevels;

      inline IndexArrayPtr get_nodecomponents() const { return nodecomponents; }

      inline const real_t node_level(size_t pid) const {
        return nodelevels->getAt(pid);
      }

      inline const Index& node_components(size_t pid) const {
        return nodecomponents->getAt(pid);
      }

      size_t add_node(const TOOLS(Vector3)& position, 
                      real_t level,
                      const Index& components,
                      size_t parent = SpaceColonization::NOID, bool active = true);

      Index junction_components(size_t nid1, size_t nid2) const;
      TOOLS(Vector3) junction_point(size_t nid1, size_t nid2) const;

    protected:
        void init();
};

typedef RCPtr<GraphColonization> GraphColonizationPtr;

PGL_END_NAMESPACE

#endif

