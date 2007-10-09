%{/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr)
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


#ifdef _WIN32
#include <malloc.h>
#define alloca _alloca
#endif


#include "scne_parser.h"


#undef yyFlexLexer
#define yyFlexLexer scne_yyFlexLexer

#define scne_yyerror(_msg) {			\
        yyerrok; \
        yyclearin; \
        parser(p); \
        if (!(p.handleError(std::string(_msg), \
                      yychar, \
			    ""))) YYABORT;\
	postream(p)<<std::endl;}
//                      yytname[YYTRANSLATE((yychar >= 0 ? yychar : -yychar))]))) YYABORT;


#include <plantgl/scenegraph/core/pgl_messages.h>
#include "scne_scanner.h"
#include <plantgl/tool/gparser.h>
#include <plantgl/scenegraph/core/smbtable.h>
#include "scne_binaryparser.h"

#include <plantgl/pgl_scenegraph.h>

#include "printer.h"
#include <plantgl/math/util_math.h>
#include <plantgl/tool/util_enviro.h>
#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/readline.h>

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE

#include <list>
#include <typeinfo>

#define SMB_TABLE_TYPE SceneObjectPtr

/*  ---------------------------------------------------------------------- */

#ifdef USE_READLINE

static char * sh_keyword[] = {
    ":Echo",
    ":Include",
    ":List",
    ":Quit",
    "AmapSymbol",
    "AsymmetricHull",
    "AxisRotated",
    "BezierCurve",
    "BezierCurve2D",
    "BezierPatch",
    "Box",
    "Cone",
    "Cylinder",
    "Disc",
    "ElevationGrid",
    "EulerRotated",
    "ExtrudedHull",
    "Extrusion",
    "FaceSet",
    "Frustum",
    "Group",
    "IFS",
    "Inline",
    "Material",
    "MonoSpectral",
    "MultiSpectral",
    "NurbsCurve",
    "NurbsCurve2D",
    "NurbsPatch",
    "Oriented",
    "Paraboloid",
    "PointSet",
    "PointSet2D",
    "Polyline",
    "Polyline2D",
    "QuadSet",
    "Revolution",
    "Swung",
    "Shape",
    "Scaled",
    "Sphere",
    "Tapered",
    "Translated",
    "TriangleSet",
    (char *)NULL
};

 static char *cone_att_keyword[] = { "Radius", "Height", "Solid", "Slices", (char *)NULL };
 static char *shape_att_keyword[] = { "Appearance", "Id", "Geometry", (char *)NULL };
 static char *inline_att_keyword[] = { "BBoxCenter","BBoxSize", "FileName", "Scene", (char *)NULL };
 static char *ashull_att_keyword[] = { "NegXHeight", "NegXRadius",
                                        "NegYRadius", "NegYHeight",
                                        "PosXRadius", "PosXHeight",
                                        "PosYRadius","PosYHeight",
                                        "Bottom","BottomShape",
                                        "Top","Topshape",
                                        "Slices","Stacks",
                                        (char *)NULL };

#endif

/*  ---------------------------------------------------------------------- */

#define GEOM_PARSER_BUILD_OBJECT(_type, shape,_name,builder) \
  GEOM_ASSERT(builder); \
  _type##Ptr _shape; \
  _shape.cast(builder->build()); \
  builder->destroy(); \
  delete builder; \
  if (_shape) { \
    if (_name) { \
      cursmbtable(t); \
      SymbolTable<SceneObjectPtr>::iterator _i = t.find(*_name); \
      if (_i != t.end()) { \
        genMessage(WARNINGMSG(DECLARED_OBJECT_ss),typeid(*shape).name(),_name->c_str()); \
        shape = NULL; \
        delete _name; \
        yyerror("Object Already declared"); \
      } \
      else { \
        _shape->setName(*_name); \
        t[*_name] = _shape; \
        delete _name; \
      }; \
    } \
    shape = new _type##Ptr(_shape); \
  } \
  else { \
    if(_name)delete _name; \
    shape = NULL; \
    yyerror("Object not Valid"); \
  }


/*  ---------------------------------------------------------------------- */


#define GEOM_PARSER_BUILD_SHAPE(shape,_name,builder) \
  GEOM_ASSERT(builder); \
  SceneObjectPtr _shape(builder->build()); \
  builder->destroy(); \
  delete builder; \
  if (_shape) { \
    if(!_name) { \
      const std::string name = _shape->getName(); \
      cursmbtable(t); \
      SymbolTable<SceneObjectPtr>::iterator _i = t.find(name); \
      if (_i != t.end()) { \
        genMessage(WARNINGMSG(DECLARED_OBJECT_ss),"Shape",name.c_str()); \
        shape = NULL; \
        yyerror("A Shape with the same Geometry has already been declared"); \
      } \
      else { \
        t[name] = _shape; \
      }; \
    } \
    else { \
      cursmbtable(t); \
      SymbolTable<SceneObjectPtr>::iterator _i = t.find(*_name); \
      if (_i != t.end()) { \
        genMessage(WARNINGMSG(DECLARED_OBJECT_ss),typeid(*shape).name(),_name->c_str()); \
        shape = NULL; \
        delete _name; \
        yyerror("Object Already declared"); \
      } \
      else { \
        _shape->setName(*_name); \
        t[*_name] = _shape; \
        delete _name; \
      }; \
    } \
    shape = new SceneObjectPtr(_shape); \
  } \
  else { \
    shape = NULL; \
    if(_name)delete _name; \
    yyerror("Object not valid "); \
  }



/*  ---------------------------------------------------------------------- */

#define GEOM_PARSER_SET_FIELD(builder,field,value){ \
  GEOM_ASSERT(builder); \
  if (value) { \
    if (! builder->field) \
      builder->field = value; \
    else { \
      genMessage(ERRORMSG(INITIALIZED_FIELD_ss),typeid(*builder).name(),#field); \
      yyerror("Field not valid "); \
      delete value; \
    } \
  } \
  else { \
      genMessage(ERRORMSG(INVALID_FIELD_VALUE_sss),(typeid(*builder).name()),#field,""); \
      yyerror("Field not valid "); \
    }  \
  }



#define GEOM_PARSER_CREATE_ARRAY(type,list,result) { \
    if (list) { \
      result = new type##Ptr(new type(list->begin(),list->end())); \
      delete list; \
    } \
    else result = NULL; \
  }


#define GEOM_PARSER_ADD_LIST(list,value,result) { \
    if ((list) && (value)) { \
      list->push_back(*value); \
      delete value; \
      result = list; \
    } \
    else { \
      if (list) delete list; \
      if (value) delete value; \
      result = NULL; \
    } \
  }


#define GEOM_PARSER_INIT_LIST(type,first,result) { \
    if (first) { \
      result = new std::list<type>(1,*first); \
      delete first; \
    } \
    else result = NULL; \
  }

#define GEOM_PARSER_CREATE_MATRIX(type,matrix,result) { \
    if (matrix) { \
      result = new type##Ptr(new type(matrix->getBegin(), \
                                      matrix->getEnd(), \
                                      matrix->getRowsNb())); \
      delete matrix; \
    } \
    else result = NULL; \
  }



#define GEOM_PARSER_ADD_MATRIX(matrix,row,result) { \
    if (!(matrix->isEmpty()) && (row)) { \
      matrix->pushRow(row->begin(),row->end()); \
      delete row; \
      result = matrix; \
    } \
    else { \
      if (matrix) delete matrix; \
      if (row) delete row; \
      result = NULL; \
    } \
  }


#define GEOM_PARSER_INIT_MATRIX(type,first,result) { \
    if (first) { \
      result = new type(first->begin(),first->end(),1); \
      delete first; \
    } \
    else result = NULL; \
  }

#define GEOM_PARSER_BUILD_TRANSFO(_type, shape, builder) \
  GEOM_ASSERT(builder); \
  _type##Ptr _shape(builder->build()); \
  builder->destroy(); \
  delete builder; \
  if (_shape) { \
    shape = new _type##Ptr(_shape); \
  } \
  else shape = NULL; \

/* ----------------------------------------------------------------------- */

static bool __verbose = true;

static int shape_nb = 0;

static std::vector<SymbolTable<SMB_TABLE_TYPE> *> symbolstack((unsigned int)0);

#define cursmbtable(t) \
   SymbolTable<SceneObjectPtr>& t = *(symbolstack[symbolstack.size()-1]);



%}

%pure_parser        /* asks for a reentrant parser: don't remove ! */
%token_table        /* generates a token table (names of the tokens) */

/* possible types of tokens returned by the lexer */

%union {
  /* basic types readed with the lexer */
  bool *                       bool_t;
  int32_t *                    int32_o;
  real_t *                     real_o;
  std::string *                string_t;
  /* extended types */
  uchar_t *                    uchar_o;
  uint32_t *                   uint32_o;
  std::list<uint32_t> *        uint32_l;
  Color3 *                     color3_t;
  Color4 *                     color4_t;
  std::list<Color4> *          color4_l;
  Color4ArrayPtr *             color4_a;
  Index3 *                     index3_t;
  std::list<Index3> *          index3_l;
  Index3ArrayPtr *             index3_a;
  Index4 *                     index4_t;
  std::list<Index4> *          index4_l;
  Index4ArrayPtr *             index4_a;
  Index *                      index_t;
  std::list<Index> *           index_l;
  IndexArrayPtr *              index_a;
  std::list<real_t> *          real_l;
  RealArrayPtr *               real_a;
  Array2<real_t> *             real_m;
  RealArray2Ptr *              real_mp;
  TOOLS(Vector2) *             vector2_t;
  std::list<TOOLS(Vector2)> *  vector2_l;
  Point2ArrayPtr *             vector2_a;
  Array2<TOOLS(Vector2)> *     vector2_m;
  Point2MatrixPtr *            vector2_mp;
  TOOLS(Vector3) *             vector3_t;
  std::list<TOOLS(Vector3)> *  vector3_l;
  Point3ArrayPtr *             vector3_a;
  Array2<TOOLS(Vector3)> *     vector3_m;
  Point3MatrixPtr *            vector3_mp;
  TOOLS(Vector4) *             vector4_t;
  std::list<TOOLS(Vector4)> *  vector4_l;
  Point4ArrayPtr *             vector4_a;
  Array2<TOOLS(Vector4)> *     vector4_m;
  Point4MatrixPtr *            vector4_mp;

  // Transformation object
  Transform4Ptr *              transfo4_t;
  std::list<Transform4Ptr>*    transfo4_l;
  Transform4ArrayPtr *         transfo4_a;
  OrthonormalBasis3DPtr *      orthobase_t;
  // Transformation builder
  Transform4::Builder *        transfo4_b;
  AxisRotation::Builder *      axisrotation_b;
  EulerRotation::Builder *     eulerrotation_b;
  BaseOrientation::Builder *   baseorientation_b;

  // Scene Object
  SceneObjectPtr *             sceneobject_o;
  ScenePtr *                   scene_o;
  // Shape Builder
  Shape::Builder *         Shape_b;
  Inline::Builder *        inline_b;
  // Appearance objects
  AppearancePtr *              appearance_o;
  // Appearance builders
  Material::Builder *          material_b;
  MonoSpectral::Builder *      monoSpectral_b;
  MultiSpectral::Builder *     multiSpectral_b;
  ImageTexture::Builder *      imageTexture_b;
  // Geometry objects
  GeometryPtr *                geometry_o;
  GeometryArrayPtr *           geometry_a;
  std::list<GeometryPtr> *     geometry_l;
  PolylinePtr *            polyline_o;
  PrimitivePtr *               primitive_o;
  LineicModelPtr *             curve_o;
  Curve2DPtr *                 curve2D_o;
  Curve2DArrayPtr *            curve2D_a;
  std::list<Curve2DPtr> *      curve2D_l;
  FontPtr *					   font_o;

  // Geometry builders
  AmapSymbol::Builder *        amapSymbol_b;
  AsymmetricHull::Builder *    asymmetricHull_b;
  AxisRotated::Builder *       axisRotated_b;
  Box::Builder *               box_b;
  BezierCurve::Builder *       bezierCurve_b;
  BezierCurve2D::Builder *     bezierCurve2D_b;
  BezierPatch::Builder *       bezierPatch_b;
  Cone::Builder *              cone_b;
  Cylinder::Builder *          cylinder_b;
  Disc::Builder *              disc_b;
  ElevationGrid::Builder *     elevationGrid_b;
  EulerRotated::Builder *      eulerRotated_b;
  ExtrudedHull::Builder *      extrudedHull_b;
  FaceSet::Builder *           faceSet_b;
  Frustum::Builder *           frustum_b;
  Extrusion::Builder *         extrusion_b;
  Group::Builder *             group_b;
  IFS::Builder *               ifs_b;
  NurbsCurve::Builder *        nurbsCurve_b;
  NurbsCurve2D::Builder *      nurbsCurve2D_b;
  NurbsPatch::Builder *        nurbsPatch_b;
  Oriented::Builder *          oriented_b;
  Paraboloid::Builder *        paraboloid_b;
  PointSet::Builder *          pointSet_b;
  PointSet2D::Builder *        pointSet2D_b;
  Polyline::Builder *      polyline_b;
  Polyline2D::Builder *    polyline2D_b;
  QuadSet::Builder *           quadSet_b;
  Revolution::Builder *        revolution_b;
  Swung::Builder *             swung_b;
  Scaled::Builder *            scaled_b;
  Sphere::Builder *            sphere_b;
  Tapered::Builder *           tapered_b;
  Translated::Builder *        translated_b;
  TriangleSet::Builder *       triangleSet_b;
  Text::Builder *			   text_b;
  Font::Builder *			   font_b;
}


