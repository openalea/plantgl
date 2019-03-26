/*  -*-c++-*-----------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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
 *  -------------------------------------------------------------------------
 */


#include <QtGlobal>
#include <QtGui/qpainter.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QWidget>
    #include <QtWidgets/QTreeWidget>
    #include <QtWidgets/QTreeWidgetItem>
#else
    #include <QtGui/QWidget>
    #include <QtGui/QTreeWidget>
    #include <QtGui/QTreeWidgetItem>
#endif

#include "qgeomlistview.h"

#include <plantgl/pgl_scenegraph.h>

#include <plantgl/math/util_math.h>
#include "../base/icons.h"

#include "util_qstring.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


#define GEOM_MESH(obj,type) \
    addNode(obj,type); \
    addAttr("CCW",obj->getCCW()); \
    addAttr("Solid",obj->getSolid()); \
    addArray("PointList",obj->getPointList(),"Vector3"); \
    addArray("IndexList",obj->getIndexList(),"Index"); \
    addAttr("NormalPerVertex",obj->getNormalPerVertex()); \
    addArray("NormalList",obj->getNormalList(),"Vector3"); \
    addArray("NormalIndexList",obj->getNormalList(),"Index"); \
    addAttr("ColorPerVertex",obj->getColorPerVertex()); \
    addArray("ColorList",obj->getColorList(),"Color4"); \
    addArray("ColorIndexList",obj->getColorIndexList(),"Index"); \
    addArray("TexCoordList",obj->getTexCoordList(),"Vector2"); \
    addArray("TexCoordIndexList",obj->getTexCoordIndexList(),"Index"); \
    addAttrNode("Skeleton",SceneObjectPtr(obj->getSkeleton()),"Polyline"); \

/* ----------------------------------------------------------------------- */


GeomListViewBuilder::GeomListViewBuilder(QWidget * parent, char * name ) :
  Action(),
  __fullmode(true),
  __pixgeom(ViewerIcon::getPixmap(ViewerIcon::geometry)),
  __pixappe(ViewerIcon::getPixmap(ViewerIcon::appearance)),
  __pixshape(ViewerIcon::getPixmap(ViewerIcon::shape)),
  __pixtransf(ViewerIcon::getPixmap(ViewerIcon::transformed)),
  __pixatt(ViewerIcon::getPixmap(ViewerIcon::attribut)),
  __pixattptr(ViewerIcon::getPixmap(ViewerIcon::attributptr))
{
  __qListView = new QTreeWidget(parent);
  setHeader();
  init();
}

GeomListViewBuilder::GeomListViewBuilder( QTreeWidget * l ) :
  Action(),
  __fullmode(true),
  __qListView(l),
  __pixgeom(ViewerIcon::getPixmap(ViewerIcon::geometry)),
  __pixappe(ViewerIcon::getPixmap(ViewerIcon::appearance)),
  __pixshape(ViewerIcon::getPixmap(ViewerIcon::shape)),
  __pixtransf(ViewerIcon::getPixmap(ViewerIcon::transformed)),
  __pixatt(ViewerIcon::getPixmap(ViewerIcon::attribut)),
  __pixattptr(ViewerIcon::getPixmap(ViewerIcon::attributptr)){
  setHeader();
  init();
}

GeomListViewBuilder::~GeomListViewBuilder( ) {
}

/* ----------------------------------------------------------------------- */

void GeomListViewBuilder::setHeader( ){
      QStringList header;
      header << "Name" << "Value" << "Type";
      __qListView->setHeaderLabels(header);
      __qListView->setSortingEnabled(false);

      // __qListView->setTreeStepSize( 20 );
      // __qListView->setSorting(-1,TRUE);
}

void GeomListViewBuilder::clear( ){
    __qListView->clear();
    init();

}

/* ----------------------------------------------------------------------- */

