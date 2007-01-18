/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier 
 *
 *       $Source$
 *       $Id$
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


/*! \file geom_explicitmodel.h
    \brief Definition of the geometry class ExplicitModel.
*/


#ifndef __geom_explicitmodel_h__
#define __geom_explicitmodel_h__

/* ----------------------------------------------------------------------- */

#include "geom_primitive.h"
#ifndef GEOM_FWDEF
#include "geom_pointarray.h"
#include "geom_transformed.h"
#endif
#include "geom_colorarray.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class ExplicitModel;
typedef RCPtr<ExplicitModel> ExplicitModelPtr;
#ifdef GEOM_FWDEF
class Transformation3D;
typedef RCPtr<Transformation3D> Transformation3DPtr;
class Point3Array;
typedef RCPtr<Point3Array> Point3ArrayPtr;
#endif
/* ----------------------------------------------------------------------- */

/**
   \class ExplicitModel
   \brief A set of 3D points.
*/

class GEOM_API ExplicitModel : public virtual Primitive
{

public:

  /// A structure which helps to build an object of type of ExplicitModel.
  struct Builder : public Primitive::Builder {

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
				 const Color4ArrayPtr& colors  = 0 );

  /// Destructor
  virtual ~ExplicitModel( );

  virtual bool isExplicit( ) const;

  /// Returns the array of  points \e self contains.
  virtual const Point3ArrayPtr& getPointList( ) const;

  /// Returns the array of points \e self contains.
  virtual Point3ArrayPtr& getPointList( );

  /// Returns the size of \b PointList.
  uint32_t getPointListSize( ) const;

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

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

