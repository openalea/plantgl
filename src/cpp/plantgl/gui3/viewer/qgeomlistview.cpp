/*  -*-c++-*-----------------------------------------------------------------
 *  AMAP                            CIRAD/INRA - UMR Modelisation des Plantes
 *
 *  $Source: /usr/cvsmaster/AMAPmod/src/GEOM/actn_qgeomlistview.cpp,v $
 *  $Id$
 *
 *  -------------------------------------------------------------------------
 *
 *  Note:
 *
 *  -------------------------------------------------------------------------
 *
 *
 *
 *  -------------------------------------------------------------------------
 */

#include <qpainter.h>
#include <qlistview.h>

#include "qgeomlistview.h"

#include <plantgl/pgl_scenegraph.h>

#include <plantgl/math/util_math.h>
#include "../base/icons.h"

#include "util_qstring.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

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


GeomListViewBuilder3::GeomListViewBuilder3(QWidget * parent, char * name ) :
  Action(),
  __fullmode(true),
  __pixgeom(ViewerIcon3::getPixmap(ViewerIcon3::icon_geometry)),
  __pixappe(ViewerIcon3::getPixmap(ViewerIcon3::icon_appearance)),
  __pixshape(ViewerIcon3::getPixmap(ViewerIcon3::icon_shape)),
  __pixtransf(ViewerIcon3::getPixmap(ViewerIcon3::icon_transformed)),
  __pixatt(ViewerIcon3::getPixmap(ViewerIcon3::icon_attribut)),
  __pixattptr(ViewerIcon3::getPixmap(ViewerIcon3::icon_attributptr))
{
  __qListView = new QListView(parent,name);
  __qListView->addColumn( "Name" );
  __qListView->addColumn( "Value" );
  __qListView->addColumn( "Type" );
//  __qListView->addColumn( "Comment" );
  __qListView->setTreeStepSize( 20 );
  __qListView->setSorting(-1,TRUE);
  init();
}

GeomListViewBuilder3::GeomListViewBuilder3( QListView * l ) :
  Action(),
  __fullmode(true),
  __qListView(l),
  __pixgeom(ViewerIcon3::getPixmap(ViewerIcon3::icon_geometry)),
  __pixappe(ViewerIcon3::getPixmap(ViewerIcon3::icon_appearance)),
  __pixshape(ViewerIcon3::getPixmap(ViewerIcon3::icon_shape)),
  __pixtransf(ViewerIcon3::getPixmap(ViewerIcon3::icon_transformed)),
  __pixatt(ViewerIcon3::getPixmap(ViewerIcon3::icon_attribut)),
  __pixattptr(ViewerIcon3::getPixmap(ViewerIcon3::icon_attributptr)){
  __qListView->addColumn( "Name" );
  __qListView->addColumn( "Value" );
  __qListView->addColumn( "Type" );
  __qListView->setTreeStepSize( 20 );
  __qListView->setSorting(-1,TRUE);
  init();
}

GeomListViewBuilder3::~GeomListViewBuilder3( ) {
}

/* ----------------------------------------------------------------------- */

void GeomListViewBuilder3::clear( ){
    __qListView->clear();
    init();

}

QListView * 
GeomListViewBuilder3::getQListView(){
  return __qListView;
}

/* ----------------------------------------------------------------------- */

void GeomListViewBuilder3::init( ){
   __rootItem = new QListViewItem(__qListView,"Global Scene","Root","Scene","The Global Scene");
   __rootItem->setPixmap(0,__pixgeom);
   __rootItem->setOpen(TRUE);
   __currentNodeItem     = __rootItem;
   __currentSiblingItem  = NULL;
   __currentAttrItem     = NULL;
   __qListView->insertItem(__rootItem);
}

/* ----------------------------------------------------------------------- */