void GeomListViewBuilder::init( ){
    QStringList labels;
   labels << "Global Scene" << "Root" << "Scene";
   __rootItem = new QTreeWidgetItem(__qListView, labels);
   __rootItem->setIcon(0,__pixgeom);
   __qListView->addTopLevelItem(__rootItem);
   __qListView->expandItem(__rootItem);

   __currentNodeItem     = __rootItem;
   __currentSiblingItem  = NULL;
   __currentAttrItem     = NULL;
}

/* ----------------------------------------------------------------------- */

void GeomListViewBuilder::addNode(const SceneObject * node,
                                  const QString& type,
                                  int pixmaptouse){

   QString ptrid = QString("ptr=0x%1").arg(node->SceneObject::getId(),8,16,QChar('0'));
   QString name = QString(node->getName().c_str());
   QStringList labels;
   labels << name << ptrid << type;
   if (name.isEmpty()) name = type.toLower()+"_"+QString::number(node->getId());
   QTreeWidgetItem * item = new QTreeWidgetItem(__currentNodeItem,labels);
   switch (pixmaptouse){
    default:
    case 0:
        item->setIcon(0,__pixgeom);
        break;
    case 1:
        item->setIcon(0,__pixshape);
        break;
    case 2:
        item->setIcon(0,__pixappe);
        break;
    case 3:
        item->setIcon(0,__pixtransf);
        break;
    }


   __currentNodeItem->addChild(item);
   pushItems();
   __currentNodeItem = item;
   __currentAttrItem = NULL;
   __currentSiblingItem = NULL;

}

void GeomListViewBuilder::endNode(){
  __currentSiblingItem = __currentNodeItem;
    popItems();
}

void GeomListViewBuilder::pushItems()
{
    __stackItem.push(qMakePair(__currentNodeItem,__currentAttrItem));
}

void GeomListViewBuilder::popItems()
{
  QPair<QTreeWidgetItem *,QTreeWidgetItem *> items = __stackItem.pop();
  __currentNodeItem = items.first;
  __currentAttrItem = items.second;
}

void GeomListViewBuilder::addAttr(const QString& name, bool value){
    addAttr(name,QString::number(value),"Boolean");
}

void GeomListViewBuilder::addAttr(const QString& name, real_t value){
    addAttr(name,QString::number(value),"Real");
}

void GeomListViewBuilder::addAttrAngle(const QString& name, real_t value){
    addAttr(name,QString::number(value*GEOM_DEG),"Degrees");
}

void GeomListViewBuilder::addAttr(const QString& name, uint_t value){
    addAttr(name,QString::number(value),"UInt32");
}

void GeomListViewBuilder::addAttr(const QString& name, uint16_t value){
    addAttr(name,QString::number(value),"UInt16");
}

void GeomListViewBuilder::addAttr(const QString& name, uchar_t value){
    addAttr(name,QString::number(value),"UInt8");
}

void GeomListViewBuilder::addAttr(const QString& name, const Vector2& value){
    addAttr(name,toQString(value),"Vector2");
}

void GeomListViewBuilder::addAttr(const QString& name, const Vector3& value){
    addAttr(name,toQString(value),"Vector3");
}

void GeomListViewBuilder::addAttr(const QString& name, const Vector4& value){
    addAttr(name,toQString(value),"Vector4");
}

void GeomListViewBuilder::addAttr(const QString& name, const Color3& value){
    addAttr(name,toQString(value),"Color3");
}

void GeomListViewBuilder::addAttr(const QString& name, const Color4& value){
    addAttr(name,toQString(value),"Color4");
}

void GeomListViewBuilder::addAttr(const QString& name, const Index& value){
    addAttr(name,toQString(value),"Index");
}

void GeomListViewBuilder::addAttr(const QString& name, const Index3& value){
    addAttr(name,toQString(value),"Index3");
}

void GeomListViewBuilder::addAttr(const QString& name, const Index4& value){
    addAttr(name,toQString(value),"Index4");
}

void GeomListViewBuilder::addAttr(const QString& name,
                                  const QString& value,
                                  const QString& type ){
   QStringList labels;
   labels << name << value << type;
   QTreeWidgetItem * item = new QTreeWidgetItem(__currentNodeItem,labels);
   item->setIcon(0,__pixatt);
   __currentNodeItem->addChild(item);
   __currentAttrItem = item;

}