/* association of types to tokens and non terminals */

%token <int32_o>       TokInt
%token <bool_t>        TokBool
%token <real_o>        TokReal
%token <string_t>      TokName
%token <string_t>      TokFile

%token TokShape
%token TokInline

// %token TokLambertian
%token TokMaterial
%token TokMonoSpectral
%token TokMultiSpectral
%token TokImageTexture

%token TokAmapSymbol
%token TokAsymmetricHull
%token TokAxisRotated
%token TokBezierCurve
%token TokBezierCurve2D
%token TokBezierPatch
%token TokBox
%token TokCone
%token TokCylinder
%token TokDisc
%token TokElevationGrid
%token TokEulerRotated
%token TokExtrudedHull
%token TokFaceSet
%token TokFrustum
%token TokExtrusion
%token TokGroup
%token TokIFS
%token TokNurbsCurve
%token TokNurbsCurve2D
%token TokNurbsPatch
%token TokOriented
%token TokParaboloid
%token TokPointSet
%token TokPointSet2D
%token TokPolyline
%token TokPolyline2D
%token TokQuadSet
%token TokRevolution
%token TokSwung
%token TokScaled
%token TokSphere
%token TokTapered
%token TokTranslated
%token TokTriangleSet
%token TokText
%token TokFont


%token TokAppearance
%token TokGeometry
%token TokScene

%token TokAmbient
%token TokDiffuse
%token TokEmission
%token TokFilter
%token TokReflectance
%token TokShininess
%token TokSpecular
%token TokTransmittance
%token TokTransparency

%token TokBlack
%token TokBlue
%token TokCyan
%token TokGreen
%token TokMagenta
%token TokRed
%token TokWhite
%token TokYellow

%token TokAngle
%token TokAngleList
%token TokAxis
%token TokAzimuth
%token TokBaseRadius
%token TokBBoxCenter
%token TokBBoxSize
%token TokBold
%token TokBottom
%token TokBottomShape
%token TokCCW
%token TokCrossSection
%token TokCtrlPointList
%token TokCtrlPointMatrix
%token TokColorList
%token TokColorIndexList
%token TokColorPerVertex
%token TokDegree
%token TokDepth
%token TokGeometryList
%token TokElevation
%token TokFamily
%token TokFileName
%token TokFontStyle
%token TokHeight
%token TokHeightList
%token TokHorizontal
%token TokId
%token TokItalic
%token TokIndexList
%token TokKnotList
%token TokMipmaping
%token TokNegXHeight
%token TokNegXRadius
%token TokNegYHeight
%token TokNegYRadius
%token TokNormalList
%token TokNormalIndexList
%token TokNormalPerVertex
%token TokOrientation
%token TokParentId
%token TokPointList
%token TokPosition
%token TokPosXHeight
%token TokPosXRadius
%token TokPosYHeight
%token TokPosYRadius
%token TokPrimary
%token TokPrimitive
%token TokProfile
%token TokProfileList
%token TokRadius
%token TokRoll
%token TokScale
%token TokScreenCoordinates
%token TokSecondary
%token TokShapeFactor
%token TokSize
%token TokSkeleton
%token TokSlices
%token TokSolid
%token TokStacks
%token TokStride
%token TokString
%token TokTransfo
%token TokTransfoList
%token TokVertical
%token TokTaper
%token TokTexCoordList
%token TokTexCoordIndexList
%token TokTop
%token TokTopShape
%token TokTopRadius
%token TokTranslation
%token TokUDegree
%token TokUKnotList
%token TokUStride
%token TokVDegree
%token TokVKnotList
%token TokVStride
//%token TokXDim
%token TokXSpacing
//%token TokYDim
%token TokYSpacing

// %token TokRotation
%token TokAxisRotation
%token TokEulerRotation
%token TokBaseOrientation


%token TokPi
%token TokPow
%token TokCos
%token TokSin
%token TokTan
%token TokACos
%token TokASin
%token TokATan
%token TokDot
%token TokAngleF
%token TokCross
%token TokNorm
%token TokDirection
%token TokOX
%token TokOY
%token TokOZ

%token TokQuit
%token TokList
%token <string_t> TokBinaryInclude

%left ','
%left TokMINUS TokPLUS
%left TokTIMES TokSLASH
%left UMINUS
%left UPLUS
%token TokError

%type <sceneobject_o>       SceneObj

/* Shape Class */
%type <sceneobject_o>       ShapeObj
%type <sceneobject_o>       InlineObj
%type <scene_o>             SceneDesc
%type <geometry_o>          GeometryObj
%type <geometry_o>          GeomObj
%type <appearance_o>        AppearanceObj

/* Geometric Class */
%type <geometry_o>          PrimitiveObj
%type <geometry_o>          TransformedObj
%type <geometry_o>          GroupObj
%type <curve2D_o>           Curve2DObj
%type <curve_o>             CurveObj
%type <sceneobject_o>       Command

/* Appearance */
%type <appearance_o>        MaterialObj
%type <appearance_o>        MonoSpectralObj
%type <appearance_o>        MultiSpectralObj
%type <appearance_o>        PhysicalModelObj
%type <appearance_o>        TextureObj

/* Primitives */
%type <geometry_o>          AmapSymbolObj
%type <geometry_o>          AsymmetricHullObj
%type <geometry_o>          BoxObj
%type <geometry_o>          BezierCurveObj
%type <geometry_o>          BezierCurve2DObj
%type <geometry_o>          BezierPatchObj
%type <geometry_o>          ConeObj
%type <geometry_o>          CylinderObj
%type <geometry_o>          DiscObj
%type <geometry_o>          ElevationGridObj
%type <geometry_o>          ExtrudedHullObj
%type <geometry_o>          FaceSetObj
%type <geometry_o>          FrustumObj
%type <geometry_o>          ExtrusionObj
%type <geometry_o>          NurbsCurveObj
%type <geometry_o>          NurbsCurve2DObj
%type <geometry_o>          NurbsPatchObj
%type <geometry_o>          ParaboloidObj
%type <geometry_o>          PointSetObj
%type <geometry_o>          PointSet2DObj
%type <geometry_o>          PolylineObj
%type <geometry_o>          Polyline2DObj
%type <geometry_o>          QuadSetObj
%type <geometry_o>          RevolutionObj
%type <geometry_o>          SwungObj
%type <geometry_o>          SphereObj
%type <geometry_o>          TriangleSetObj

/* Transformed */
%type <geometry_o>          AxisRotatedObj
%type <geometry_o>          EulerRotatedObj
%type <geometry_o>          IFSObj
%type <geometry_o>          OrientedObj
%type <geometry_o>          ScaledObj
%type <geometry_o>          TaperedObj
%type <geometry_o>          TranslatedObj

/* Text */
%type <geometry_o>          TextObj
%type <font_o>				FontObj

/* Shape builder */
%type <Shape_b>         ShapeFieldList
%type <Shape_b>         ShapeShortFieldList
%type <inline_b>            InlineFieldList

/* Appearance builders */
%type <material_b>          MaterialFieldList
%type <monoSpectral_b>      MonoSpectralFieldList
%type <multiSpectral_b>     MultiSpectralFieldList
%type <imageTexture_b>      ImageTextureFieldList

/* Geom builders */
%type <amapSymbol_b>        AmapSymbolFieldList
%type <asymmetricHull_b>    AsymmetricHullFieldList
%type <axisRotated_b>       AxisRotatedFieldList
%type <box_b>               BoxFieldList
%type <bezierCurve_b>       BezierCurveFieldList
%type <bezierCurve2D_b>     BezierCurve2DFieldList
%type <bezierPatch_b>       BezierPatchFieldList
%type <cone_b>              ConeFieldList
%type <disc_b>              DiscFieldList
%type <cylinder_b>          CylinderFieldList
%type <elevationGrid_b>     ElevationGridFieldList
%type <eulerRotated_b>      EulerRotatedFieldList
%type <extrudedHull_b>      ExtrudedHullFieldList
%type <faceSet_b>           FaceSetFieldList
%type <frustum_b>           FrustumFieldList
%type <paraboloid_b>        ParaboloidFieldList
%type <extrusion_b>         ExtrusionFieldList
%type <group_b>             GroupFieldList
%type <ifs_b>               IFSFieldList
%type <nurbsCurve_b>        NurbsCurveFieldList
%type <nurbsCurve2D_b>      NurbsCurve2DFieldList
%type <nurbsPatch_b>        NurbsPatchFieldList
%type <oriented_b>          OrientedFieldList
%type <pointSet_b>          PointSetFieldList
%type <pointSet2D_b>        PointSet2DFieldList
%type <polyline_b>          PolylineFieldList
%type <polyline2D_b>        Polyline2DFieldList
%type <quadSet_b>           QuadSetFieldList
%type <revolution_b>        RevolutionFieldList
%type <swung_b>             SwungFieldList
%type <scaled_b>            ScaledFieldList
%type <sphere_b>            SphereFieldList
%type <tapered_b>           TaperedFieldList
%type <translated_b>        TranslatedFieldList
%type <triangleSet_b>       TriangleSetFieldList
%type <text_b>				TextFieldList
%type <font_b>				FontFieldList

//%type <profilescaling_b>    ProfileScalingFieldList

/* References */
%type <appearance_o>        AppearanceRef
%type <geometry_o>          GeometryRef

