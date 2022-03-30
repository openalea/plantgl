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



/*! \file appe_appearance.h
    \brief Definition of the appearance class Appearance.
*/


#ifndef __appe_appearance_h__
#define __appe_appearance_h__


#include <plantgl/scenegraph/core/sceneobject.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


class Appearance;
typedef RCPtr<Appearance> AppearancePtr;
// typedef SymbolTable<AppearancePtr> AppearanceSymbolTable;
class AppearanceSymbolTable;


/* ------------------------------------------------------------------------- */

/**
    \class Appearance
    \brief Abstract base class for all appearance objects.
*/


class SG_API Appearance : public SceneObject
{

public:

  /// A structure which helps to build a Appearance when parsing.
  struct SG_API Builder : public SceneObject::Builder {

    /// A constructor. It inits all the pointers to null.
    Builder( );

    /// A destructor.
    virtual ~Builder( );

    /** Returns a pointer to a Geometry constructed using the values
        contained within \e self. If \e self is not valid, it returns a
        null pointer. */
    virtual SceneObjectPtr build( ) const = 0;

  };


  /// Default constructor.
  Appearance( ) ;

  /// Default constructor.
  Appearance(const std::string& name) ;

  /// Destructor
  virtual ~Appearance( );

  virtual bool isTexture() const;

}; // class Appearance

/// Appearance Pointer
typedef RCPtr<Appearance> AppearancePtr;


/* ------------------------------------------------------------------------- */

// __appe_appearance_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/// Write Appearance \b a in \b stream
// SG_API std::ostream& operator<<( std::ostream& stream, PGL(Appearance&) a );

/* ----------------------------------------------------------------------- */
#endif