void GeomListViewBuilder::addAttr(const QString& name, const Transform4Ptr& value){
    pushItems();
    __currentNodeItem = __currentAttrItem;
    __currentAttrItem = NULL;
    QString val = "ptr="+(value?QString("0x%1").arg(ptr_to_uint32(value),8,16,QChar('0')):"NULL");
    // QString val = "ptr="+(!value.isNull()?QString("0x%1").arg(value.toUint32()):"NULL");
    addAttrPtr(name, val,"Transform4");

    if(value){
        Vector3 scale, rotate, translate;
        value->getTransformation( scale, rotate, translate );
        addAttr("Scale",scale);
        addAttrAngle("Azimuth",rotate.x());
        addAttrAngle("Elevation",rotate.y());
        addAttrAngle("Roll",rotate.z());
        addAttr("Translation",translate);
    }
    popItems();
}

void GeomListViewBuilder::addAttrPtr(const QString& name,
                                     const QString& value,
                                     const QString& type ){
   QStringList labels; labels << name << value << type;
   QTreeWidgetItem * item = new QTreeWidgetItem(__currentNodeItem,
                                               // __currentAttrItem,
                                              labels);
   item->setIcon(0,__pixattptr);
   __currentNodeItem->addChild(item);
   __currentAttrItem = item;
}

template <class T>
void GeomListViewBuilder::addArray(const QString& name, const T& _array, const QString& type)
{
  addAttrPtr(name,
             "ptr="+(_array?QString("0x%1").arg(ptr_to_uint32(_array),8,16,QChar('0')):"NULL"),
//           "ptr="+(_array?QString("0x%1").arg(ptr_to_uint32(_array):"NULL"),
             "Array<"+type+">["+QString::number(_array?_array->size():0)+']');

  if(__fullmode && _array){
      pushItems();
    __currentNodeItem = __currentAttrItem;
    __currentAttrItem = NULL;

    for(uint_t it = 0; it < _array->size() ; it++) {
        addAttr('['+QString::number(it)+']',_array->getAt(it));
    }
    popItems();
  }
}

void GeomListViewBuilder::addArrayAngle(const QString& name, const RealArrayPtr& _array)
{
  addAttrPtr(name,
             (_array?QString("ptr=0x%1").arg(ptr_to_uint32(_array),8,16,QChar('0')):"ptr=NULL"),
//           (_array?QString("ptr=0x%1").arg(ptr_to_uint32(_array)):"ptr=NULL"),
             "Array<Angle>["+QString::number(_array?_array->size():0)+']');

  if(__fullmode && _array){
      pushItems();
    __currentNodeItem = __currentAttrItem;
    __currentAttrItem = NULL;

    for(uint_t it = 0; it < _array->size() ; it++) {
        addAttrAngle('['+QString::number(it)+']',_array->getAt(it));
    }
    popItems();
  }
}

template <class T>
void GeomListViewBuilder::addArrayNode(const QString& name, const T& _array, const QString& type)
{
  addAttrPtr(name,
             "ptr="+(_array?QString("0x%1").arg(ptr_to_uint32(_array),8,16,QChar('0')):"NULL"),
//           "ptr="+(_array?QString("0x%1").arg(ptr_to_uint32(_array)):"NULL"),
             "Array<"+type+">["+QString::number(_array?_array->size():0)+']');

  if( _array){
    pushItems();
    __currentNodeItem = __currentAttrItem;
    __currentAttrItem = NULL;

    for(uint_t it = 0; it < _array->size() ; it++) {
        _array->getAt(it)->apply(*this);
    }
    popItems();
  }
}