/* types */
%type <real_o>              Angle
%type <real_a>              AngleArray
%type <real_l>              AngleList
%type <color3_t>            Color3
%type <color4_t>            Color4
%type <color4_a>            Color4Array
%type <color4_l>            Color4List
%type <curve2D_a>           Curve2DArray
%type <curve2D_l>           Curve2DList
%type <vector4_a>           CtrlPointArray
%type <vector3_a>           CtrlPointArray2D
%type <vector4_mp>          CtrlPointMatrix
%type <index_t>             Index
%type <index_a>             IndexArray
%type <index_l>             IndexList
%type <index3_t>            Index3
%type <index3_a>            Index3Array
%type <index3_l>            Index3List
%type <index4_t>            Index4
%type <index4_a>            Index4Array
%type <index4_l>            Index4List
%type <geometry_a>          GeometryArray
%type <geometry_l>          GeometryList
%type <string_t>            Name
%type <string_t>            Filename
%type <vector2_a>           Point2Array
%type <vector3_a>           Point3Array
%type <vector4_a>           Point4Array
%type <vector3_a>           Normal3Array
//%type <vector2_mp>        Point2Matrix
%type <vector3_mp>          Point3Matrix
%type <vector4_mp>          Point4Matrix
//%type <vector2_m>         Point2ArrayList
%type <vector3_m>           Point3ArrayList
%type <vector4_m>           Point4ArrayList
%type <primitive_o>         Primitive
%type <real_o>              Real
%type <real_o>              RealAtom
%type <real_a>              RealArray
%type <real_l>              RealList
%type <real_m>              RealArrayList
%type <real_mp>             RealMatrix
%type <polyline_o>          Skeleton
%type <int32_o>             Integer
%type <uchar_o>             Uchar
%type <uint32_o>            Uint32
%type <uint32_l>            Uint32List
%type <vector2_t>           Vector2
%type <vector2_t>           Vector2Atom
%type <vector2_l>           Vector2List
%type <vector3_t>           Vector3
%type <vector3_t>           Vector3Atom
%type <vector3_l>           Vector3List
%type <vector4_t>           Vector4
%type <vector4_t>           Vector4Atom
%type <vector4_l>           Vector4List
%type <orthobase_t>         Rotation
%type <orthobase_t>         AxisRotation
%type <orthobase_t>         EulerRotation
%type <orthobase_t>         BaseOrientation
%type <axisrotation_b>      AxisRotationFieldList
%type <eulerrotation_b>     EulerRotationFieldList
%type <baseorientation_b>   BaseOrientationFieldList
%type <transfo4_t>          Transfo
%type <transfo4_a>          TransfoArray
%type <transfo4_l>          TransfoList
%type <transfo4_b>          TransfoFieldList


%start SceneFile

%%

SceneFile :
 SceneObjList {
    symbolstack.pop_back();
 }
;

SceneObjList :
   SceneObjList SceneObj{
    if ($2) {
      if ((*$2)->isNamed()) {
#ifdef PGL_DEBUG
        parser(p); \
        Printer printer(postream(p));
        (*$2)->apply(printer);
        postream(p) << std::endl;
#endif
      }
      else {
        genMessage(WARNINGMSG(UNNAMED_OBJECT));
        delete $2;
        YYERROR;
      };
      delete $2;
    }
#ifdef USE_READLINE
    lexer(l);
    if(l.useReadline()) setKeyword(sh_keyword);
#endif
  }
| /* empty */{
    shape_nb = 0;
    parser(p);
    smbtable(p,t);
    symbolstack.push_back(&t);
#ifdef USE_READLINE
    lexer(l);
    if(l.useReadline()) setKeyword(sh_keyword);
#endif
};


SceneDesc :
  '{' SceneObjects '}' {
    cursmbtable(t);
    symbolstack.pop_back();
    SymbolTable<SceneObjectPtr> * t2 = &t;
    SceneObjectSymbolTable * smbt = (SceneObjectSymbolTable *)(t2);
    if(smbt != NULL)
      $$ = new ScenePtr(new Scene(*smbt));
    else
      $$ = NULL;
};

SceneObjects :
 SceneObjects SceneObj {
    if ($2) {
      if ((*$2)->isNamed()) {
#ifdef PGL_DEBUG
        parser(p);
        Printer printer(postream(p));
        (*$2)->apply(printer);
        postream(p) << std::endl;
#endif
      }
      else {
        genMessage(WARNINGMSG(UNNAMED_OBJECT));
        delete $2;
        YYERROR;
      };
      delete $2;
    }
  }
|   {
    shape_nb = 0;
    SymbolTable<SMB_TABLE_TYPE> * t = new SymbolTable<SMB_TABLE_TYPE>;
    symbolstack.push_back(t);
#ifdef USE_READLINE
    lexer(l);
    if(l.useReadline()) setKeyword(sh_keyword);
#endif
};



SceneObj:
   GeomObj            { if($1){ $$ = new SceneObjectPtr(*$1);delete $1; }else $$ = NULL;  }
 | AppearanceObj       { if($1){ $$ = new SceneObjectPtr(*$1);delete $1; }else $$ = NULL;  }
 | ShapeObj            { $$ = $1; }
 | InlineObj           { $$ = $1; }
 | Command             { $$ = NULL; }
 | error '}' {
   GEOM_TRACE("scne_parser : resynchronized");
   $$ = 0;
 };

AppearanceObj:
   MaterialObj { $$ = $1; }
 | TextureObj {  $$ = $1; }
 | PhysicalModelObj {  $$ = $1; }
 | AppearanceRef { $$ = $1; };

GeometryObj:
   GeomObj { $$ = $1; }
 | GeometryRef { $$ = $1; }
 | error '}' {
   GEOM_TRACE("scne_parser : resynchronized");
   $$ = 0;
 };

GeomObj:
   PrimitiveObj { $$ = $1; }
 | TransformedObj {  $$ = $1; }
 | GroupObj { $$ = $1; }
 | TextObj { $$ = $1; };


/**
 **  Shape object
 **/

ShapeObj:
  TokShape Name '{' ShapeFieldList '}' {
    GEOM_PARSER_BUILD_SHAPE($$,$2,$4);shape_nb++;
    if(__verbose)
      if(shape_nb%10==0)std::cerr << "\x0d" << "Already parsed : " << shape_nb << " shapes." << std::flush;
  }
| TokShape  Name '{' ShapeShortFieldList '}'{
  GEOM_PARSER_BUILD_SHAPE($$,$2,$4);shape_nb++;
  if(__verbose)
    if(shape_nb%10==0)std::cerr << "\x0d" << "Already parsed : " << shape_nb << " shapes." << std::flush;
};

InlineObj:
  TokInline Name '{' InlineFieldList '}' {
      int shpe_nb = shape_nb;
      GEOM_PARSER_BUILD_SHAPE($$,$2,$4);
      shape_nb += shpe_nb;
  }
;

InlineFieldList:
  InlineFieldList TokFileName Filename {
      GEOM_ASSERT($1);
      if ($3) {
          if (! $1->FileName || ! $1->Scene )
              $1->FileName = $3;
          else if ( $1->FileName) {
              genMessage(ERRORMSG(INITIALIZED_FIELD_ss),typeid(*$1).name(),"FileName");
              yyerror("Field not valid ");
              //postream(p) << std::endl;
              delete $3;
          }
          else {
              genMessage(ERRORMSG(INITIALIZED_FIELD_ss),typeid(*$1).name(),"Scene");
              yyerror("Field not valid ");
              //postream(p) << std::endl;
              delete $3;
          }
      }
      else {
          genMessage(ERRORMSG(INVALID_FIELD_VALUE_sss),(typeid(*$1).name()),"Filename","");
          yyerror("Field not valid ");
          //postream(p) << std::endl;
      }
      $$=$1;
//      GEOM_PARSER_SET_FIELD($1,FileName,$3);
  }
 | InlineFieldList TokScene SceneDesc {
      GEOM_ASSERT($1);
      if ($3) {
          if (! $1->FileName || ! $1->Scene )
              $1->Scene = $3;
          else if ( $1->Scene) {
              genMessage(ERRORMSG(INITIALIZED_FIELD_ss),typeid(*$1).name(),"Scene");
              yyerror("Field not valid ");
              //postream(p) << std::endl;
              delete $3;
          }
          else {
              genMessage(ERRORMSG(INITIALIZED_FIELD_ss),typeid(*$1).name(),"FileName");
              yyerror("Field not valid ");
              //postream(p) << std::endl;
              delete $3;
          }
      }
      else {
          genMessage(ERRORMSG(INVALID_FIELD_VALUE_sss),(typeid(*$1).name()),"Scene","");
          yyerror("Field not valid ");
          //postream(p) << std::endl;
      }
      $$=$1;
//     GEOM_PARSER_SET_FIELD($1,Scene,$3);
   }
 | InlineFieldList TokTranslation Vector3 {
     GEOM_PARSER_SET_FIELD($1,Translation,$3); $$=$1;
   }
 | InlineFieldList TokScale Vector3 {
     GEOM_PARSER_SET_FIELD($1,Scale,$3); $$=$1;
   }
 | {
   $$ = new Inline::Builder;
#ifdef USE_READLINE
   lexer(l);
   if(l.useReadline())setKeyword(inline_att_keyword);
#endif
 }
;

ShapeFieldList:
   ShapeFieldList TokId Uint32 {
       GEOM_PARSER_SET_FIELD($1,Id,$3); $$=$1;
   }
   ShapeFieldList TokParentId Uint32 {
       GEOM_PARSER_SET_FIELD($1,ParentId,$3); $$=$1;
   }
 | ShapeFieldList TokGeometry GeometryObj {
       GEOM_PARSER_SET_FIELD($1,Geometry,$3); $$=$1;
   }
 | ShapeFieldList TokAppearance AppearanceObj {
     GEOM_PARSER_SET_FIELD($1,Appearance,$3); $$=$1;
   }
 | { $$ = new Shape::Builder;
#ifdef USE_READLINE
    lexer(l);
   if(l.useReadline())setKeyword(shape_att_keyword);
#endif
}
;

ShapeShortFieldList:
   GeometryRef ',' AppearanceRef {
       $$ = new Shape::Builder;
       GEOM_PARSER_SET_FIELD($$,Geometry,$1);
       GEOM_PARSER_SET_FIELD($$,Appearance,$3);
   }
|  GeometryRef {
       $$ = new Shape::Builder;
       GEOM_PARSER_SET_FIELD($$,Geometry,$1);
}
;

/**
 **  Reference object
 **/

AppearanceRef:
 TokName {
//     parser(p);
//     smbtable(p,t);
     uint32_t index = symbolstack.size()-1;
     SymbolTable<SMB_TABLE_TYPE>* t = symbolstack[index];
     SymbolTable<SMB_TABLE_TYPE>::iterator _i = t->find(*$1);
     while(index > 0 && _i == t->end()){
        index--;
        t = symbolstack[index];
        _i = t->find(*$1);
     }
     if (_i != t->end()){
         AppearancePtr _app;
         if(_app.cast(_i->second))
             $$ = new AppearancePtr(_app);
         else{
             genMessage(WARNINGMSG(INVALID_TYPE_sss),"Appearance",$1->c_str(),typeid(*(_i->second)).name());
             $$ = NULL;
         }
     }
     else {
       genMessage(WARNINGMSG(UNDECLARED_OBJECT_s),$1->c_str());
       $$ = NULL;
     }
     delete $1;
   }
;

GeometryRef:
   TokName {
//     parser(p);
//     smbtable(p,t);

     uint32_t index = symbolstack.size()-1;
     SymbolTable<SMB_TABLE_TYPE>* t = symbolstack[index];
     SymbolTable<SMB_TABLE_TYPE>::iterator _i = t->find(*$1);
     while(index > 0 && _i == t->end()){
        index--;
        t = symbolstack[index];
        _i = t->find(*$1);
     }
     if (_i != t->end()){
       GeometryPtr  _geom;
       if(_geom.cast(_i->second))
         $$ = new GeometryPtr(_geom);
       else{
         genMessage(WARNINGMSG(INVALID_TYPE_sss),"Geometry",$1->c_str(),typeid(*(_i->second)).name());
         $$ = NULL;
       }

     }
     else {
         genMessage(WARNINGMSG(UNDECLARED_OBJECT_s),$1->c_str());
         $$ = NULL;
     }
     delete $1;
   }
;

/**
 **  Geometry object classes
 **/

