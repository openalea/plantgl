/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) 
 *
 *       $Source$
 *       $Id: vxl_voxel.h 1169 2003-05-07 10:45:54Z puech $
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



/*! \file vxl_voxel.h
    \brief Definition of Voxel.
*/

#ifndef __vxl_voxel_h__
#define __vxl_voxel_h__

/* ----------------------------------------------------------------------- */

#include "tile.h"
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/scene/shape.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/** 
    \class Voxel
    \brief A Voxel is a regular Tile whose geometry is a box.
*/

/* ----------------------------------------------------------------------- */

class ALGO_API Voxel : public Tile {

    public :

  static AppearancePtr EMPTY_APPEARANCE;
  static AppearancePtr FILLED_APPEARANCE;
  static AppearancePtr UNDETERMINED_APPEARANCE;
	
    /** Default constructor. */
	Voxel(Tile * Complex = NULL, 
	      const unsigned char Scale = 0, 
	      const TileType Type = Empty, 
	      const unsigned char Num = 0, 
	      const TOOLS(Vector3)& PMin = TOOLS(Vector3)::ORIGIN,
	      const TOOLS(Vector3)& PMax = TOOLS(Vector3)::ORIGIN);
    
    

    /// Destructor
    virtual ~Voxel();


    /// Validity check.
    virtual bool isValid() const ;

    /// Return center of \e self.
    inline TOOLS(Vector3) getCenter() const {
	return (__ur + __ll) / 2 ;
    }
    
    /// Return size of \e self.
    inline TOOLS(Vector3) getSize() const {
	return (__ur - __ll) / 2 ;
    }
    
    /// Return coordinates min of \e self.
    inline const TOOLS(Vector3)& getMinCoord() const {
	return __ll;
    }
    
    /// Return coordinates min of \e self.
    inline TOOLS(Vector3)& getMinCoord(){
	return __ll;
    }
    
    /// Return Lower Left Corner of \e self.
    inline const TOOLS(Vector3)& getLowerLeftCorner() const {
	return __ll;
    }
    
    /// Return Lower Left Corner of \e self.
    inline TOOLS(Vector3)& getLowerLeftCorner(){
	return __ll;
    }
    
    /// Return coordinates max of \e self.
    inline const TOOLS(Vector3)& getMaxCoord() const {
	return __ur;
    }
    
    /// Return coordinates max of \e self.
    inline TOOLS(Vector3)& getMaxCoord(){
	return __ur;
    }
    
    /// Return Upper Right Corner of \e self.
    inline const TOOLS(Vector3)& getUpperRightCorner() const {
	return __ur;
    }
    
    /// Return Upper Right Corner  of \e self.
    inline TOOLS(Vector3)& getUpperRightCorner(){
	return __ur;
    }
    
    /// Set coordinates min and max of \e self from \e bbox.
    void setBBox(const BoundingBox& bbox);

    /// Set coordinates min and max of \e self from \e bbox.
    void setBBox(const BoundingBoxPtr& bbox);

    
   /*! Intersection between \e self and \e voxel.
      \pre
      - \e self must be valid;
      - \e voxel must ne non null and valid. */
   bool intersect( const Voxel& voxel ) const;

   /*! Intersection between \e self and \e bbox.
      \pre
      - \e self must be valid;
      - \e bbox must be valid. */
   bool intersect( const BoundingBox& bbox ) const;

  /*! Intersection between \e self and \e point.
      \pre
      - \e self must be valid;
      - \e point must be valid. */
    bool intersect( const TOOLS(Vector3)& point ) const;

  /*! Intersection between projection of \e self on z=0 and \e point.
      \pre
      - \e self must be valid;
      - \e point must be valid. */
    bool intersect( const TOOLS(Vector2)& point ) const;

  /*! Intersection between  \e self and a segment defined by [ \e point1 , \e point2 ]. 
      \pre
      - \e self must be valid;
      - \e point1 and \e point2 must be valid. */
    bool intersect( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2 ) const;

  /*! Intersection between projection of \e self on z=0 and a segment defined by [ \e point1 , \e point2 ]. 
      \pre
      - \e self must be valid;
      - \e point1 and \e point2 must be valid. */
    bool intersect( const TOOLS(Vector2)& point1, const TOOLS(Vector2)& point2 ) const;

  /*! Intersection between \e self and a triangle defined by [ \e point1 , \e point2 , \e point3 ].
      Test if an edge of the voxel cross the triangle.
      \pre
      - \e self must be valid;
      - \e point1, \e point2  and \e point3 must be valid. */
   bool intersect( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2, const TOOLS(Vector3)& point3 ) const;

  /*! Intersection between \e self and a quad facet defined by [ \e point1 , \e point2 , \e point3 , \e point4 ].
      Test if an edge of the voxel cross the facet.
      \pre
      - \e self must be valid;
      - \e facet must be planar and convex;
      - \e point1, \e point2, \e point3 and \e point4 must be valid. */
   bool intersect( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2, const TOOLS(Vector3)& point3, const TOOLS(Vector3)& point4 ) const;


   /// Return a representation of self with GEOM object.
   Shape3DPtr representation() const;


   /// Returns whether \e v1 intersect \e v2.
   friend ALGO_API bool intersection( const Voxel& v1, const Voxel& v2);
   
   /// Returns whether \e v1 intersect \e b2.
   friend ALGO_API bool intersection( const Voxel& v1, const BoundingBox& b2);
   
   /** Returns whether \e v1 intersect \e b2.
       \pre
       - \e b2 must be non null and valid.
   */
   friend ALGO_API bool intersection( const Voxel& v1, const BoundingBoxPtr& b2);
   
   /// Returns whether \e v1 intersect \e v2.
   friend ALGO_API bool intersection( const Voxel& v1, const TOOLS(Vector3)& v2);

   /** Returns whether \e v1 intersect segment define by [ \e seg1, \e seg2 ].
       This is a progressive test.
       Test first if a vertex of the segment lies in the voxel,
       then if a point of the edge of the segment lies in the voxel. */
   friend ALGO_API bool intersection( const Voxel& v1, const TOOLS(Vector3)& seg1, const TOOLS(Vector3)& seg2);

   /** Returns whether \e v1 intersect triangle define by [ \e triangle1, \e triangle2 , \e triangle3 ].
       This is a progressive test.
       Test first if a vertex of the triangle lies in the voxel,
       then if an edge of the triangle lies in the voxel,
       and finally if an edge of the voxel lies in the triangle. */
   friend ALGO_API bool intersection( const Voxel& v1, const TOOLS(Vector3)& triangle1, const TOOLS(Vector3)& triangle2, const TOOLS(Vector3)& triangle3);

   /** Returns whether \e v1 intersect quad facet define by [ \e quad1, \e quad2 , \e quad3 , \e quad4 ].
       This is a progressive test.
       Test first if a vertex of the facet lies in the voxel,
       then if an edge of the facet lies in the voxel,
       and finally if an edge of the voxel lies in the facet.
       \pre
       - the facet must be planar and convex.
   */
   friend ALGO_API bool intersection( const Voxel& v1, const TOOLS(Vector3)& quad1, const TOOLS(Vector3)& quad2, const TOOLS(Vector3)& quad3, const TOOLS(Vector3)& quad4);

   protected :
	
   /// coordinates min of \e self.
   TOOLS(Vector3) __ll;
   
   /// coordinates max of \e self.
   TOOLS(Vector3) __ur;
   
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
