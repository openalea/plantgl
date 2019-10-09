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




/*! \file vxl_tile.h
    \brief Definition of Tile.
*/

#ifndef __vxl_tile_h__
#define __vxl_tile_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/tool/util_types.h>
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
    const size_t getId() const {
    return (size_t)this;
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