void GeomListViewBuilder3::addNode(const SceneObjectPtr& node,
								  const QString& type,
								  int pixmaptouse){

// QString ptrid = QString("ptr=0x%1").arg(node->SceneObject::getId(),8,16,QChar('0'));
   QString ptrid = QString("ptr=0x%1").arg(node->SceneObject::getId());
   QString name = QString(node->getName().c_str());
   if (name.isEmpty()) name = type.lower(); //+"_"+QString::number(node->getId());
   QListViewItem * item = new QListViewItem(__currentNodeItem,__currentSiblingItem,name,ptrid,type);
   switch (pixmaptouse){
	default:
	case 0:
		item->setPixmap(0,__pixgeom);
		break;
	case 1:
		item->setPixmap(0,__pixshape);
		break;
	case 2:
		item->setPixmap(0,__pixappe);
		break;
	case 3:
		item->setPixmap(0,__pixtransf);
		break;
	}


   __currentNodeItem->insertItem(item);
   pushItems();
   __currentNodeItem = item;
   __currentAttrItem = NULL;
   __currentSiblingItem = NULL;

}

void GeomListViewBuilder3::endNode(){
  __currentSiblingItem = __currentNodeItem;
	popItems();
}

void GeomListViewBuilder3::pushItems()
{
	QPair<QListViewItem *,QListViewItem *> * items = new QPair<QListViewItem *,QListViewItem *>(__currentNodeItem,__currentAttrItem);
	__stackItem.push(items);
}

void GeomListViewBuilder3::popItems()
{
  QPair<QListViewItem *,QListViewItem *> * items = __stackItem.pop();
  __currentNodeItem = items->first;
  __currentAttrItem = items->second;
  delete items;
}

void GeomListViewBuilder3::addAttr(const QString& name, bool value){
	addAttr(name,QString::number(value),"Boolean");
}

void GeomListViewBuilder3::addAttr(const QString& name, real_t value){
	addAttr(name,QString::number(value),"Real");
}

void GeomListViewBuilder3::addAttrAngle(const QString& name, real_t value){
	addAttr(name,QString::number(value*GEOM_DEG),"Degrees");
}

void GeomListViewBuilder3::addAttr(const QString& name, uint_t value){
	addAttr(name,QString::number(value),"UInt32");
}

void GeomListViewBuilder3::addAttr(const QString& name, uint16_t value){
	addAttr(name,QString::number(value),"UInt16");
}

void GeomListViewBuilder3::addAttr(const QString& name, uchar_t value){
	addAttr(name,QString::number(value),"UInt8");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Vector2& value){
	addAttr(name,toQString(value),"Vector2");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Vector3& value){
	addAttr(name,toQString(value),"Vector3");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Vector4& value){
	addAttr(name,toQString(value),"Vector4");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Color3& value){
	addAttr(name,toQString(value),"Color3");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Color4& value){
	addAttr(name,toQString(value),"Color4");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Index& value){
	addAttr(name,toQString(value),"Index");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Index3& value){
	addAttr(name,toQString(value),"Index3");
}

void GeomListViewBuilder3::addAttr(const QString& name, const Index4& value){
	addAttr(name,toQString(value),"Index4");
}

void GeomListViewBuilder3::addAttr(const QString& name,
								  const QString& value,
								  const QString& type ){
   QListViewItem * item = new QListViewItem(__currentNodeItem,__currentAttrItem,name,value,type);
   item->setPixmap(0,__pixatt);
   __currentNodeItem->insertItem(item);
   __currentAttrItem = item;

}

