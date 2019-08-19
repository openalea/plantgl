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





#include "quadset.h"
#include "mesh_inline.h"


PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


QuadSet::QuadSet() : IndexedMesh<Index4Array>(){ }

QuadSet::QuadSet( const Point3ArrayPtr& points,
                  const Index4ArrayPtr& indices,
                  bool normalPerVertex,
                  bool ccw,
                  bool solid,
                  const PolylinePtr& skeleton ) :
  IndexedMesh<Index4Array>(points,indices,normalPerVertex,ccw,solid,skeleton){}

QuadSet::QuadSet( const Point3ArrayPtr& points,
                  const Index4ArrayPtr& indices,
                  const Point3ArrayPtr& normals ,
                  const Index4ArrayPtr& nomalIndices,
                  const Color4ArrayPtr& colors,
                  const Index4ArrayPtr& colorIndices,
                  const Point2ArrayPtr& texCoord,
                  const Index4ArrayPtr& texCoordIndices,
                  bool normalPerVertex,
                  bool colorPerVertex,
                  bool ccw,
                  bool solid,
                  const PolylinePtr& skeleton) :
    IndexedMesh<Index4Array>(points,indices,normals,nomalIndices,colors,colorIndices,
         texCoord,texCoordIndices,normalPerVertex,colorPerVertex,ccw,solid,skeleton){ }


QuadSet::~QuadSet( ) {
}

SceneObjectPtr
QuadSet::copy(DeepCopier& copier) const{
  return SceneObjectPtr(mesh_copy<QuadSet>(*this,copier));
}

bool QuadSet::isValid( ) const {
  return isAValidMesh<QuadSet>();
}


ExplicitModelPtr
QuadSet::transform( const Transformation3DPtr& transformation ) const
{ return mesh_transform<QuadSet>(*this,transformation); }

/* ----------------------------------------------------------------------- */
