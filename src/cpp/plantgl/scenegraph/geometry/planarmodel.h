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



/*! \file geom_planarmodel.h
    \brief Definition of the geometry class PlanarModel.
*/



#ifndef __geom_planarmodel_h__
#define __geom_planarmodel_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class PlanarModel
    \brief Abstract base class for objects of type of planar model.
*/

/* ----------------------------------------------------------------------- */

class SG_API PlanarModel : public ParametricModel
{

public:

  /// A structure which helps to build an object of type of PlanarModel.
  struct SG_API Builder : public ParametricModel::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

  };


  /// Constructor.
  PlanarModel();

  /// Destructor
  virtual ~PlanarModel( );

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const ;

  virtual bool isAVolume( ) const ;


};

/// PlanarModel Pointer
typedef RCPtr<PlanarModel> PlanarModelPtr;


/* ----------------------------------------------------------------------- */

// __geom_planarmodel_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

