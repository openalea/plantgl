/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) 
 *
 *       $Source$
 *       $Id: vxl_tile.h 1703 2006-06-16 13:04:47Z fboudon $
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



/*! \file vxl_tile.h
    \brief Definition of Tile.
*/

#ifndef __vxl_tile_h__
#define __vxl_tile_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <tool/util_types.h>
#include <string>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class Tile
    \brief Base class for Multiscale Space Tile like Octree.\n
    - This class contains basic information about type of node in the hierarchy.\n
    - Sub class must refine this class with attribute that contains the \n
    top-down decomposition topology, the within-scale topology and other attributes.\n
    - getName() and Num attribute suppose that components of a complex can be ordered.\n
*/

/*! \enum Tile::TileType
  This enum specifies the Type of a Tile.
*/
/*! \var Tile::TileType Tile::Empty
   Value for Empty Tile
*/
/*! \var Tile::TileType Tile::Filled
   Value for Tile Filled by objects
*/
/*! \var Tile::TileType Tile::Undetermined
   Value for Undetermined Tile
*/
    

/* ----------------------------------------------------------------------- */

class ALGO_API Tile {

    public :

	
  enum TileType { Empty , Filled, Undetermined };
   

    /// Default constructor.
    Tile(Tile * Complex = NULL, 
	 const unsigned char Scale = 0, 
	 const TileType Type = Empty,
	 const unsigned char Num = 0);
	
    /// Destructor
    virtual ~Tile();
    
    /// Validity check.
    virtual bool isValid() const = 0;

    /// Return type of \e self.
    const TileType& getType() const{
	return __type;
    }

    /// Return type of \e self.
    TileType& getType() {
	return __type;
    }

    /// Return Name of \e self.
	std::string getName() const;

    /// Return id of \e self.
    const unsigned int getId() const {
	return (unsigned int)this;
    }

    /// Return num of \e self.
    const unsigned char& getNum() const {
	return __num;
    }

    /// Return num of \e self.
    unsigned char& getNum(){
	return __num;
    }

    /// Return scale of \e self.
    const unsigned char& getScale() const {
	return __scale;
    }

    /// Return scale of \e self.
    unsigned char& getScale(){
	return __scale;
    }

    /// Return pointee to self's \e father.
    inline const Tile * getComplex() const {
	return __Complex;
    }

    /// Return pointee to self's \e father.
    inline Tile * getComplex() {
	return __Complex;
    }

    /// Return whether \e self is decomposed.
    virtual bool isDecomposed() const = 0;
    
    /// Return the number of components of \e self.
    virtual uchar_t getComponentsSize() const = 0;
    
    protected :
    
    /// Identifiant of \e self.
    unsigned char __num;

    /// Scale of the node.
    unsigned char __scale;

    /// Type of \e self.
    TileType __type;

    /// pointee to \e complex.
    Tile * __Complex;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