void GeomListViewBuilder3::addAttr(const QString& name, const Transform4Ptr& value){
	pushItems();
	__currentNodeItem = __currentAttrItem;
	__currentAttrItem = NULL;
    // QString val = "ptr="+(!value.isNull()?QString("0x%1").arg(value.toUint32(),8,16,QChar('0')):"NULL");
    QString val = "ptr="+(!value.isNull()?QString("0x%1").arg(value.toUint32()):"NULL");
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

void GeomListViewBuilder3::addAttrPtr(const QString& name,
								     const QString& value,
									 const QString& type ){
   QListViewItem * item = new QListViewItem(__currentNodeItem,
											  __currentAttrItem,
											  name,value,type);
   item->setPixmap(0,__pixattptr);
   __currentNodeItem->insertItem(item);
   __currentAttrItem = item;
}

template <class T>
void GeomListViewBuilder3::addArray(const QString& name, const T& _array, const QString& type)
{
  addAttrPtr(name,
//			 "ptr="+(!_array.isNull()?QString("0x%1").arg(_array.toUint32(),8,16,QChar('0')):"NULL"),
			 "ptr="+(!_array.isNull()?QString("0x%1").arg(_array.toUint32()):"NULL"),
			 "Array<"+type+">["+QString::number(_array?_array->getSize():0)+']');

  if(__fullmode && _array.isValid()){ 
	  pushItems();
	__currentNodeItem = __currentAttrItem;
	__currentAttrItem = NULL;

    for(uint_t it = 0; it < _array->getSize() ; it++) { 
		addAttr('['+QString::number(it)+']',_array->getAt(it));
    }
	popItems();
  } 
}

void GeomListViewBuilder3::addArrayAngle(const QString& name, const RealArrayPtr& _array)
{
  addAttrPtr(name,
//			 (!_array.isNull()?QString("ptr=0x%1").arg(_array.toUint32(),8,16,QChar('0')):"ptr=NULL"),
			 (!_array.isNull()?QString("ptr=0x%1").arg(_array.toUint32()):"ptr=NULL"),
			 "Array<Angle>["+QString::number(_array?_array->getSize():0)+']');

  if(__fullmode && _array.isValid()){ 
	  pushItems();
	__currentNodeItem = __currentAttrItem;
	__currentAttrItem = NULL;

    for(uint_t it = 0; it < _array->getSize() ; it++) { 
		addAttrAngle('['+QString::number(it)+']',_array->getAt(it));
    }
	popItems();
  } 
}

template <class T>
void GeomListViewBuilder3::addArrayNode(const QString& name, const T& _array, const QString& type)
{
  addAttrPtr(name,
//			 "ptr="+(!_array.isNull()?QString("0x%1").arg(_array.toUint32(),8,16,QChar('0')):"NULL"),
			 "ptr="+(!_array.isNull()?QString("0x%1").arg(_array.toUint32()):"NULL"),
			 "Array<"+type+">["+QString::number(_array?_array->getSize():0)+']');

  if( _array.isValid()){ 
	pushItems();
	__currentNodeItem = __currentAttrItem;
	__currentAttrItem = NULL;

    for(uint_t it = 0; it < _array->getSize() ; it++) { 
		_array->getAt(it)->apply(*this);
    }
	popItems();
  } 
}


template <class T>
void GeomListViewBuilder3::addMatrix(const QString& name, const T& _matrix, const QString& type)
{
  addAttrPtr(name,
//			 "ptr="+(!_matrix.isNull()?QString("0x%1").arg(_matrix.toUint32(),8,16,QChar('0')):"NULL"),
			 "ptr="+(!_matrix.isNull()?QString("0x%1").arg(_matrix.toUint32()):"NULL"),
			 "Matrix<"+type+">["+QString::number(_matrix?_matrix->getColsSize():0)+','+
			 QString::number(_matrix?_matrix->getRowsSize():0)+']');

  if(__fullmode && _matrix.isValid()){ 
	  pushItems();
	__currentNodeItem = __currentAttrItem;
	__currentAttrItem = NULL;

	for(uint_t it1 = 0; it1 < _matrix->getColsSize() ; it1++) { 
		for(uint_t it2 = 0; it2 < _matrix->getRowsSize() ; it2++) { 
			addAttr('['+QString::number(it1)+','+QString::number(it2)+']',_matrix->getAt(it1,it2));
		}
	}
	popItems();
  } 
}



void GeomListViewBuilder3::addAttrNode(const QString& name,
								     const SceneObjectPtr& obj,
									 const QString& type ){
//   QString value = "ptr="+(!obj.isNull()?QString("0x%1").arg(obj.toUint32(),8,16,QChar('0')):"NULL");
   QString value = "ptr="+(!obj.isNull()?QString("0x%1").arg(obj.toUint32()):"NULL");
   pushItems();
   addAttrPtr(name,value,type);
   __currentNodeItem = __currentAttrItem;
   __currentAttrItem = NULL;
   if(!obj.isNull()){
	obj->apply(*this);
   }
   popItems();
}

/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder3::process(Shape * geomShape){
   addNode(geomShape,"Shape",1);
   if(geomShape->getId() == Shape::NOID)addAttr("Id","None","");
   else addAttr("Id",geomShape->getId());
   addAttrNode("Geometry",SceneObjectPtr(geomShape->geometry),"GeometryPtr");
   addAttrNode("Appearance",SceneObjectPtr(geomShape->appearance),"AppearancePtr");
   endNode();
   return true;
}

/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  GEOM_MESH(amapSymbol,"AmapSymbol")
  addAttr("FileName",QString(amapSymbol->getFileName().c_str()),"String");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( AsymmetricHull * asymmetricHull ) {
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


bool GeomListViewBuilder3::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);
  addNode(axisRotated,"AxisRotated",3);
  addAttr("Axis",axisRotated->getAxis());
  addAttrAngle("Angle",axisRotated->getAngle());
  addAttrNode("Geometry",SceneObjectPtr(axisRotated->getGeometry()),"GeometryPtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  addNode(bezierCurve,"BezierCurve");
  addAttr("Degree",bezierCurve->getDegree());
  addAttr("Stride",bezierCurve->getStride());
  addArray("CtrlPointList",bezierCurve->getCtrlPointList(),"Vector4");
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( BezierPatch * bezierPatch ) {
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


bool GeomListViewBuilder3::process( Box * box ) {
  GEOM_ASSERT(box);
  addNode(box,"Box");
  addAttr("Size",box->getSize());
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Cone * cone ) {
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


bool GeomListViewBuilder3::process( Cylinder * cylinder ) {
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


bool GeomListViewBuilder3::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);
  addNode(elevationGrid,"ElevationGrid");
  addAttr("XSpacing",elevationGrid->getXSpacing());
  addAttr("YSpacing",elevationGrid->getYSpacing());
  addMatrix("HeightList",elevationGrid->getHeightList(),"Real");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( EulerRotated * eulerRotated ) {
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


bool GeomListViewBuilder3::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);
  addNode(extrudedHull,"ExtrudedHull");
  addAttr("CCW",extrudedHull->getCCW());
  addAttrNode("Horizontal",SceneObjectPtr(extrudedHull->getHorizontal()),"Curve2DPtr");
  addAttrNode("Vertical",SceneObjectPtr(extrudedHull->getVertical()),"Curve2DPtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  GEOM_MESH(faceSet,"FaceSet");
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Frustum * frustum ) {
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


bool GeomListViewBuilder3::process( Extrusion * extrusion ){
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

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Group * group ) {
  GEOM_ASSERT(group);
  addNode(group,"Group",3);
  addArrayNode("GeometryList",group->getGeometryList(),"Geometry");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( IFS * ifs ) {
  GEOM_ASSERT(ifs);
  addNode(ifs,"Iterative Function System",3);
  addAttr("Depth",ifs->getDepth());
  addArray("TransfoList",ifs->getTransfoList(),"Transform4");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Material * material ) {
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

bool GeomListViewBuilder3::process( ImageTexture * texture ) {
  GEOM_ASSERT(material);
  addNode(texture,"ImageTexture",2);
  addAttr("Filename",QString(texture->getFilename().c_str()),"String");
  addAttr("Ambient",texture->getAmbient());
  addAttr("Diffuse",texture->getDiffuse());
  addAttr("Specular",texture->getSpecular());
  addAttr("Emission",texture->getEmission());
  addAttr("Shininess",texture->getShininess());
  addAttr("Transparency",texture->getTransparency());

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  addNode(monoSpectral,"MonoSpectral",2);

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  addNode(multiSpectral,"MultiSpectral",2);

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  addNode(nurbsCurve,"NurbsCurve");
  addAttr("Degree",nurbsCurve->getDegree());
  addAttr("Stride",nurbsCurve->getStride());
  addArray("CtrlPointList",nurbsCurve->getCtrlPointList(),"Vector4");
  addArray("KnotList",nurbsCurve->getKnotList(),"Real");

  endNode();
  return true;
}

/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( NurbsPatch * nurbsPatch ) {
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


bool GeomListViewBuilder3::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);
  addNode(oriented,"Oriented",3);
  addAttr("Primary",oriented->getPrimary());
  addAttr("Secondary",oriented->getSecondary());
  addAttrNode("Geometry",SceneObjectPtr(oriented->getGeometry()),"GeometryPtr");

  endNode();
  return true;

}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Paraboloid * paraboloid ) {
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


bool GeomListViewBuilder3::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);

  addNode(pointSet,"PointSet");
  addArray("PointList",pointSet->getPointList(),"Vector3");
  addArray("ColorList",pointSet->getColorList(),"Color4");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);

  addNode(polyline,"Polyline");
  addArray("PointList",polyline->getPointList(),"Vector3");
  addArray("ColorList",polyline->getColorList(),"Color4");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);

  addNode(revolution,"Revolution");
  addAttrNode("Profile",SceneObjectPtr(revolution->getProfile()),"Curve2DPtr");
  addAttr("Slices",revolution->getSlices());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Swung * swung )
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


bool GeomListViewBuilder3::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  GEOM_MESH(quadSet,"QuadSet");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);

  addNode(scaled,"Scaled",3);
  addAttr("Scale",scaled->getScale());
  addAttrNode("Geometry",SceneObjectPtr(scaled->getGeometry()),"GeometryPtr");


 endNode();
 return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);

  addNode(sphere,"Sphere");
  addAttr("Radius",sphere->getRadius());
  addAttr("Slices",sphere->getSlices());
  addAttr("Stacks",sphere->getStacks());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);

  addNode(tapered,"Tapered",3);
  addAttr("BaseRadius",tapered->getBaseRadius());
  addAttr("TopRadius",tapered->getTopRadius());
  addAttrNode("Primitive",SceneObjectPtr(tapered->getPrimitive()),"PrimitivePtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Translated * translated ) {
  GEOM_ASSERT(translated);

  addNode(translated,"Translated",3);
  addAttr("Translation",translated->getTranslation());
  addAttrNode("Geometry",SceneObjectPtr(translated->getGeometry()),"GeometryPtr");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  GEOM_MESH(triangleSet,"TriangleSet");

  endNode();
  return true;
}


