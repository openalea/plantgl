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


/*! \file geom_explicitmodel.h
    \brief Definition of the geometry class ExplicitModel.
*/


#ifndef __geom_explicitmodel_h__
#define __geom_explicitmodel_h__

/* ----------------------------------------------------------------------- */

#include "primitive.h"

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>

#ifndef GEOM_FWDEF
#include <plantgl/scenegraph/transformation/transformed.h>
#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class ExplicitModel;
typedef RCPtr<ExplicitModel> ExplicitModelPtr;

#ifdef GEOM_FWDEF
class Transformation3D;
typedef RCPtr<Transformation3D> Transformation3DPtr;
#endif

/* ----------------------------------------------------------------------- */

/**
   \class ExplicitModel
   \brief A set of 3D points.
*/

class SG_API ExplicitModel : public virtual Primitive
{

public:

  /// A structure which helps to build an object of type of ExplicitModel.
  struct SG_API Builder : public Primitive::Builder {

    /// A pointer to the \b PointList field.
    Point3ArrayPtr * PointList;

    /// A pointer to the \b TexCoordList field.
    Color4ArrayPtr * ColorList;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual void destroy( );

    void EMDestroy( );

    virtual bool isValid( ) const;

    bool EMValid( ) const;

  };

  /// Constructs a empty ExplicitModel.
  ExplicitModel( );

  /// Constructs a ExplicitModel.
  ExplicitModel( const Point3ArrayPtr& points,
                 const Color4ArrayPtr& colors  = Color4ArrayPtr() );

  /// Destructor
  virtual ~ExplicitModel( );

  virtual bool isExplicit( ) const;

  /// Returns the array of  points \e self contains.
  virtual const Point3ArrayPtr& getPointList( ) const;

  /// Returns the array of points \e self contains.
  virtual Point3ArrayPtr& getPointList( );

  /// Returns the size of \b PointList.
  uint_t getPointListSize( ) const;

  /// Returns \b ColorList values.
  Color4ArrayPtr& getColorList( );

  /// Returns \b ColorList values.
  const Color4ArrayPtr& getColorList( ) const ;

  /// Returns the image of \e self by the transformation \e transformation.
  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const = 0;

  /// Returns whether \b Color is set to its default value.
  bool isColorListToDefault() const;

  /// Returns whether \b self has a colorList.
  bool hasColorList() const;

protected:

  /// The \b PointList field.
  Point3ArrayPtr __pointList;

  /// The \b Color List
  Color4ArrayPtr __colorList;

}; // ExplicitModel


/// ExplicitModel Pointer
typedef RCPtr<ExplicitModel> ExplicitModelPtr;


/* ----------------------------------------------------------------------- */

// __geom_explicitmodel_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

