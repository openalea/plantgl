/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
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

/*! \file algo_merge.h
    \brief Some algorithms for merging 2 mesh.
*/

#ifndef __algo_merge_h__
#define __algo_merge_h__

#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/scenegraph/geometry/quadset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include "discretizer.h"


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Merge
   \brief A class of algorithm which merge two Explicit Model.
          It is an optimization of the fusion algorithm.
*/


class ALGO_API Merge
{

public:

  /*! \enum MODEL_TYPE    
  Type of the model to be merge.
  */
  /*!
  \var MODEL_TYPE POINT_SET
	PointSet type
  */
  /*!
  \var MODEL_TYPE POLYLINE
    Polyline type
  */
  /*!
  \var MODEL_TYPE FACE_SET
    FaceSet type
  */
  /*!
  \var MODEL_TYPE QUAD_SET
    QuadSet type
  */
  /*!
  \var MODEL_TYPE TRIANGLE_SET
    TrinagleSet type
  */
  /*!
  \var MODEL_TYPE OTHER
    Other
  */

    enum MODEL_TYPE
      {
      POINT_SET= 0,
      POLYLINE,
      FACE_SET,
      QUAD_SET,
      TRIANGLE_SET,
      OTHER
      };

    static MODEL_TYPE getType( const ExplicitModelPtr& model );

public:

    /// Constructor.
    Merge( Discretizer& discretizer,
           ExplicitModelPtr& model );

    /// Destructor.
    virtual ~Merge( ) {}

    /// get the resulting model.
    virtual const ExplicitModelPtr& getModel() const
      { return __model; }

    virtual void setModel(const ExplicitModelPtr&);

    /// Merge with the Geometry \e geom
    virtual bool apply( GeometryPtr& geom );

    /// Merge with the ExplicitModel \e geom
    virtual bool apply( ExplicitModelPtr& geom );

    /// Merge with the PointSet \e geom
    virtual bool apply( PointSet& geom);

    /// Merge with the polyline \e geom
    virtual bool apply( Polyline& geom );

    /// Merge with the QuadSet \e geom
    virtual bool apply( QuadSet& geom );

    /// Merge with the TriangleSet \e geom
    virtual bool apply( TriangleSet& geom );

    /// Merge with the FaceSet \e geom
    virtual bool apply( FaceSet& geom );

    /// Method use for optimization purpose when all objects are similar.
    virtual void setIsoModel( uint_t nbObjects );

	virtual void checkNormals(Mesh& geom);

protected:

    /// The model to merge.
    ExplicitModelPtr __model;

    /// The type of model.
    MODEL_TYPE __type;

    /// The discretizer used to store the discretize parametric while
    Discretizer& __discretizer;

private:

    bool init();

    bool __isoModel;
};


/* ----------------------------------------------------------------------- */

// __algo_merge_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