template <class T>
void GeomListViewBuilder::addMatrix(const QString& name, const T& _matrix, const QString& type)
{
  addAttrPtr(name,
             "ptr="+(_matrix?QString("0x%1").arg(ptr_to_uint32(_matrix),8,16,QChar('0')):"NULL"),
//           "ptr="+(_matrix?QString("0x%1").arg(ptr_to_uint32(_matrix)):"NULL"),
             "Matrix<"+type+">["+QString::number(_matrix?_matrix->getColumnSize():0)+','+
             QString::number(_matrix?_matrix->getRowSize():0)+']');

  if(__fullmode && _matrix){
      pushItems();
    __currentNodeItem = __currentAttrItem;
    __currentAttrItem = NULL;

    for(uint_t it1 = 0; it1 < _matrix->getColumnSize() ; it1++) {
        for(uint_t it2 = 0; it2 < _matrix->getRowSize() ; it2++) {
            addAttr('['+QString::number(it1)+','+QString::number(it2)+']',_matrix->getAt(it1,it2));
        }
    }
    popItems();
  }
}



void GeomListViewBuilder::addAttrNode(const QString& name,
                                     const SceneObjectPtr& obj,
                                     const QString& type ){
   QString value = "ptr="+(obj?QString("0x%1").arg(ptr_to_uint32(obj),8,16,QChar('0')):"NULL");
//   QString value = "ptr="+(obj?QString("0x%1").arg(ptr_to_uint32(obj)):"NULL");
   pushItems();
   addAttrPtr(name,value,type);
   __currentNodeItem = __currentAttrItem;
   __currentAttrItem = NULL;
   if(obj){
    obj->apply(*this);
   }
   popItems();
}

/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder::process(Shape * geomShape){
   addNode(geomShape,"Shape",1);
   if(geomShape->getId() == Shape::NOID)addAttr("Id","None","");
   else addAttr("Id",geomShape->getId());
   if(geomShape->getParentId() != Shape::NOID)addAttr("ParentId",geomShape->getParentId());
   addAttrNode("Geometry",SceneObjectPtr(geomShape->geometry),"GeometryPtr");
   addAttrNode("Appearance",SceneObjectPtr(geomShape->appearance),"AppearancePtr");
   endNode();
   return true;
}

