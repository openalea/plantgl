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






#include "triangleset.h"
#include "polyline.h"
#include "mesh_inline.h"


PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

/*
TriangleSet::Builder::Builder() :
  Mesh::Builder(),
  IndexList(0),
  NormalIndexList(0),
  ColorIndexList(0),
  TexCoordIndexList(0)
  {
}


TriangleSet::Builder::~Builder() {
  //nothing to do
}


SceneObjectPtr TriangleSet::Builder::build( ) const {
  if (isValid()){
    TriangleSet * t = new TriangleSet(*PointList,
                                      *IndexList,
                                      NormalList?*NormalList:Point3ArrayPtr(),
                                      NormalIndexList?*NormalIndexList:Index3ArrayPtr(),
                                      ColorList?*ColorList:Color4ArrayPtr(),
                                      ColorIndexList?*ColorIndexList:Index3ArrayPtr(),
                                      TexCoordList?*TexCoordList:Point2ArrayPtr(),
                                      TexCoordIndexList?*TexCoordIndexList:Index3ArrayPtr(),
                                      NormalPerVertex? *NormalPerVertex : DEFAULT_NORMALPERVERTEX,
                                      ColorPerVertex? *ColorPerVertex: DEFAULT_COLORPERVERTEX,
                                      CCW ? *CCW : DEFAULT_CCW,
                                      Solid ? *Solid : DEFAULT_SOLID,
                                      Skeleton ? *Skeleton : DEFAULT_SKELETON);
    return SceneObjectPtr(t);
  }
  return SceneObjectPtr();
}


void TriangleSet::Builder::destroy( ) {
  MeshDestroy();
  if (IndexList) delete IndexList;
  if (NormalIndexList) delete NormalIndexList;
  if (ColorIndexList) delete ColorIndexList;
  if (TexCoordIndexList) delete TexCoordIndexList;
}


bool TriangleSet::Builder::isValid( ) const {
  if(!MeshValid()) return false;

  // PointList size check
  uint_t _pointListSize = (*PointList)->size();
  if (_pointListSize < 3) {
    pglErrorEx(PGLERRORMSG(INVALID_FIELD_SIZE_sss),"TriangleSet","PointList","Must have more than 2 points.");
    return false;
  };

  // IndexList field
  if (! IndexList) {
    pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","IndexList");
    return false;
  };

  // IndexList size check
  uint_t _indexListSize = (*IndexList)->size();
  if (_indexListSize < 1) {
    pglErrorEx(PGLERRORMSG(INVALID_FIELD_SIZE_sss),"TriangleSet","IndexList","Number of Index must be greater than 0.");
    return false;
  };

  // IndexList values check
  for (uint_t _i = 0; _i < _indexListSize; _i++) {
    // Max index check
    if ((*(*IndexList)->getAt(_i).getMax()) >= _pointListSize) {
        pglDebugEx
            (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","IndexList",number(_i+1).c_str(),"Do not represent any point of the list.");
        return false;
    };
    // Unique elements check
    if (! (*IndexList)->getAt(_i).isUnique()) {
        pglDebugEx
            (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","IndexList",number(_i+1).c_str(),"Redundance : Already declared.");
    };
  };

  if (NormalList) {
      uint_t _normalListSize = (*NormalList)->size();
      if (!NormalIndexList){
          if(NormalPerVertex && !*NormalPerVertex){
              if(_normalListSize != _indexListSize){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalList","Number of normals must be compatible to IndexList size.");
                return false;
              }
          }
          else
          {
              if(_normalListSize != _pointListSize){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalList","Number of normals must be compatible to PointList size.");
                return false;
              }
          }
      }
      else {
          uint_t _normalIndexListSize = (*NormalIndexList)->size();
          if(NormalPerVertex && !*NormalPerVertex){
            pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalPerVertex","If NormalIndexList is specified, NormalPerVertex should be True.");
            return false;
          }
          else
          {
              if(_normalIndexListSize != _indexListSize){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalIndexList","Number of normals indices must be compatible to IndexList size.");
                return false;
              }
          }
          // NormalIndexList values check
          for (uint_t _i = 0; _i < _normalIndexListSize; _i++) {
            // Max index check
            if ((*(*NormalIndexList)->getAt(_i).getMax()) >= _normalListSize) {
                pglDebugEx
                    (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","NormalIndexList",number(_i+1).c_str(),"Do not represent any normal of the list.");
                return false;
            }
         }

      }
  }
  else
  {
      if (NormalIndexList){
        pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","NormalList");
        return false;
      }
  }

  if (ColorList) {
      uint_t _colorListSize = (*ColorList)->size();
      if (!ColorIndexList){
          if(ColorPerVertex && !*ColorPerVertex){
              if(_colorListSize != _indexListSize){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorList","Number of colors must be compatible to IndexList size.");
                return false;
              }
          }
          else
          {
              if(_colorListSize != _pointListSize){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorList","Number of colors must be compatible to PointList size.");
                return false;
              }
          }
      }
      else {
          uint_t _colorIndexListSize = (*ColorIndexList)->size();
          if(ColorPerVertex && !*ColorPerVertex){
            pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorPerVertex","If ColorIndexList is specified, ColorPerVertex should be True.");
            return false;
          }
          else
          {
              if(_colorIndexListSize != _indexListSize){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorIndexList","Number of colors indices must be compatible to IndexList size.");
                return false;
              }
          }
          // ColorIndexList values check
          for (uint_t _i = 0; _i < _colorIndexListSize; _i++) {
            // Max index check
            if ((*(*ColorIndexList)->getAt(_i).getMax()) >= _colorListSize) {
                pglDebugEx
                    (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","ColorIndexList",number(_i+1).c_str(),"Do not represent any color of the list.");
                return false;
            }
         }

      }
  }
  else
  {
      if (ColorIndexList){
        pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","ColorList");
        return false;
      }
  }

  if (TexCoordList) {
      uint_t _texCoordListSize = (*TexCoordList)->size();
      if (!TexCoordIndexList){
          if(_texCoordListSize != _pointListSize){
            pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","TexCoordList","Number of TexCoord must be compatible to PointList size.");
            return false;
          }
      }
      else {
          uint_t _texCoordIndexListSize = (*TexCoordIndexList)->size();
          if(_texCoordIndexListSize != _indexListSize){
            pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","TexCoordIndexList","Number of TexCoord indices must be compatible to IndexList size.");
            return false;
          }
          // TexCoordIndexList values check
          for (uint_t _i = 0; _i < _texCoordIndexListSize; _i++) {
            // Max index check
            if ((*(*TexCoordIndexList)->getAt(_i).getMax()) >= _texCoordListSize) {
                pglDebugEx
                    (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","TexCoordIndexList",number(_i+1).c_str(),"Do not represent any TexCoord of the list.");
                return false;
            }
         }

      }
  }
  else
  {
      if (TexCoordIndexList){
        pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","TexCoordList");
        return false;
      }
  }

  return true;
}
*/
/* ----------------------------------------------------------------------- */

