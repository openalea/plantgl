/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */


/*! \file algo_merge.h
    \brief Some algorithms for merging 2 mesh.
*/

#ifndef __algo_merge_h__
#define __algo_merge_h__

#include "discretizer.h"
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/scenegraph/geometry/quadset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/pointset.h>


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
