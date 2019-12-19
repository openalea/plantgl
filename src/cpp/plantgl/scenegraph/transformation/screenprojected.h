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