PrimitiveObj:
   AmapSymbolObj { $$ = $1; }
 | AsymmetricHullObj { $$ = $1; }
 | BoxObj { $$ = $1; }
 | BezierCurveObj { $$ = $1; }
 | BezierCurve2DObj { $$ = $1; }
 | BezierPatchObj { $$ = $1; }
 | ConeObj { $$ = $1; }
 | DiscObj { $$ = $1; }
 | PointSetObj { $$ = $1; }
 | PointSet2DObj { $$ = $1; }
 | CylinderObj { $$ = $1; }
 | ElevationGridObj { $$ = $1; }
 | ExtrudedHullObj { $$ = $1; }
 | FaceSetObj { $$ = $1; }
 | FrustumObj { $$ = $1; }
 | ExtrusionObj { $$ = $1; }
 | NurbsCurveObj { $$ = $1; }
 | NurbsCurve2DObj { $$ = $1; }
 | NurbsPatchObj { $$ = $1; }
 | ParaboloidObj { $$ = $1; }
 | PolylineObj { $$ = $1; }
 | Polyline2DObj { $$ = $1; }
 | QuadSetObj { $$ = $1; }
 | RevolutionObj { $$ = $1; }
 | SwungObj { $$ = $1; }
 | SphereObj { $$ = $1; }
 | TriangleSetObj { $$ = $1; }
;

TransformedObj:
   TranslatedObj { $$ = $1; }
 | ScaledObj { $$ = $1; }
 | OrientedObj { $$ = $1; }
 | AxisRotatedObj { $$ = $1; }
 | EulerRotatedObj { $$ = $1; }
 | TaperedObj { $$ = $1; }
 | IFSObj { $$ = $1; }
;

Curve2DObj:
   BezierCurve2DObj   { if($1) {$$ = new Curve2DPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | NurbsCurve2DObj    { if($1) {$$ = new Curve2DPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | Polyline2DObj      { if($1) {$$ = new Curve2DPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | GeometryRef        { if($1) {$$ = new Curve2DPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | Point2Array {
      Polyline2D::Builder * builder = new Polyline2D::Builder;
      GEOM_PARSER_SET_FIELD(builder,PointList,$1);
      std::string * name = NULL;
      SceneObjectPtr * obj = NULL;
      GEOM_PARSER_BUILD_OBJECT(SceneObject,obj,name,builder);
      Curve2DPtr * line = NULL;
      if(obj){
          line = new Curve2DPtr; line->cast(*obj);
      }
      $$ = line;
  }
;

// | CircleObj          { if($1) {$$ = new Curve2DPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }

CurveObj:
 BezierCurveObj       { if($1) {$$ = new LineicModelPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | NurbsCurveObj      { if($1) {$$ = new LineicModelPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | GeometryRef        { if($1) {$$ = new LineicModelPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | PolylineObj        { if($1) {$$ = new LineicModelPtr; $$->cast(*$1);delete $1;}else $$ = NULL; }
 | Point3Array {
      Polyline::Builder * builder = new Polyline::Builder;
      GEOM_PARSER_SET_FIELD(builder,PointList,$1);
      std::string * name = NULL;
      SceneObjectPtr * obj = NULL;
      GEOM_PARSER_BUILD_OBJECT(SceneObject,obj,name,builder);
      LineicModelPtr * line = NULL;
      if(obj){
          line = new LineicModelPtr; line->cast(*obj);
      }
      $$ = line;
  }
;

GroupObj:
  TokGroup Name '{' GroupFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  }
;

/**
 **  Appearance classes
 **/

PhysicalModelObj:
   MonoSpectralObj { $$ = $1; }
 | MultiSpectralObj { $$ = $1; }
;

/**
 **  Appearance objects
 **/

MaterialObj:
  TokMaterial Name '{' MaterialFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Appearance,$$,$2,$4);
  }
 | Color3 {
    Material::Builder * a= new Material::Builder;
    GEOM_PARSER_SET_FIELD(a,Ambient,$1);
    std::string * name = NULL;
    GEOM_PARSER_BUILD_OBJECT(Appearance,$$,name,a);
 };

TextureObj:
  TokImageTexture Name '{' ImageTextureFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Appearance,$$,$2,$4);
  };

MonoSpectralObj:
  TokMonoSpectral Name '{' MonoSpectralFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Appearance,$$,$2,$4);
  };

MultiSpectralObj:
  TokMultiSpectral Name '{' MultiSpectralFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Appearance,$$,$2,$4);
  }
;



/**
 **  Commands
 **/

Command:
 TokQuit {
     parser(p);
     postream(p) << "Bye ..." << std::endl;
     YYACCEPT;
   }
 | '?' {
     parser(p);
     cursmbtable(t);
     Printer printer(postream(p),postream(p),postream(p));
     for (SceneObjectSymbolTable::iterator _i = t.begin();_i != t.end();_i++) {
       _i->second->apply(printer);
       postream(p) << std::endl;
     };
  }
 | TokList {
     parser(p);
     cursmbtable(t);
     Printer printer(postream(p),postream(p),postream(p));
     for (SceneObjectSymbolTable::iterator _i = t.begin();_i != t.end();_i++) {
       const char * _cname = typeid(*(_i->second)).name();
       postream(p) << "<" << _cname << "> : " <<  (_i->second->isNamed() ? _i->second->getName().c_str() : "No Name") << std::endl;
     };
  }
 | TokBinaryInclude {
   parser(p);
   cursmbtable(t);
   BinaryParser _bp (postream(p));
   _bp.parse(*$1);
   ScenePtr a = _bp.getScene();
   SceneObjectPtr b;
   for(Scene::iterator _it = a->getBegin(); _it != a->getEnd() ; _it++){
     if((*_it)->isNamed()){
       b = (*_it);
       if(b)
         t[b->getName()]=b;
     }
   }
 //    cerr << "Binary include : Not yet implemented !" << endl;
 }
;


/**
 **  Primitives
 **/

AmapSymbolObj:
  TokAmapSymbol Name '{' AmapSymbolFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

AsymmetricHullObj:
  TokAsymmetricHull Name '{' AsymmetricHullFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

AxisRotatedObj:
  TokAxisRotated Name '{' AxisRotatedFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

BezierCurveObj:
  TokBezierCurve Name '{' BezierCurveFieldList '}' {
      GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

BezierPatchObj:
  TokBezierPatch Name '{' BezierPatchFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

BoxObj:
  TokBox Name '{' BoxFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

ConeObj:
  TokCone Name '{' ConeFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

CylinderObj:
  TokCylinder Name '{' CylinderFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

ElevationGridObj:
  TokElevationGrid Name '{' ElevationGridFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

EulerRotatedObj:
  TokEulerRotated Name '{' EulerRotatedFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

ExtrudedHullObj:
  TokExtrudedHull Name '{' ExtrudedHullFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

FaceSetObj:
  TokFaceSet Name '{' FaceSetFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

ExtrusionObj:
  TokExtrusion Name '{' ExtrusionFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

FrustumObj:
  TokFrustum Name '{' FrustumFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

IFSObj:
  TokIFS Name '{' IFSFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

NurbsCurveObj:
  TokNurbsCurve Name '{' NurbsCurveFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

NurbsPatchObj:
  TokNurbsPatch Name '{' NurbsPatchFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

OrientedObj:
  TokOriented Name '{' OrientedFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

ParaboloidObj:
  TokParaboloid Name '{' ParaboloidFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

PointSetObj:
  TokPointSet Name '{' PointSetFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

PolylineObj:
  TokPolyline Name '{' PolylineFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

QuadSetObj:
  TokQuadSet Name '{' QuadSetFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

RevolutionObj:
  TokRevolution Name '{' RevolutionFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

SwungObj:
  TokSwung Name '{' SwungFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

ScaledObj:
  TokScaled Name '{' ScaledFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

SphereObj:
  TokSphere Name '{' SphereFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

TaperedObj:
  TokTapered Name '{' TaperedFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

TranslatedObj:
  TokTranslated Name '{' TranslatedFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

TriangleSetObj:
  TokTriangleSet Name '{' TriangleSetFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

BezierCurve2DObj:
  TokBezierCurve2D Name '{' BezierCurve2DFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

DiscObj:
  TokDisc Name '{' DiscFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

NurbsCurve2DObj:
  TokNurbsCurve2D Name '{' NurbsCurve2DFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

Polyline2DObj:
  TokPolyline2D Name '{' Polyline2DFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

PointSet2DObj:
  TokPointSet2D Name '{' PointSet2DFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

TextObj:
  TokText Name '{' TextFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Geometry,$$,$2,$4);
  };

FontObj:
  TokFont Name '{' FontFieldList '}' {
    GEOM_PARSER_BUILD_OBJECT(Font,$$,$2,$4);
  };

/* Field list */

AmapSymbolFieldList:
   AmapSymbolFieldList TokFileName Filename {
     GEOM_PARSER_SET_FIELD($1,FileName,$3); $$=$1;
   }
 | AmapSymbolFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3); $$=$1;
   }
 | { $$ = new AmapSymbol::Builder; };

AsymmetricHullFieldList:
   AsymmetricHullFieldList TokNegXRadius Real {
     GEOM_PARSER_SET_FIELD($1,NegXRadius,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokPosXRadius Real {
     GEOM_PARSER_SET_FIELD($1,PosXRadius,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokNegYRadius Real {
     GEOM_PARSER_SET_FIELD($1,NegYRadius,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokPosYRadius Real {
     GEOM_PARSER_SET_FIELD($1,PosYRadius,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokNegXHeight Real {
     GEOM_PARSER_SET_FIELD($1,NegXHeight,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokPosXHeight Real {
     GEOM_PARSER_SET_FIELD($1,PosXHeight,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokNegYHeight Real {
     GEOM_PARSER_SET_FIELD($1,NegYHeight,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokPosYHeight Real {
     GEOM_PARSER_SET_FIELD($1,PosYHeight,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokBottom Vector3 {
     GEOM_PARSER_SET_FIELD($1,Bottom,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokTop Vector3 {
     GEOM_PARSER_SET_FIELD($1,Top,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokBottomShape Real {
     GEOM_PARSER_SET_FIELD($1,BottomShape,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokTopShape Real {
     GEOM_PARSER_SET_FIELD($1,TopShape,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | AsymmetricHullFieldList TokStacks Uchar {
     GEOM_PARSER_SET_FIELD($1,Stacks,$3); $$=$1;
   }
 | { $$ = new AsymmetricHull::Builder;
#ifdef USE_READLINE
 lexer(l);
 if(l.useReadline())setKeyword(ashull_att_keyword);
#endif
 };

AxisRotatedFieldList:
   AxisRotatedFieldList TokAxis Vector3 {
     GEOM_PARSER_SET_FIELD($1,Axis,$3); $$=$1;
   }
 | AxisRotatedFieldList TokAngle Angle {
     GEOM_PARSER_SET_FIELD($1,Angle,$3); $$=$1;
   }
 | AxisRotatedFieldList TokGeometry GeometryObj {
     GEOM_PARSER_SET_FIELD($1,Geometry,$3); $$=$1;
   }
 | { $$ = new AxisRotated::Builder; };

BezierCurveFieldList:
   BezierCurveFieldList TokDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,Degree,$3); $$=$1;
   }
 | BezierCurveFieldList TokStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,Stride,$3); $$=$1;
   }
 | BezierCurveFieldList TokCtrlPointList CtrlPointArray {
     GEOM_PARSER_SET_FIELD($1,CtrlPointList,$3); $$=$1;
   }
 | { $$ = new BezierCurve::Builder; };

BezierCurve2DFieldList:
   BezierCurve2DFieldList TokDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,Degree,$3); $$=$1;
   }
 | BezierCurve2DFieldList TokStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,Stride,$3); $$=$1;
   }
 | BezierCurve2DFieldList TokCtrlPointList CtrlPointArray2D {
     GEOM_PARSER_SET_FIELD($1,CtrlPointList,$3); $$=$1;
   }
 | { $$ = new BezierCurve2D::Builder; };

BezierPatchFieldList:
   BezierPatchFieldList TokUDegree Uint32 {
      GEOM_PARSER_SET_FIELD($1,UDegree,$3); $$=$1;
   }
 | BezierPatchFieldList TokVDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,VDegree,$3); $$=$1;
   }
 | BezierPatchFieldList TokUStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,UStride,$3); $$=$1;
   }
 | BezierPatchFieldList TokVStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,VStride,$3); $$=$1;
   }
 | BezierPatchFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3);  $$=$1;
 }
 | BezierPatchFieldList TokCtrlPointMatrix CtrlPointMatrix {
     GEOM_PARSER_SET_FIELD($1,CtrlPointMatrix,$3); $$=$1;
   }
 | { $$ = new BezierPatch::Builder; };

BoxFieldList:
   BoxFieldList TokSize Vector3 {
     GEOM_PARSER_SET_FIELD($1,Size,$3); $$=$1;
   }
 | { $$ = new Box::Builder; };

ConeFieldList:
   ConeFieldList TokRadius Real {
     GEOM_PARSER_SET_FIELD($1,Radius,$3); $$=$1;
   }
 | ConeFieldList TokHeight Real {
     GEOM_PARSER_SET_FIELD($1,Height,$3); $$=$1;
   }
 | ConeFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3); $$=$1;
   }
 | ConeFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | {
   $$ = new Cone::Builder;
#ifdef USE_READLINE
   lexer(l);
   if(l.useReadline()) setKeyword(cone_att_keyword);
#endif
 };

CylinderFieldList:
   CylinderFieldList TokRadius Real {
     GEOM_PARSER_SET_FIELD($1,Radius,$3); $$=$1;
   }
 | CylinderFieldList TokHeight Real {
     GEOM_PARSER_SET_FIELD($1,Height,$3); $$=$1;
   }
 | CylinderFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3); $$=$1;
   }
 | CylinderFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | {$$ = new Cylinder::Builder; };

DiscFieldList:
   DiscFieldList TokRadius Real {
     GEOM_PARSER_SET_FIELD($1,Radius,$3); $$=$1;
   }
 | DiscFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | {$$ = new Disc::Builder; };

ElevationGridFieldList:
  ElevationGridFieldList TokHeightList RealMatrix {
     GEOM_PARSER_SET_FIELD($1,HeightList,$3); $$=$1;
   }
 | ElevationGridFieldList TokXSpacing Real {
     GEOM_PARSER_SET_FIELD($1,XSpacing,$3); $$=$1;
   }
 | ElevationGridFieldList TokYSpacing Real {
     GEOM_PARSER_SET_FIELD($1,YSpacing,$3); $$=$1;
   }
 | ElevationGridFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3);  $$=$1;
   }
 | {$$ = new ElevationGrid::Builder; };

EulerRotatedFieldList:
   EulerRotatedFieldList TokAzimuth Angle {
     GEOM_PARSER_SET_FIELD($1,Azimuth,$3); $$=$1;
   }
 | EulerRotatedFieldList TokElevation Angle {
    GEOM_PARSER_SET_FIELD($1,Elevation,$3); $$=$1;
   }
 | EulerRotatedFieldList TokRoll Angle {
     GEOM_PARSER_SET_FIELD($1,Roll,$3); $$=$1;
   }
 | EulerRotatedFieldList TokGeometry GeometryObj {
     GEOM_PARSER_SET_FIELD($1,Geometry,$3); $$=$1;
   }
 | {$$ = new EulerRotated::Builder; };

ExtrudedHullFieldList:
   ExtrudedHullFieldList TokVertical Curve2DObj {
     GEOM_PARSER_SET_FIELD($1,Vertical,$3);  $$=$1;
   }
 | ExtrudedHullFieldList TokHorizontal Curve2DObj {
     GEOM_PARSER_SET_FIELD($1,Horizontal,$3);  $$=$1;
   }
 | ExtrudedHullFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3);  $$=$1;
   }
 | { $$ = new ExtrudedHull::Builder; };

ExtrusionFieldList:
  ExtrusionFieldList TokAxis CurveObj {
     GEOM_PARSER_SET_FIELD($1,Axis,$3);  $$=$1;
 }
 | ExtrusionFieldList TokCrossSection Curve2DObj {
     GEOM_PARSER_SET_FIELD($1,CrossSection,$3);  $$=$1;
 }
 | ExtrusionFieldList TokScale Point2Array {
     GEOM_PARSER_SET_FIELD($1,Scale,$3); $$=$1;
 }
 | ExtrusionFieldList TokOrientation AngleArray {
     GEOM_PARSER_SET_FIELD($1,Orientation,$3); $$=$1;
 }
 | ExtrusionFieldList TokKnotList RealArray {
     GEOM_PARSER_SET_FIELD($1,KnotList,$3); $$=$1;
 }
 | ExtrusionFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3); $$=$1;
 }
 | ExtrusionFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3); $$=$1;
 }
 | {$$ = new Extrusion::Builder; };