/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  GEOM_MESH(amapSymbol,"AmapSymbol")
  addAttr("FileName",QString(amapSymbol->getFileName().c_str()),"String");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);
  addNode(asymmetricHull,"AsymmetricHull");
  addAttr("Bottom",asymmetricHull->getBottom());
  addAttr("Top",asymmetricHull->getTop());
  addAttr("NegXHeight",asymmetricHull->getNegXHeight());
  addAttr("NegXRadius",asymmetricHull->getNegXRadius());
  addAttr("NegYHeight",asymmetricHull->getNegYHeight());
  addAttr("NegYRadius",asymmetricHull->getNegYRadius());
  addAttr("PosXHeight",asymmetricHull->getPosXHeight());
  addAttr("PosXRadius",asymmetricHull->getPosXRadius());
  addAttr("PosYHeight",asymmetricHull->getPosYHeight());
  addAttr("PosYRadius",asymmetricHull->getPosYRadius());
  addAttr("BottomShape",asymmetricHull->getBottomShape());
  addAttr("TopShape",asymmetricHull->getTopShape());
  addAttr("Slices",asymmetricHull->getSlices());
  addAttr("Stacks",asymmetricHull->getStacks());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);
  addNode(axisRotated,"AxisRotated",3);
  addAttr("Axis",axisRotated->getAxis());
  addAttrAngle("Angle",axisRotated->getAngle());
  addAttrNode("Geometry",SceneObjectPtr(axisRotated->getGeometry()),"GeometryPtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  addNode(bezierCurve,"BezierCurve");
  addAttr("Degree",bezierCurve->getDegree());
  addAttr("Stride",bezierCurve->getStride());
  addAttr("Width",bezierCurve->getWidth());
  addArray("CtrlPointList",bezierCurve->getCtrlPointList(),"Vector4");
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);
  addNode(bezierPatch,"BezierPatch");
  addAttr("UDegree",bezierPatch->getUDegree());
  addAttr("VDegree",bezierPatch->getVDegree());
  addAttr("UStride",bezierPatch->getVStride());
  addAttr("VStride",bezierPatch->getVStride());
  addMatrix("CtrlPointMatrix",bezierPatch->getCtrlPointMatrix(),"Vector4");
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Box * box ) {
  GEOM_ASSERT(box);
  addNode(box,"Box");
  addAttr("Size",box->getSize());
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Cone * cone ) {
  GEOM_ASSERT(cone);
  addNode(cone,"Cone");
  addAttr("Height",cone->getHeight());
  addAttr("Radius",cone->getRadius());
  addAttr("Slices",cone->getSlices());
  addAttr("Solid",cone->getSolid());
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);
  addNode(cylinder,"Cylinder");
  addAttr("Height",cylinder->getHeight());
  addAttr("Radius",cylinder->getRadius());
  addAttr("Slices",cylinder->getSlices());
  addAttr("Solid",cylinder->getSolid());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);
  addNode(elevationGrid,"ElevationGrid");
  addAttr("XSpacing",elevationGrid->getXSpacing());
  addAttr("YSpacing",elevationGrid->getYSpacing());
  addMatrix("HeightList",elevationGrid->getHeightList(),"Real");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);
  addNode(eulerRotated,"EulerRotated");
  addAttrAngle("Azimuth",eulerRotated->getAzimuth());
  addAttrAngle("Elevation",eulerRotated->getElevation());
  addAttrAngle("Roll",eulerRotated->getRoll());
  addAttrNode("Geometry",SceneObjectPtr(eulerRotated->getGeometry()),"Geometry");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);
  addNode(extrudedHull,"ExtrudedHull");
  addAttr("CCW",extrudedHull->getCCW());
  addAttrNode("Horizontal",SceneObjectPtr(extrudedHull->getHorizontal()),"Curve2DPtr");
  addAttrNode("Vertical",SceneObjectPtr(extrudedHull->getVertical()),"Curve2DPtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  GEOM_MESH(faceSet,"FaceSet");
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);
  addNode(frustum,"Frustum");
  addAttr("Height",frustum->getHeight());
  addAttr("Radius",frustum->getRadius());
  addAttr("Taper",frustum->getTaper());
  addAttr("Slices",frustum->getSlices());
  addAttr("Solid",frustum->getSolid());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Extrusion * extrusion ){
  GEOM_ASSERT(extrusion);
  addNode(extrusion,"Extrusion");
  addAttrNode("Axis",SceneObjectPtr(extrusion->getAxis()),"CurvePtr");
  addAttrNode("CrossSection",SceneObjectPtr(extrusion->getCrossSection()),"Curve2DPtr");
  addAttr("CCW",extrusion->getCCW());
  addAttr("Solid",extrusion->getSolid());
  if(extrusion->getProfileTransformation()){
      const ProfileTransformationPtr& prof = extrusion->getProfileTransformation();
      addArray("Scale",prof->getScale(),"Vector2");
      addArrayAngle("Orientation",prof->getOrientation());
      addArray("KnotList",prof->getKnotList(),"Real");
  }
  addAttr("InitialNormal",extrusion->getInitialNormal());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Group * group ) {
  GEOM_ASSERT(group);
  addNode(group,"Group",3);
  addArrayNode("GeometryList",group->getGeometryList(),"Geometry");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( IFS * ifs ) {
  GEOM_ASSERT(ifs);
  addNode(ifs,"Iterative Function System",3);
  addAttr("Depth",ifs->getDepth());
  addArray("TransfoList",ifs->getTransfoList(),"Transform4");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Material * material ) {
  GEOM_ASSERT(material);
  addNode(material,"Material",2);
  addAttr("Ambient",material->getAmbient());
  addAttr("Diffuse",material->getDiffuse());
  addAttr("Specular",material->getSpecular());
  addAttr("Emission",material->getEmission());
  addAttr("Shininess",material->getShininess());
  addAttr("Transparency",material->getTransparency());

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder::process( Texture2D * texture ) {
  GEOM_ASSERT(texture);
  addNode(texture,"Texture2D",2);
  addAttrNode("Image",SceneObjectPtr(texture->getImage()),"ImageTexture");
  addAttrNode("Transformation",SceneObjectPtr(texture->getTransformation()),"Texture2DTransformation");
  addAttr("BaseColor",texture->getBaseColor());
  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  addNode(texture,"ImageTexture",2);
  addAttr("Filename",QString(texture->getFilename().c_str()),"String");
  addAttr("Mipmaping",texture->getMipmaping());
  addAttr("RepeatS",texture->getRepeatS());
  addAttr("RepeatT",texture->getRepeatT());

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Texture2DTransformation * texturetransfo ) {
  GEOM_ASSERT(texturetransfo);
  addNode(texturetransfo,"Texture2DTransformation",2);
  addAttr("Scale",texturetransfo->getScale());
  addAttr("Translation",texturetransfo->getTranslation());
  addAttr("RotationCenter",texturetransfo->getRotationCenter());
  addAttr("RotationAngle",texturetransfo->getRotationAngle());

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  addNode(monoSpectral,"MonoSpectral",2);

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  addNode(multiSpectral,"MultiSpectral",2);

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  addNode(nurbsCurve,"NurbsCurve");
  addAttr("Degree",nurbsCurve->getDegree());
  addAttr("Stride",nurbsCurve->getStride());
  addAttr("Width",nurbsCurve->getWidth());
  addArray("CtrlPointList",nurbsCurve->getCtrlPointList(),"Vector4");
  addArray("KnotList",nurbsCurve->getKnotList(),"Real");

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  addNode(nurbsPatch,"NurbsPatch");
  addAttr("UDegree",nurbsPatch->getUDegree());
  addAttr("VDegree",nurbsPatch->getVDegree());
  addAttr("UStride",nurbsPatch->getUStride());
  addAttr("VStride",nurbsPatch->getVStride());
  addMatrix("CtrlPointMatrix",nurbsPatch->getCtrlPointMatrix(),"Vector4");
  addArray("UKnotList",nurbsPatch->getUKnotList(),"Real");
  addArray("VKnotList",nurbsPatch->getVKnotList(),"Real");

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);
  addNode(oriented,"Oriented",3);
  addAttr("Primary",oriented->getPrimary());
  addAttr("Secondary",oriented->getSecondary());
  addAttrNode("Geometry",SceneObjectPtr(oriented->getGeometry()),"GeometryPtr");

  endNode();
  return true;

}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);

  addNode(paraboloid,"Paraboloid");
  addAttr("Height",paraboloid->getHeight());
  addAttr("Radius",paraboloid->getRadius());
  addAttr("Shape",paraboloid->getShape());
  addAttr("Slices",paraboloid->getSlices());
  addAttr("Stacks",paraboloid->getStacks());
  addAttr("Solid",paraboloid->getSolid());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);

  addNode(pointSet,"PointSet");
  addArray("PointList",pointSet->getPointList(),"Vector3");
  addArray("ColorList",pointSet->getColorList(),"Color4");
  addAttr("Width",pointSet->getWidth());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);

  addNode(polyline,"Polyline");
  addArray("PointList",polyline->getPointList(),"Vector3");
  addArray("ColorList",polyline->getColorList(),"Color4");
  addAttr("Width",polyline->getWidth());
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);

  addNode(revolution,"Revolution");
  addAttrNode("Profile",SceneObjectPtr(revolution->getProfile()),"Curve2DPtr");
  addAttr("Slices",revolution->getSlices());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Swung * swung )
{
  GEOM_ASSERT(swung);

  addNode(swung,"Swung");
  addArrayNode("ProfileList",swung->getProfileList(),"Curve2D");
  addArrayAngle("AngleList",swung->getAngleList());

  addAttr("Slices",swung->getSlices());
  addAttr("CCW",swung->getCCW());
  addAttr("Degree",swung->getDegree());
  addAttr("Stride",swung->getStride());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  GEOM_MESH(quadSet,"QuadSet");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);

  addNode(scaled,"Scaled",3);
  addAttr("Scale",scaled->getScale());
  addAttrNode("Geometry",SceneObjectPtr(scaled->getGeometry()),"GeometryPtr");


 endNode();
 return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( ScreenProjected * scp ) {
  GEOM_ASSERT(scp);

  addNode(scp,"ScreenProjected",3);
  addAttr("KeepAspectRatio",scp->getKeepAspectRatio());
  addAttrNode("Geometry",SceneObjectPtr(scp->getGeometry()),"GeometryPtr");


 endNode();
 return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);

  addNode(sphere,"Sphere");
  addAttr("Radius",sphere->getRadius());
  addAttr("Slices",sphere->getSlices());
  addAttr("Stacks",sphere->getStacks());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);

  addNode(tapered,"Tapered",3);
  addAttr("BaseRadius",tapered->getBaseRadius());
  addAttr("TopRadius",tapered->getTopRadius());
  addAttrNode("Primitive",SceneObjectPtr(tapered->getPrimitive()),"PrimitivePtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Translated * translated ) {
  GEOM_ASSERT(translated);

  addNode(translated,"Translated",3);
  addAttr("Translation",translated->getTranslation());
  addAttrNode("Geometry",SceneObjectPtr(translated->getGeometry()),"GeometryPtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  GEOM_MESH(triangleSet,"TriangleSet");

  endNode();
  return true;
}


/* ------------------------------------------------------------------------- */


bool GeomListViewBuilder::process( BezierCurve2D * bezierCurve  ) {
  GEOM_ASSERT(bezierCurve);
  addNode(bezierCurve,"BezierCurve2D");
  addAttr("Degree",bezierCurve->getDegree());
  addAttr("Stride",bezierCurve->getStride());
  addAttr("Width",bezierCurve->getWidth());
  addArray("CtrlPointList",bezierCurve->getCtrlPointList(),"Vector3");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( Disc * disc ) {
  GEOM_ASSERT(disc);

  addNode(disc,"Disc");
  addAttr("Radius",disc->getRadius());
  addAttr("Slices",disc->getSlices());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder::process( NurbsCurve2D * nurbsCurve  ) {
  GEOM_ASSERT(nurbsCurve);

  addNode(nurbsCurve,"NurbsCurve2D");
  addAttr("Degree",nurbsCurve->getDegree());
  addAttr("Stride",nurbsCurve->getStride());
  addAttr("Width",nurbsCurve->getWidth());
  addArray("CtrlPointList",nurbsCurve->getCtrlPointList(),"Vector3");
  addArray("KnotList",nurbsCurve->getKnotList(),"Real");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */



bool GeomListViewBuilder::process( PointSet2D *  pointSet ) {
  GEOM_ASSERT(pointSet);

  addNode(pointSet,"PointSet2D");
  addArray("PointList",pointSet->getPointList(),"Vector2");
  addAttr("Width",pointSet->getWidth());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */



bool GeomListViewBuilder::process( Polyline2D * polyline  ) {
  GEOM_ASSERT(polyline);

  addNode(polyline,"Polyline2D");
  addArray("PointList",polyline->getPointList(),"Vector2");
  addAttr("Width",polyline->getWidth());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder::process( Text * text  ) {
  GEOM_ASSERT(text);
  addNode(text,"Text");
  addAttr("String",QString(text->getString().c_str()),"String");
  addAttr("Position",text->getPosition());
  addAttr("ScreenCoordinates",text->getScreenCoordinates());
  addAttrNode("FontStyle",SceneObjectPtr(text->getFontStyle()),"Font");

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder::process( Font * font ) {
  GEOM_ASSERT(font);
  addNode(font,"Font");

  addAttr("Family",font->getFamily().c_str(),"String");
  addAttr("Size",font->getSize());
  addAttr("Bold",font->getBold());
  addAttr("Italic",font->getItalic());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */
