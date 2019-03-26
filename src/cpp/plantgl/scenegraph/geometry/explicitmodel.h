/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP
 *
 *       File author(s): F. Boudon et al.
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