FaceSetFieldList:
   FaceSetFieldList TokPointList Point3Array {
     GEOM_PARSER_SET_FIELD($1,PointList,$3); $$=$1;
   }
 | FaceSetFieldList TokIndexList IndexArray {
     GEOM_PARSER_SET_FIELD($1,IndexList,$3);  $$=$1;
   }
 | FaceSetFieldList TokNormalPerVertex TokBool {
     GEOM_PARSER_SET_FIELD($1,NormalPerVertex,$3);  $$=$1;
   }
 | FaceSetFieldList TokNormalList Normal3Array {
     GEOM_PARSER_SET_FIELD($1,NormalList,$3);  $$=$1;
   }
 | FaceSetFieldList TokNormalIndexList IndexArray {
     GEOM_PARSER_SET_FIELD($1,NormalIndexList,$3);  $$=$1;
   }
 | FaceSetFieldList TokColorPerVertex TokBool {
     GEOM_PARSER_SET_FIELD($1,ColorPerVertex,$3);  $$=$1;
   }
 | FaceSetFieldList TokColorList Color4Array {
     GEOM_PARSER_SET_FIELD($1,ColorList,$3);  $$=$1;
   }
 | FaceSetFieldList TokColorIndexList IndexArray {
     GEOM_PARSER_SET_FIELD($1,ColorIndexList,$3);  $$=$1;
   }
 | FaceSetFieldList TokTexCoordList Point2Array {
     GEOM_PARSER_SET_FIELD($1,TexCoordList,$3);  $$=$1;
   }
 | FaceSetFieldList TokTexCoordIndexList IndexArray {
     GEOM_PARSER_SET_FIELD($1,TexCoordIndexList,$3);  $$=$1;
   }
 | FaceSetFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3);  $$=$1;
   }
 | FaceSetFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3);  $$=$1;
   }
 | FaceSetFieldList TokSkeleton Skeleton {
     GEOM_PARSER_SET_FIELD($1,Skeleton,$3); $$=$1;
   }
 | { $$ = new FaceSet::Builder; };

FrustumFieldList:
   FrustumFieldList TokRadius Real {
     GEOM_PARSER_SET_FIELD($1,Radius,$3); $$=$1;
   }
 | FrustumFieldList TokHeight Real {
     GEOM_PARSER_SET_FIELD($1,Height,$3); $$=$1;
   }
 | FrustumFieldList TokTaper Real {
     GEOM_PARSER_SET_FIELD($1,Taper,$3); $$=$1;
   }
 | FrustumFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3); $$=$1;
   }
 | FrustumFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | {$$ = new Frustum::Builder; };

IFSFieldList:
   IFSFieldList TokDepth Uchar {
     GEOM_PARSER_SET_FIELD($1,Depth,$3); $$=$1;
   }
 | IFSFieldList TokGeometry GeometryObj {
     GEOM_PARSER_SET_FIELD($1,Geometry,$3); $$=$1;
   }
 | IFSFieldList TokTransfoList TransfoArray {
     GEOM_PARSER_SET_FIELD($1,TransfoList,$3); $$=$1;
   }
 | { $$ = new IFS::Builder; };

GroupFieldList:
   GroupFieldList TokGeometryList GeometryArray {
     GEOM_PARSER_SET_FIELD($1,GeometryList,$3); $$=$1;
   }
 | GroupFieldList TokSkeleton Skeleton {
     GEOM_PARSER_SET_FIELD($1,Skeleton,$3); $$=$1;
   }
 | { $$ = new Group::Builder; };

NurbsCurveFieldList:
   NurbsCurveFieldList TokDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,Degree,$3); $$=$1;
   }
 | NurbsCurveFieldList TokStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,Stride,$3); $$=$1;
   }
 | NurbsCurveFieldList TokCtrlPointList CtrlPointArray {
     GEOM_PARSER_SET_FIELD($1,CtrlPointList,$3); $$=$1;
   }
 | NurbsCurveFieldList TokKnotList RealArray {
     GEOM_PARSER_SET_FIELD($1,KnotList,$3); $$=$1;
   }
 | { $$ = new NurbsCurve::Builder; };

NurbsCurve2DFieldList:
   NurbsCurve2DFieldList TokDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,Degree,$3); $$=$1;
   }
 | NurbsCurve2DFieldList TokStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,Stride,$3); $$=$1;
   }
 | NurbsCurve2DFieldList TokCtrlPointList CtrlPointArray2D {
     GEOM_PARSER_SET_FIELD($1,CtrlPointList,$3); $$=$1;
   }
 | NurbsCurve2DFieldList TokKnotList RealArray {
     GEOM_PARSER_SET_FIELD($1,KnotList,$3); $$=$1;
   }
 | { $$ = new NurbsCurve2D::Builder; };

NurbsPatchFieldList:
   NurbsPatchFieldList TokUDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,UDegree,$3); $$=$1;
   }
 | NurbsPatchFieldList TokVDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,VDegree,$3); $$=$1;
   }
 | NurbsPatchFieldList TokUStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,UStride,$3); $$=$1;
   }
 | NurbsPatchFieldList TokVStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,VStride,$3); $$=$1;
   }
 | NurbsPatchFieldList TokUKnotList RealArray {
     GEOM_PARSER_SET_FIELD($1,UKnotList,$3); $$=$1;
   }
 | NurbsPatchFieldList TokVKnotList RealArray {
     GEOM_PARSER_SET_FIELD($1,VKnotList,$3); $$=$1;
   }
 | NurbsPatchFieldList TokCtrlPointMatrix CtrlPointMatrix {
     GEOM_PARSER_SET_FIELD($1,CtrlPointMatrix,$3); $$=$1;
   }
 | NurbsPatchFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3);  $$=$1;
   }
 | { $$ = new NurbsPatch::Builder; };

OrientedFieldList:
   OrientedFieldList TokPrimary Vector3 {
     GEOM_PARSER_SET_FIELD($1,Primary,$3); $$=$1;
   }
 | OrientedFieldList TokSecondary Vector3 {
     GEOM_PARSER_SET_FIELD($1,Secondary,$3); $$=$1;
   }
 | OrientedFieldList TokGeometry GeometryObj {
     GEOM_PARSER_SET_FIELD($1,Geometry,$3); $$=$1;
   }
 | {$$ = new Oriented::Builder; };

ParaboloidFieldList:
   ParaboloidFieldList TokRadius Real {
     GEOM_PARSER_SET_FIELD($1,Radius,$3); $$=$1;
   }
 | ParaboloidFieldList TokHeight Real {
     GEOM_PARSER_SET_FIELD($1,Height,$3); $$=$1;
   }
 | ParaboloidFieldList TokShapeFactor Real {
     GEOM_PARSER_SET_FIELD($1,Shape,$3); $$=$1;
   }
 | ParaboloidFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3); $$=$1;
   }
 | ParaboloidFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | ParaboloidFieldList TokStacks Uchar {
     GEOM_PARSER_SET_FIELD($1,Stacks,$3); $$=$1;
   }
 | {$$ = new Paraboloid::Builder; };

