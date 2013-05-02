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



/*! \file screen_projected.h
    \brief Definition of the transformation ScreenProjected.
*/

#ifndef __screen_projected_h__
#define __screen_projected_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/scenegraph/transformation/transformed.h>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/** 
    \class ScreenProjected
    \brief A transformation that project in screen coordinates
*/


class SG_API ScreenProjected : public Transformed
{

public:

	static bool DEFAULT_KEEPASPECTRATIO;

   /// A structure which helps to build an object of type of Transformed.
  struct SG_API Builder : public Transformed::Builder {

    /// A pointer to the \b Geometry field.
    GeometryPtr * Geometry;

	bool * KeepAspectRatio;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;


  };


  /// Construcs a ScreenProjected.
  ScreenProjected( const GeometryPtr& geometry, bool keepAspectRatio = true);

  ScreenProjected( );

  /// Destructor.
  virtual ~ScreenProjected( );

  PGL_OBJECT(ScreenProjected)

  /// Returns Geometry value.
  virtual const GeometryPtr getGeometry( ) const
  { return __geometry; }

  /// Returns Geometry field.
  GeometryPtr& getGeometry( )
  { return __geometry; }

  virtual bool isACurve( ) const
  { return __geometry->isACurve(); }


  virtual bool isASurface( ) const
  { return __geometry->isASurface(); }

  virtual bool isAVolume( ) const 
  { return __geometry->isAVolume(); }

  virtual bool isExplicit( ) const 
  { return __geometry->isExplicit(); }

  /// Returns the transformation attached to \e self.
  virtual Transformation3DPtr getTransformation( ) const
  { return Transformation3DPtr(); }

  virtual bool isValid( ) const;

  bool getKeepAspectRatio() const { return __keepAspectRatio; }

  bool& getKeepAspectRatio() { return __keepAspectRatio; }

  bool isKeepAspectRatioToDefault() const { return __keepAspectRatio == DEFAULT_KEEPASPECTRATIO; }

protected:

  /// The Geometry field.
  GeometryPtr __geometry;

  bool __keepAspectRatio;
};
 
/// Transformed Pointer
typedef RCPtr<ScreenProjected> ScreenProjectedPtr;

/* ----------------------------------------------------------------------- */

// __screenprojected_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