/* ------------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( BezierCurve2D * bezierCurve  ) {
  GEOM_ASSERT(bezierCurve);
  addNode(bezierCurve,"BezierCurve2D");
  addAttr("Degree",bezierCurve->getDegree());
  addAttr("Stride",bezierCurve->getStride());
  addArray("CtrlPointList",bezierCurve->getCtrlPointList(),"Vector3");
 
  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( Disc * disc ) {
  GEOM_ASSERT(disc);

  addNode(disc,"Disc");
  addAttr("Radius",disc->getRadius());
  addAttr("Slices",disc->getSlices());

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */


bool GeomListViewBuilder3::process( NurbsCurve2D * nurbsCurve  ) {
  GEOM_ASSERT(nurbsCurve);

  addNode(nurbsCurve,"NurbsCurve2D");
  addAttr("Degree",nurbsCurve->getDegree());
  addAttr("Stride",nurbsCurve->getStride());
  addArray("CtrlPointList",nurbsCurve->getCtrlPointList(),"Vector3");
  addArray("KnotList",nurbsCurve->getKnotList(),"Real");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */



bool GeomListViewBuilder3::process( PointSet2D *  pointSet ) {
  GEOM_ASSERT(pointSet);

  addNode(pointSet,"PointSet2D");
  addArray("PointList",pointSet->getPointList(),"Vector2");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */



bool GeomListViewBuilder3::process( Polyline2D * polyline  ) {
  GEOM_ASSERT(polyline);

  addNode(polyline,"Polyline2D");
  addArray("PointList",polyline->getPointList(),"Vector2");

  endNode();
  return true;
}


/* ----------------------------------------------------------------------- */

bool GeomListViewBuilder3::process( Text * text  ) {
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

bool GeomListViewBuilder3::process( Font * font ) {
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