PointSetFieldList:
   PointSetFieldList TokPointList Point3Array {
     GEOM_PARSER_SET_FIELD($1,PointList,$3);  $$=$1;
   }
 | PointSetFieldList TokColorList Color4Array {
     GEOM_PARSER_SET_FIELD($1,ColorList,$3);  $$=$1;
   }
 | { $$ = new PointSet::Builder; };

PointSet2DFieldList:
   PointSet2DFieldList TokPointList Point2Array {
     GEOM_PARSER_SET_FIELD($1,PointList,$3);  $$=$1;
   }
 | { $$ = new PointSet2D::Builder; };

PolylineFieldList:
   PolylineFieldList TokPointList Point3Array {
     GEOM_PARSER_SET_FIELD($1,PointList,$3);  $$=$1;
   }
 | PolylineFieldList TokColorList Color4Array {
     GEOM_PARSER_SET_FIELD($1,ColorList,$3);  $$=$1;
   }
 | { $$ = new Polyline::Builder; };

Polyline2DFieldList:
   Polyline2DFieldList TokPointList Point2Array {
     GEOM_PARSER_SET_FIELD($1,PointList,$3);  $$=$1;
   }
 | { $$ = new Polyline2D::Builder; };

QuadSetFieldList:
   QuadSetFieldList TokPointList Point3Array {
     GEOM_PARSER_SET_FIELD($1,PointList,$3); $$=$1;
   }
 | QuadSetFieldList TokIndexList Index4Array {
     GEOM_PARSER_SET_FIELD($1,IndexList,$3);  $$=$1;
   }
 | QuadSetFieldList TokNormalPerVertex TokBool {
     GEOM_PARSER_SET_FIELD($1,NormalPerVertex,$3);  $$=$1;
   }
 | QuadSetFieldList TokNormalList Normal3Array {
     GEOM_PARSER_SET_FIELD($1,NormalList,$3);  $$=$1;
   }
 | QuadSetFieldList TokNormalIndexList Index4Array {
     GEOM_PARSER_SET_FIELD($1,NormalIndexList,$3);  $$=$1;
   }
 | QuadSetFieldList TokColorPerVertex TokBool {
     GEOM_PARSER_SET_FIELD($1,ColorPerVertex,$3);  $$=$1;
   }
 | QuadSetFieldList TokColorList Color4Array {
     GEOM_PARSER_SET_FIELD($1,ColorList,$3);  $$=$1;
   }
 | QuadSetFieldList TokColorIndexList Index4Array {
     GEOM_PARSER_SET_FIELD($1,ColorIndexList,$3);  $$=$1;
   }
 | QuadSetFieldList TokTexCoordList Point2Array {
     GEOM_PARSER_SET_FIELD($1,TexCoordList,$3);  $$=$1;
   }
 | QuadSetFieldList TokTexCoordIndexList Index4Array {
     GEOM_PARSER_SET_FIELD($1,TexCoordIndexList,$3);  $$=$1;
   }
 | QuadSetFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3);  $$=$1;
   }
 | QuadSetFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3);  $$=$1;
   }
 | QuadSetFieldList TokSkeleton Skeleton {
     GEOM_PARSER_SET_FIELD($1,Skeleton,$3); $$=$1;
   }
 | { $$ = new QuadSet::Builder; };

RevolutionFieldList:
   RevolutionFieldList TokPointList Curve2DObj {
     GEOM_PARSER_SET_FIELD($1,Profile,$3); $$=$1;
   }
 | RevolutionFieldList TokProfile Curve2DObj {
     GEOM_PARSER_SET_FIELD($1,Profile,$3); $$=$1;
   }
 | RevolutionFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | { $$ = new Revolution::Builder; };

SwungFieldList:
   SwungFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3); $$=$1;
   }
 | SwungFieldList TokProfileList Curve2DArray {
     GEOM_PARSER_SET_FIELD($1,ProfileList,$3); $$=$1;
   }
 | SwungFieldList TokAngleList AngleArray {
     GEOM_PARSER_SET_FIELD($1,AngleList,$3); $$=$1;
   }
 | SwungFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | SwungFieldList TokDegree Uint32 {
     GEOM_PARSER_SET_FIELD($1,Degree,$3); $$=$1;
   }
 | SwungFieldList TokStride Uint32 {
     GEOM_PARSER_SET_FIELD($1,Stride,$3); $$=$1;
   }
 | { $$ = new Swung::Builder; };


SphereFieldList:
   SphereFieldList TokRadius Real {
     GEOM_PARSER_SET_FIELD($1,Radius,$3); $$=$1;
   }
 | SphereFieldList TokSlices Uchar {
     GEOM_PARSER_SET_FIELD($1,Slices,$3); $$=$1;
   }
 | SphereFieldList TokStacks Uchar {
     GEOM_PARSER_SET_FIELD($1,Stacks,$3); $$=$1;
   }
 | { $$ = new Sphere::Builder; };

ScaledFieldList:
   ScaledFieldList TokScale Vector3 {
     GEOM_PARSER_SET_FIELD($1,Scale,$3); $$=$1;
   }
 | ScaledFieldList TokGeometry GeometryObj {
     GEOM_PARSER_SET_FIELD($1,Geometry,$3); $$=$1;
   }
 | { $$ = new Scaled::Builder; };

TaperedFieldList:
   TaperedFieldList TokBaseRadius Real {
     GEOM_PARSER_SET_FIELD($1,BaseRadius,$3); $$=$1;
   }
 | TaperedFieldList TokTopRadius Real {
     GEOM_PARSER_SET_FIELD($1,TopRadius,$3); $$=$1;
   }
 | TaperedFieldList TokPrimitive Primitive {
     GEOM_PARSER_SET_FIELD($1,Primitive,$3); $$=$1;
   }
 | { $$ = new Tapered::Builder; };

TranslatedFieldList:
   TranslatedFieldList TokTranslation Vector3 {
     GEOM_PARSER_SET_FIELD($1,Translation,$3); $$=$1;
   }
 | TranslatedFieldList TokGeometry GeometryObj {
     GEOM_PARSER_SET_FIELD($1,Geometry,$3); $$=$1;
   }
| { $$ = new Translated::Builder; };


TriangleSetFieldList:
   TriangleSetFieldList TokPointList Point3Array {
     GEOM_PARSER_SET_FIELD($1,PointList,$3); $$=$1;
   }
 | TriangleSetFieldList TokIndexList Index3Array {
     GEOM_PARSER_SET_FIELD($1,IndexList,$3);  $$=$1;
   }
 | TriangleSetFieldList TokNormalPerVertex TokBool {
     GEOM_PARSER_SET_FIELD($1,NormalPerVertex,$3);  $$=$1;
   }
 | TriangleSetFieldList TokNormalList Normal3Array {
     GEOM_PARSER_SET_FIELD($1,NormalList,$3);  $$=$1;
   }
 | TriangleSetFieldList TokNormalIndexList Index3Array {
     GEOM_PARSER_SET_FIELD($1,NormalIndexList,$3);  $$=$1;
   }
 | TriangleSetFieldList TokColorPerVertex TokBool {
     GEOM_PARSER_SET_FIELD($1,ColorPerVertex,$3);  $$=$1;
   }
 | TriangleSetFieldList TokColorList Color4Array {
     GEOM_PARSER_SET_FIELD($1,ColorList,$3);  $$=$1;
   }
 | TriangleSetFieldList TokColorIndexList Index3Array {
     GEOM_PARSER_SET_FIELD($1,ColorIndexList,$3);  $$=$1;
   }
 | TriangleSetFieldList TokTexCoordList Point2Array {
     GEOM_PARSER_SET_FIELD($1,TexCoordList,$3);  $$=$1;
   }
 | TriangleSetFieldList TokTexCoordIndexList Index3Array {
     GEOM_PARSER_SET_FIELD($1,TexCoordIndexList,$3);  $$=$1;
   }
 | TriangleSetFieldList TokCCW TokBool {
     GEOM_PARSER_SET_FIELD($1,CCW,$3);  $$=$1;
   }
 | TriangleSetFieldList TokSolid TokBool {
     GEOM_PARSER_SET_FIELD($1,Solid,$3);  $$=$1;
   }
 | TriangleSetFieldList TokSkeleton Skeleton {
     GEOM_PARSER_SET_FIELD($1,Skeleton,$3); $$=$1;
   }
 | { $$ = new TriangleSet::Builder; };


TextFieldList:
   TextFieldList TokString Filename {
     GEOM_PARSER_SET_FIELD($1,String,$3); $$=$1;
   }
 | TextFieldList TokPosition Vector3 {
     GEOM_PARSER_SET_FIELD($1,Position,$3);  $$=$1;
   }
 | TextFieldList TokScreenCoordinates TokBool {
     GEOM_PARSER_SET_FIELD($1,ScreenCoordinates,$3);  $$=$1;
   }
 | TextFieldList TokFontStyle FontObj {
     GEOM_PARSER_SET_FIELD($1,FontStyle,$3);  $$=$1;
   }
 | { $$ = new Text::Builder; };

FontFieldList:
   FontFieldList TokFamily Filename {
     GEOM_PARSER_SET_FIELD($1,Family,$3); $$=$1;
   }
 | FontFieldList TokSize Uint32 {
     GEOM_PARSER_SET_FIELD($1,Size,$3);  $$=$1;
   }
 | FontFieldList TokBold TokBool {
     GEOM_PARSER_SET_FIELD($1,Bold,$3);  $$=$1;
   }
 | FontFieldList TokItalic TokBool {
     GEOM_PARSER_SET_FIELD($1,Italic,$3);  $$=$1;
   }
 | { $$ = new Font::Builder; };

/**
 ** Appearance field lists
 **/

MaterialFieldList:
   MaterialFieldList TokAmbient Color3 {
     GEOM_PARSER_SET_FIELD($1,Ambient,$3); $$=$1;
   }
 | MaterialFieldList TokDiffuse Real {
     GEOM_PARSER_SET_FIELD($1,Diffuse,$3); $$=$1;
   }
 | MaterialFieldList TokSpecular Color3 {
     GEOM_PARSER_SET_FIELD($1,Specular,$3); $$=$1;
   }
 | MaterialFieldList TokEmission Color3 {
     GEOM_PARSER_SET_FIELD($1,Emission,$3); $$=$1;
   }
 | MaterialFieldList TokShininess Real {
     GEOM_PARSER_SET_FIELD($1,Shininess,$3); $$=$1;
   }
 | MaterialFieldList TokTransparency Real {
     GEOM_PARSER_SET_FIELD($1,Transparency,$3); $$=$1;
   }
 | { $$ = new Material::Builder; };

ImageTextureFieldList:
   ImageTextureFieldList TokFileName Filename {
     if($3)*$3 = TOOLS(absolute_filename)(*$3);
     GEOM_PARSER_SET_FIELD($1,FileName,$3); $$=$1;
   }   
 | ImageTextureFieldList TokMipmaping TokBool {
     GEOM_PARSER_SET_FIELD($1,Mipmaping,$3); $$=$1;
   }
 | ImageTextureFieldList TokAmbient Color3 {
     GEOM_PARSER_SET_FIELD($1,Ambient,$3); $$=$1;
   }
 | ImageTextureFieldList TokDiffuse Real {
     GEOM_PARSER_SET_FIELD($1,Diffuse,$3); $$=$1;
   }
 | ImageTextureFieldList TokSpecular Color3 {
     GEOM_PARSER_SET_FIELD($1,Specular,$3); $$=$1;
   }
 | ImageTextureFieldList TokEmission Color3 {
     GEOM_PARSER_SET_FIELD($1,Emission,$3); $$=$1;
   }
 | ImageTextureFieldList TokShininess Real {
     GEOM_PARSER_SET_FIELD($1,Shininess,$3); $$=$1;
   }
 | ImageTextureFieldList TokTransparency Real {
     GEOM_PARSER_SET_FIELD($1,Transparency,$3); $$=$1;
   }
 | { $$ = new ImageTexture::Builder; };