/*
TriangleSet::TriangleSet() :
    Mesh(),
    __indexList(),
    __normalIndexList(),
    __texCoordIndexList(),
    __colorIndexList(){
}

TriangleSet::TriangleSet( const Point3ArrayPtr& points,
                          const Index3ArrayPtr& indices,
                          bool normalPerVertex,
                          bool ccw,
                          bool solid,
                          const PolylinePtr& skeleton ) :
    Mesh(points,normalPerVertex,ccw,solid,skeleton),
    __indexList(indices),
    __normalIndexList(),
    __texCoordIndexList(),
    __colorIndexList(){
    GEOM_ASSERT(isValid());
}

TriangleSet::TriangleSet( const Point3ArrayPtr& points,
                          const Index3ArrayPtr& indices,
                          const Point3ArrayPtr& normals ,
                          const Index3ArrayPtr& nomalIndices,
                          const Color4ArrayPtr& colors,
                          const Index3ArrayPtr& colorIndices,
                          const Point2ArrayPtr& texCoord,
                          const Index3ArrayPtr& texCoordIndices,
                          bool normalPerVertex,
                          bool colorPerVertex,
                          bool ccw,
                          bool solid,
                          const PolylinePtr& skeleton) :
    Mesh(points,normals,colors,texCoord,normalPerVertex,colorPerVertex,ccw,solid,skeleton),
    __indexList(indices),
    __normalIndexList(nomalIndices),
    __colorIndexList(colorIndices),
    __texCoordIndexList(texCoordIndices){
    GEOM_ASSERT(isValid());
} */

TriangleSet::TriangleSet() :
    IndexedMesh<Index3Array>(){
}

TriangleSet::TriangleSet( const Point3ArrayPtr& points,
                          const Index3ArrayPtr& indices,
                          bool normalPerVertex,
                          bool ccw,
                          bool solid,
                          const PolylinePtr& skeleton ) :
    IndexedMesh<Index3Array>(points,indices,normalPerVertex,ccw,solid,skeleton){
}

TriangleSet::TriangleSet( const Point3ArrayPtr& points,
                          const Index3ArrayPtr& indices,
                          const Point3ArrayPtr& normals ,
                          const Index3ArrayPtr& nomalIndices,
                          const Color4ArrayPtr& colors,
                          const Index3ArrayPtr& colorIndices,
                          const Point2ArrayPtr& texCoord,
                          const Index3ArrayPtr& texCoordIndices,
                          bool normalPerVertex,
                          bool colorPerVertex,
                          bool ccw,
                          bool solid,
                          const PolylinePtr& skeleton) :
    IndexedMesh<Index3Array>(points,indices,normals,nomalIndices,
                             colors,colorIndices,texCoord,texCoordIndices,
                             normalPerVertex,colorPerVertex,ccw,solid,skeleton){
}

TriangleSet::~TriangleSet( ) {
}

bool TriangleSet::isValid() const { return isAValidMesh<TriangleSet>(); }


SceneObjectPtr
TriangleSet::copy(DeepCopier& copier) const{
  return SceneObjectPtr(mesh_copy<TriangleSet>(*this,copier));
}

ExplicitModelPtr
TriangleSet::transform( const Transformation3DPtr& transformation ) const {
  return mesh_transform<TriangleSet>(*this,transformation);
}

/* ----------------------------------------------------------------------- */

