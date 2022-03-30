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


/*! \file geom_elevationgrid.h
    \brief Definition of the geometry class ElevationGrid.
*/


#ifndef __geom_elevationgrid_h__
#define __geom_elevationgrid_h__

/* ----------------------------------------------------------------------- */

#include "patch.h"
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE
class RealArray2;
typedef RCPtr<RealArray2> RealArray2Ptr;
PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class ElevationGrid
   \brief A mesh represented by an array of points.
*/


class SG_API ElevationGrid : public Patch
{

public:

  /// The default XSpacing field value.
  static const real_t DEFAULT_X_SPACING;

  /// The default YSpacing field value.
  static const real_t DEFAULT_Y_SPACING;


  /// A structure which helps to build a ElevationGrid when parsing.
  struct SG_API Builder : public Patch::Builder {


    /// A pointer to the HeightList field.
    RealArray2Ptr * HeightList;

    /// A pointer to the XSpacing field.
    real_t * XSpacing;

    /// A pointer to the YSpacing field.
    real_t * YSpacing;

    /// Constructor.
    Builder( );

    /// Detructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  ElevationGrid();

  /// Complete constructor.
  ElevationGrid( const RealArray2Ptr& heights,
         real_t xSpacing = DEFAULT_X_SPACING,
         real_t ySpacing = DEFAULT_Y_SPACING,
         bool ccw = DEFAULT_CCW);

  /// Destructor
  virtual ~ElevationGrid( );

  PGL_OBJECT(ElevationGrid)

  /** Returns the value of \b HeightList at the i-th row and the j-th column.
      \warning
      - \e i must belong to the range [0,XDim[;
      - \e j must belong to the range [0,YDim[. */
  const real_t& getHeightAt( const uint_t i, const uint_t j ) const;

  /** Returns the field of \b HeightList at the i-th row and the j-th column.
      \warning
      - \e i must belong to the range [0,XDim[;
      - \e j must belong to the range [0,YDim[. */
  real_t& getHeightAt( const uint_t i, const uint_t j );

  /// Returns \b HeightList values.
  const RealArray2Ptr& getHeightList( ) const;

  /// Returns \b HeightList field.
  RealArray2Ptr& getHeightList( );

  /// Returns \e CrtlPoints value.
  virtual Point4MatrixPtr getCtrlPoints( ) const;


  /// Returns \b XDim value.
  const uint_t getXDim( ) const;

  /// Returns the extent of \e self along the \c x-axis.
  const real_t getXSize( ) const;

  /// Returns \b XSpacing value.
  const real_t& getXSpacing( ) const;

  /// Returns \b XSpacing field.
  real_t& getXSpacing( ) ;

  /// Returns \b YDim value.
  const uint_t getYDim( ) const;

  /// Returns the extent of \e self along the \c y-axis.
  const real_t getYSize( ) const;

  /// Returns YSpacing value.
  const real_t& getYSpacing( ) const;

  /// Returns YSpacing field.
  real_t& getYSpacing( );

  /// Returns the (i,j)th point of the grid
  Vector3 getPointAt(uint_t i, uint_t j) const;

  ///  Returns the field of \b HeightList at the position pos. If pos is outside grid, return 0
  real_t getHeightAt(const Vector2 pos) const;

  /// Returns whether \b XSpacing is set to its default value.
  bool isXSpacingToDefault( ) const;

  /// Returns whether \b YSpacing is set to its default value.
  bool isYSpacingToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The HeightList field.
  RealArray2Ptr __heightList;

  /// The XSpacing field.
  real_t __xSpacing;

  /// The YSpacing field.
  real_t __ySpacing;

}; // ElevationGrid

/// ElevationGrid Pointer
typedef RCPtr<ElevationGrid> ElevationGridPtr;


/* ----------------------------------------------------------------------- */

// __geom_elevationgrid_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