MonoSpectralFieldList:
   MonoSpectralFieldList TokReflectance Real {
     GEOM_PARSER_SET_FIELD($1,Reflectance,$3); $$=$1;
   }
 | MonoSpectralFieldList TokTransmittance Real {
     GEOM_PARSER_SET_FIELD($1,Transmittance,$3); $$=$1;
   }
 | { $$ = new MonoSpectral::Builder; };

MultiSpectralFieldList:
   MultiSpectralFieldList TokReflectance RealArray {
     GEOM_PARSER_SET_FIELD($1,Reflectance,$3); $$=$1;
   }
 | MultiSpectralFieldList TokTransmittance RealArray {
     GEOM_PARSER_SET_FIELD($1,Transmittance,$3); $$=$1;
   }
 | MultiSpectralFieldList TokFilter Index3 {
     GEOM_PARSER_SET_FIELD($1,Filter,$3); $$=$1;
   }
 | { $$ = new MultiSpectral::Builder; };

/**
 ** Transformation field lists
 **/



/* Types */

Angle:
   Real {
     if ($1)
       *$1 *= real_t(GEOM_RAD);
     $$ = $1;
   }
 | TokACos '(' Real ')' { if(!$3)$$=NULL;
                          else {$$ = new real_t(acos(*$3));delete $3;}}
 | TokASin '(' Real ')' { if(!$3)$$=NULL;
                          else { $$ = new real_t(asin(*$3));delete $3;}}
 | TokATan '(' Real ')' { if(!$3)$$=NULL;
                          else {$$ = new real_t(atan(*$3));delete $3;}}
 | TokAngleF '(' Vector3 ',' Vector3 ')' { if(!$3||!$5)$$=NULL;
                                                  else {$$ = new real_t(angle(*$3,*$5));delete $3;delete $5;}}
 | TokAngleF '(' Vector2 ',' Vector2 ')' { if(!$3||!$5)$$=NULL;
                                                  else {$$ = new real_t(angle(*$3,*$5));delete $3;delete $5;}};

AngleArray:
   '[' AngleList ']' {
     GEOM_PARSER_CREATE_ARRAY(RealArray,$2,$$);
   };

AngleList:
   AngleList ',' Angle {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Angle {
     GEOM_PARSER_INIT_LIST(real_t,$1,$$);
   };

CtrlPointArray:
   Point3Array {
     if ($1) {
       $$ = new Point4ArrayPtr(new Point4Array(*(*$1),1));
       delete $1;
     }
     else
       $$ = 0;
   }
 | Point4Array {
     $$ = $1;
   };

CtrlPointArray2D:
   Point2Array {
     if ($1) {
       $$ = new Point3ArrayPtr(new Point3Array(*(*$1),1));
       delete $1;
     }
     else
       $$ = 0;
   }
 | Point3Array {
     $$ = $1;
   };

CtrlPointMatrix:
   Point3Matrix {
     if ($1) {
       $$ = new Point4MatrixPtr(new Point4Matrix(*$1,1));
       delete $1;
     }
     else
       $$ = 0;
   }
 | Point4Matrix {
     $$ = $1;
   };

Normal3Array:
   Point3Array {
     if ($1) {
	   (*$1)->normalize();
       $$ = $1;
     }
     else
       $$ = 0;
   };

GeometryArray:
   '[' GeometryList ']' {
     GEOM_PARSER_CREATE_ARRAY(GeometryArray,$2,$$);
   };

GeometryList:
   GeometryList ',' GeometryObj {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | GeometryObj {
     GEOM_PARSER_INIT_LIST(GeometryPtr,$1,$$);
   };

Curve2DArray:
   '[' Curve2DList ']' {
     GEOM_PARSER_CREATE_ARRAY(Curve2DArray,$2,$$);
   };

Curve2DList:
   Curve2DList ',' Curve2DObj {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Curve2DObj {
     GEOM_PARSER_INIT_LIST(Curve2DPtr,$1,$$);
   };

/**
 **  Transformation Type
 **/

TransfoArray:
   '[' TransfoList ']' {
     GEOM_PARSER_CREATE_ARRAY(Transform4Array,$2,$$);
   };

TransfoList:
   TransfoList ',' Transfo {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Transfo {
     GEOM_PARSER_INIT_LIST(Transform4Ptr,$1,$$);
   };

Transfo:
   TokTransfo Name '{' TransfoFieldList '}' {
    GEOM_PARSER_BUILD_TRANSFO(Transform4,$$,$4);
  };

Rotation:
   AxisRotation { $$ = $1; }
 | EulerRotation { $$ = $1; }
 | BaseOrientation { $$ = $1; };

AxisRotation:
  TokAxisRotation Name '{' AxisRotationFieldList '}' {
    GEOM_PARSER_BUILD_TRANSFO(OrthonormalBasis3D,$$,$4);
  };

EulerRotation:
  TokEulerRotation Name '{' EulerRotationFieldList '}' {
    GEOM_PARSER_BUILD_TRANSFO(OrthonormalBasis3D,$$,$4);
  };

BaseOrientation:
  TokBaseOrientation Name '{' BaseOrientationFieldList '}' {
    GEOM_PARSER_BUILD_TRANSFO(OrthonormalBasis3D,$$,$4);
  };

TransfoFieldList:
   TransfoFieldList TokTranslation Vector3 {
     GEOM_PARSER_SET_FIELD($1,Translation,$3); $$=$1;
   }
 | TransfoFieldList TokScale Vector3 {
     GEOM_PARSER_SET_FIELD($1,Scale,$3); $$=$1;
   }
 | TransfoFieldList Rotation {
     GEOM_PARSER_SET_FIELD($1,Rotation,$2); $$=$1;
   }
 | { $$ = new Transform4::Builder; };

AxisRotationFieldList:
   AxisRotationFieldList TokAxis Vector3 {
     GEOM_PARSER_SET_FIELD($1,Axis,$3); $$=$1;
   }
 | AxisRotationFieldList TokAngle Angle {
     GEOM_PARSER_SET_FIELD($1,Angle,$3); $$=$1;
   }
 | { $$ = new AxisRotation::Builder; };

EulerRotationFieldList:
   EulerRotationFieldList TokAzimuth Angle {
     GEOM_PARSER_SET_FIELD($1,Azimuth,$3); $$=$1;
   }
 | EulerRotationFieldList TokElevation Angle {
    GEOM_PARSER_SET_FIELD($1,Elevation,$3); $$=$1;
   }
 | EulerRotationFieldList TokRoll Angle {
     GEOM_PARSER_SET_FIELD($1,Roll,$3); $$=$1;
   }
 | {$$ = new EulerRotation::Builder; };

BaseOrientationFieldList:
   BaseOrientationFieldList TokPrimary Vector3 {
     GEOM_PARSER_SET_FIELD($1,Primary,$3); $$=$1;
   }
 | BaseOrientationFieldList TokSecondary Vector3 {
     GEOM_PARSER_SET_FIELD($1,Secondary,$3); $$=$1;
   }
 | {$$ = new BaseOrientation::Builder; };


/**
 **  Other Type
 **/
Color3:
   '<' Uchar ',' Uchar ',' Uchar '>' {
     if (($2) && ($4) && ($6)) {
       $$ = new Color3(*$2,*$4,*$6);
       delete $2;
       delete $4;
       delete $6;
     }
     else {
      $$ = NULL;
       if ($2) delete $2;
       if ($4) delete $4;
       if ($6) delete $6;
     }
   }
 | Uchar {
       if($1){
           $$ = new Color3(*$1);
           delete $1;
       }
       else
           $$ = NULL;
 }
 | TokBlack { $$ = new Color3(Color3::BLACK); }
 | TokWhite { $$ = new Color3(Color3::WHITE); }
 | TokRed { $$ = new Color3(Color3::RED); }
 | TokGreen { $$ = new Color3(Color3::GREEN); }
 | TokBlue { $$ = new Color3(Color3::BLUE); }
 | TokCyan { $$ = new Color3(Color3::CYAN); }
 | TokMagenta { $$ = new Color3(Color3::MAGENTA); }
 | TokYellow { $$ = new Color3(Color3::YELLOW); };

Color4:
   '<' Uchar ',' Uchar ',' Uchar ',' Uchar '>' {
     if (($2) && ($4) && ($6) && ($8)) {
       $$ = new Color4(*$2,*$4,*$6,*$8);
       delete $2;
       delete $4;
       delete $6;
       delete $8;
     }
     else {
      $$ = NULL;
       if ($2) delete $2;
       if ($4) delete $4;
       if ($6) delete $6;
       if ($8) delete $8;
     }
   }
 | Uchar {
       if($1){
           $$ = new Color4(*$1);
           delete $1;
       }
       else
           $$ = NULL;
 }
 | TokBlack { $$ = new Color4(Color4::BLACK); }
 | TokWhite { $$ = new Color4(Color4::WHITE); }
 | TokRed { $$ = new Color4(Color4::RED); }
 | TokGreen { $$ = new Color4(Color4::GREEN); }
 | TokBlue { $$ = new Color4(Color4::BLUE); }
 | TokCyan { $$ = new Color4(Color4::CYAN); }
 | TokMagenta { $$ = new Color4(Color4::MAGENTA); }
 | TokYellow { $$ = new Color4(Color4::YELLOW); };

Index:
   '[' Uint32List ']' {
     if ($2) {
       $$ = new Index($2->begin(),$2->end());
       delete $2;
     }
     else
       $$ = NULL;
   };

Color4Array:
   '[' Color4List ']' {
     GEOM_PARSER_CREATE_ARRAY(Color4Array,$2,$$);
   };

Color4List:
   Color4List ',' Color4 {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Color4 {
     GEOM_PARSER_INIT_LIST(Color4,$1,$$);
   };

IndexArray:
   '[' IndexList ']' {
     GEOM_PARSER_CREATE_ARRAY(IndexArray,$2,$$);
   };

IndexList:
   IndexList ',' Index {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Index {
     GEOM_PARSER_INIT_LIST(Index,$1,$$);
   };

Index3:
   '[' Uint32 ',' Uint32 ',' Uint32 ']' {
     if (($2) && ($4) && ($6)) {
       $$ = new Index3(*$2,*$4,*$6);
       delete $2;
       delete $4;
       delete $6;
     }
     else {
       $$ = NULL;
       if ($2) delete $2;
       if ($4) delete $4;
       if ($6) delete $6;
     }
   };

Index3Array:
   '[' Index3List ']' {
     GEOM_PARSER_CREATE_ARRAY(Index3Array,$2,$$);
   };

Index3List:
   Index3List ',' Index3 {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Index3 {
     GEOM_PARSER_INIT_LIST(Index3,$1,$$);
   };

Index4:
   '[' Uint32 ',' Uint32 ',' Uint32 ',' Uint32 ']' {
     if (($2) && ($4) && ($6) && ($8)) {
       $$ = new Index4(*$2,*$4,*$6,*$8);
       delete $2;
       delete $4;
       delete $6;
       delete $8;
     }
     else {
       $$ = NULL;
       if ($2) delete $2;
       if ($4) delete $4;
       if ($6) delete $6;
       if ($8) delete $8;
     }
   };

Index4Array:
   '[' Index4List ']' {
     GEOM_PARSER_CREATE_ARRAY(Index4Array,$2,$$);
   };

Index4List:
   Index4List ',' Index4 {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Index4 {
     GEOM_PARSER_INIT_LIST(Index4,$1,$$);
   };

Name:
   TokName { $$ = $1;}
 | { $$ = 0; };

Filename :
 TokFile { $$ = $1;}
| TokName { if(*$1 == "AMAPMOD_DIR" || *$1 == "OPENALEA_DIR")$$ = new std::string(TOOLS(getOpenAleaDir()));
            else if(*$1 == "PLANTGL_DIR")$$ = new std::string(TOOLS(getPlantGLDir()));
            else if($1 &&!$1->empty()&&(*$1)[0]=='%'){
				 std::string env($1->begin()+1,$1->end());
				 if(!env.empty()){
					char * d = getenv(env.c_str());
					if(d!=NULL) $$ = new std::string(d);
					else $$ = NULL;
				 }
				 else $$ = NULL;
            }
            else $$ = $1 
		  }
| Filename TokPLUS Filename { 
		if($1 && $3){
			$1->append(*$3);
			$$ = $1;
			delete $3;
		}
		else if($1)$$ = $1;
		else if($3)$$ = $3;
		else $$ = NULL;
  };

Point2Array:
   '[' Vector2List ']' {
     GEOM_PARSER_CREATE_ARRAY(Point2Array,$2,$$);
   };

/*
Point2ArrayList:
   Point2ArrayList ',' '[' Vector2List ']' {
     GEOM_PARSER_ADD_MATRIX($1,$4,$$);
   }
   |  '[' Vector2List ']' {
     GEOM_PARSER_INIT_MATRIX(Point2Matrix,$2,$$);
   };

Point2Matrix:
   '[' Point2ArrayList ']' {
     GEOM_PARSER_CREATE_MATRIX(Point2Matrix,$2,$$);
   };
*/

Point3Array:
   '[' Vector3List ']' {
     GEOM_PARSER_CREATE_ARRAY(Point3Array,$2,$$);
   };

Point3ArrayList:
   Point3ArrayList ',' '[' Vector3List ']' {
     GEOM_PARSER_ADD_MATRIX($1,$4,$$);
   }
   |  '[' Vector3List ']' {
     GEOM_PARSER_INIT_MATRIX(Point3Matrix,$2,$$);
   };

Point3Matrix:
   '[' Point3ArrayList ']' {
     GEOM_PARSER_CREATE_MATRIX(Point3Matrix,$2,$$);
   };

Point4Array:
   '[' Vector4List ']' {
     GEOM_PARSER_CREATE_ARRAY(Point4Array,$2,$$);
   };

Point4ArrayList:
   Point4ArrayList ','  '[' Vector4List ']' {
     GEOM_PARSER_ADD_MATRIX($1,$4,$$);
   }
   | '[' Vector4List ']' {
     GEOM_PARSER_INIT_MATRIX(Point4Matrix,$2,$$);
   };

Point4Matrix:
   '[' Point4ArrayList ']' {
     GEOM_PARSER_CREATE_MATRIX(Point4Matrix,$2,$$);
   };

Primitive:
   GeometryRef { if($1) {$$ = new PrimitivePtr; $$->cast(*$1);delete $1;} else $$ = NULL; }
 | PrimitiveObj  { if($1) { $$ = new PrimitivePtr; $$->cast(*$1); delete $1; } else $$ = NULL; };

Real :
   RealAtom {$$ = $1;}
 | Real TokPLUS  Real {$$ = new real_t(*$1 + *$3); delete $1; delete $3;}
 | Real TokMINUS Real {$$ = new real_t(*$1 - *$3); delete $1; delete $3;}
 | Real TokTIMES Real {$$ = new real_t(*$1 * *$3); delete $1; delete $3;}
 | Real TokSLASH Real {$$ = new real_t(*$1 / *$3); delete $1; delete $3;}
 | TokMINUS Real %prec UMINUS {$$ =  new real_t(- *$2); delete $2;}
 | TokPLUS  Real %prec UPLUS {$$ =  $2;}
 | TokPow '(' Real ',' Real ')' { if(!$3||!$5){$$=NULL;if($3)delete $3; if($5)delete $5;}else {$$ = new real_t(pow(*$3,*$5));delete $3; delete $5;}}
 | TokCos '(' Angle ')' { if(!$3)$$=NULL;else {$$ = new real_t(cos(*$3));delete $3;} }
 | TokSin '(' Angle ')' { if(!$3)$$=NULL;else {$$ = new real_t(sin(*$3));delete $3;} }
 | TokTan '(' Angle ')' { if(!$3)$$=NULL;else {$$ = new real_t(tan(*$3));delete $3;} }
 | TokDot '(' Vector4 ',' Vector4 ')' { if(!$3||!$5){$$=NULL;if($3)delete $3; if($5)delete $5;}else {$$ = new real_t(dot(*$3,*$5));delete $3; delete $5;}}
 | TokDot '(' Vector3 ',' Vector3 ')' { if(!$3||!$5){$$=NULL;if($3)delete $3; if($5)delete $5;}else {$$ = new real_t(dot(*$3,*$5));delete $3; delete $5;}}
 | TokDot '(' Vector2 ',' Vector2 ')' { if(!$3||!$5){$$=NULL;if($3)delete $3; if($5)delete $5;}else {$$ = new real_t(dot(*$3,*$5));delete $3; delete $5;}}
 | TokNorm '(' Vector4 ')' { if(!$3)$$=NULL;else {$$ = new real_t(norm(*$3));delete $3;}}
 | TokNorm '(' Vector3 ')' { if(!$3)$$=NULL;else {$$ = new real_t(norm(*$3));delete $3;}}
 | TokNorm '(' Vector2 ')' { if(!$3)$$=NULL;else {$$ = new real_t(norm(*$3));delete $3;}}
 | '(' Real ')' { $$ = $2 ;};

RealAtom :
   TokInt { $$ = new real_t(*$1); delete $1; }
 | TokReal { $$ = $1; }
 | TokPi { $$ = new real_t(180); };

RealArray:
   '[' RealList ']' {
     GEOM_PARSER_CREATE_ARRAY(RealArray,$2,$$);
   };

RealList:
   RealList ',' Real {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Real {
     GEOM_PARSER_INIT_LIST(real_t,$1,$$);
   };

RealArrayList:
   RealArrayList ',' '[' RealList ']' {
     GEOM_PARSER_ADD_MATRIX($1,$4,$$);
   }
 | '[' RealList ']' {
     GEOM_PARSER_INIT_MATRIX(RealArray2,$2,$$);
   };

RealMatrix:
   '[' RealArrayList ']' {
     GEOM_PARSER_CREATE_MATRIX(RealArray2,$2,$$);
   };

Integer :
   TokInt {$$ = $1;}
 | Integer TokPLUS  Integer {$$ = new int32_t(*$1 + *$3); delete $1; delete $3;}
 | Integer TokMINUS Integer {$$ = new int32_t(*$1 - *$3); delete $1; delete $3;}
 | Integer TokTIMES Integer {$$ = new int32_t(*$1 * *$3); delete $1; delete $3;}
 | Integer TokSLASH Integer {$$ = new int32_t(*$1 / *$3); delete $1; delete $3;}
 | TokMINUS Integer %prec UMINUS {$$ =  new int32_t(- *$2); delete $2;}
 | TokPLUS  Integer %prec UPLUS  {$$ =  $2;}
 | '(' Integer ')' { $$ = $2 ;};


Uchar:
   Integer {
     GEOM_ASSERT($1);
     if ((*$1 >= 0) && (*$1 <= 255))
       $$ = new uchar_t(*$1);
     else
       $$ = NULL;
     delete $1;
   };

Uint32:
  Integer {
     GEOM_ASSERT($1);
     if (*$1 >= 0)
       $$ = new uint32_t(*$1);
     else
       $$ = NULL;
     delete $1;
  };

Uint32List:
   Uint32List ',' Uint32 {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Uint32 {
     GEOM_PARSER_INIT_LIST(uint32_t,$1,$$);
   };

Skeleton:
   GeometryRef { if($1) { $$ = new PolylinePtr; $$->cast(*$1); delete $1; } else $$ = NULL; }
 | PolylineObj { if($1) { $$ = new PolylinePtr; $$->cast(*$1); delete $1; } else $$ = NULL; };

Vector2:
   Vector2Atom  {$$ = $1;}
 | Vector2 TokPLUS  Vector2  {
   if(($1)&&($3)){
     *$1 += *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector2 TokMINUS Vector2  {
   if(($1)&&($3)){
     *$1 -= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector2 TokTIMES Real{
   if(($1)&&($3)){
     *$1 *= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector2 TokSLASH Real{
   if(($1)&&($3)){
     *$1 /= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 |  '(' Vector2 ')' { $$ = $2 ;};

Vector2Atom:
   '<' Real ',' Real '>' {
     if (($2) && ($4)) {
       $$ = new TOOLS(Vector2)(*$2,*$4);
       delete $2;
       delete $4;
     }
     else {
       $$ = NULL;
       if ($2) delete $2;
       if ($4) delete $4;
     }
   };

Vector2List:
   Vector2List ',' Vector2 {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Vector2 {
     GEOM_PARSER_INIT_LIST(TOOLS(Vector2),$1,$$);
   };

Vector3:
   Vector3Atom  {$$ = $1;}
 | Vector3 TokPLUS  Vector3  {
   if(($1)&&($3)){
     *$1 += *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector3 TokMINUS Vector3  {
   if(($1)&&($3)){
     *$1 -= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector3 TokTIMES Real{
   if(($1)&&($3)){
     *$1 *= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector3 TokSLASH Real{
   if(($1)&&($3)){
     *$1 /= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 |  TokCross '(' Vector3 ',' Vector3 ')' {
   if(($3)&&($5)){
     $$ = new Vector3(cross(*$3,*$5));
     delete $3;
     delete $5;
   }
   else {
     $$ = NULL;
     if($5)delete $5;
     if($3)delete $3;
   }
 }
 | TokDirection '(' Vector3 ')' { if($3)*$3 = direction(*$3); $$ = $3;}
 |  '(' Vector3 ')' { $$ = $2 ;};

Vector3Atom:
   '<' Real ',' Real ',' Real '>' {
     if (($2) && ($4) && ($6)) {
       $$ = new TOOLS(Vector3)(*$2,*$4,*$6);
       delete $2;
       delete $4;
       delete $6;
     }
     else {
       $$ = NULL;
       if ($2) delete $2;
       if ($4) delete $4;
       if ($6) delete $6;
     }
   }
 | TokOX { $$ = new Vector3(Vector3::OX); }
 | TokOY { $$ = new Vector3(Vector3::OY); }
 | TokOZ { $$ = new Vector3(Vector3::OZ); };

Vector3List:
   Vector3List ',' Vector3 {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Vector3 {
     GEOM_PARSER_INIT_LIST(TOOLS(Vector3),$1,$$);
   };

Vector4:
   Vector4Atom  {$$ = $1;}
 | Vector4 TokPLUS  Vector4  {
   if(($1)&&($3)){
     *$1 += *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector4 TokMINUS Vector4  {
   if(($1)&&($3)){
     *$1 -= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector4 TokTIMES Real{
   if(($1)&&($3)){
     *$1 *= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 | Vector4 TokSLASH Real{
   if(($1)&&($3)){
     *$1 /= *$3;
     $$ = $1;
     delete $3;
   }
   else {
     $$ = NULL;
     if($1)delete $1;
     if($3)delete $3;
   }
 }
 |  '(' Vector4 ')' { $$ = $2 ;};

Vector4Atom:
   '<' Real ',' Real ',' Real ',' Real '>' {
     if (($2) && ($4) && ($6) && ($8)) {
       $$ = new TOOLS(Vector4)(*$2,*$4,*$6,*$8);
       delete $2;
       delete $4;
       delete $6;
       delete $8;
     }
     else {
       $$ = NULL;
       if ($2) delete $2;
       if ($4) delete $4;
       if ($6) delete $6;
       if ($8) delete $8;
     }
   };


Vector4List:
   Vector4List ',' Vector4 {
     GEOM_PARSER_ADD_LIST($1,$3,$$);
   }
 | Vector4 {
     GEOM_PARSER_INIT_LIST(TOOLS(Vector4),$1,$$);
   };

%%



void parserVerbose(bool b)
{
  __verbose = b;
}

bool isParserVerbose()
{
  return (__verbose);
}
