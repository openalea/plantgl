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


#include <sstream>
#include <fstream>

#include <plantgl/tool/dirnames.h>

#include "geomview_file.h"
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/tool/util_array2.h>

#include <plantgl/scenegraph/geometry/group.h>
#include <plantgl/scenegraph/geometry/bezierpatch.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/quadset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/elevationgrid.h>
#include <plantgl/scenegraph/geometry/sphere.h>
#include <plantgl/scenegraph/transformation/translated.h>

#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/appearance/material.h>

/* ----------------------------------------------------------------------- */

using namespace std;
PGL_USING_NAMESPACE
PGL_BEGIN_NAMESPACE

#define COMMENT_OUTPUT
// #define COMMENT_PARSE_OUTPUT

/* ----------------------------------------------------------------------- */

ScenePtr GeomViewBuilder::Parse(const string& filename, ostream& output)
{
    string p = get_cwd();
    string new_cwd = get_dirname(filename);     // gets the new working dir name
    if (new_cwd != "") {                       // defines the new cwd
        if (!chg_dir(new_cwd)){ 
            output << "\tchg_dir() returned an error."; 
            output << std::endl;
        }
    }
    ifstream input(filename.c_str());
    if(!input) {
        chg_dir(p);
        return ScenePtr();
    }
    GeomViewBuilder t(input,output);
    ScenePtr scene = t.parse();
    chg_dir(p);
    return scene;
}

/* ----------------------------------------------------------------------- */

bool contains(const std::string& t, const char a){
        std::string::const_iterator it = t.begin();
        while(it != t.end() && *it != a)it++;
        return (it != t.end());
}

bool contains(const std::string& t, const std::string& a){
    size_t pos = t.find(a);
    if(pos >= t.size()) return false;
    else return true;
}

/* ----------------------------------------------------------------------- */


ScenePtr GeomViewBuilder::parse() 
{ 
    ScenePtr scene(new Scene);
    while(!endFile()){
        GeometryPtr geom = parseGeometry();
        if(geom){
            scene->add(Shape3DPtr(new Shape(geom)));
        }
    }
    return scene;
}

/* <oogl-object>  ::=
 *      [ "{" ]
 *          [ "define" symbolname ]
 *          [ "appearance" appearance ]
 *          [ ["="] object-keyword ...
 *           | "<" filename
 *           | ":" symbolname ]
 *      [ "}" ]
 */
GeometryPtr GeomViewBuilder::parseGeometry() 
{
    bool bracket = false;
    uchar_t t = 0;
    string token = processToken();
    string name;
    GeometryPtr geom;
    if(token == "{"){
        bracket = true;
        token = readToken();
    }
#ifdef COMMENT_PARSE_OUTPUT
    string geomname = token;
    __output << "*** Comment : Begin of geometry :'" << geomname <<'\'' << endl;
#endif
    while(t < 6 && !endFile() && token != "}"){
#ifdef COMMENT_PARSE_OUTPUT
//  __output << "*** Comment : __token  :'" << __token <<"' - token :'" << token << '\'' << endl;
#endif
        if(token == "define"){
            name = readToken();
#ifdef COMMENT_PARSE_OUTPUT
            __output << "*** Comment : Defining '" << name.c_str() << '\''<< endl;
#endif
            if( (t & 1) == 0) 
                t = t + 1;
            else
                __output << "*** Error : define  '" << name << "'  already define (t=" << t << " - t&1=" << (t&1) << ')' << endl;
            token = processToken();     
        }
        else if(token == "appearance"){
            //          parseAppearance(token);
            if( (t & 2) == 0) t = t + 2;
            else
                __output << "*** Error : appearance already define" << endl;
            token = processToken();     
        }
        else if(token == "<"){
            if( geom ) {
                __token = token;
                break;
            }
            t = t + 4;
            string file = processToken();
#ifdef COMMENT_PARSE_OUTPUT
            __output << "*** Comment : Geometry in file '" << file.c_str() << '\''<< endl;
#endif
            if(!file.empty()){
                string p = get_cwd();
                if(pushFile(file)){
                    geom = parseGeometry();
                    if(!geom || !geom->isValid())
                        __output << "*** Error : geometry not valid" << endl;
                    popFile();
                    chg_dir(p);
                }
                else __output << "*** Error : cannot read file '" << file.c_str() << '\'' << endl;
            }
            else __output << "*** Error : Invalid filename '" << file.c_str() << '\'' << endl;
            token = processToken();     
        }
        else if(token == ":"){
            if( geom) {
                __token = token;
                break;
            }
            t = t + 4;
            string reference = readToken();
#ifdef COMMENT_PARSE_OUTPUT
            __output << "*** Comment : Reference to Geometry '" << reference.c_str() << '\''<< endl;
#endif
            geom = __defines[reference];
            token = processToken();     
        }
        else if(token == "="){
            if(geom) {
                __token = token;
                break;
            }
            t = t + 4;
            string type = readToken();
            geom = parseGeometry(type);
            token = processToken();     
        }
        else if(!token.empty()){
            if(geom) {
                __token = token;
                break;
            }
            t = t + 4;
            geom = parseGeometry(token);
            if(!geom || !geom->isValid())
                __output << "*** Error : geometry not valid" << endl;
            token = processToken();
        }
        else token = processToken();
    }
#ifdef COMMENT_PARSE_OUTPUT
    __output << "*** Comment : End of geometry " << geomname << endl;
#endif
    if(!geom)
        __output << "*** Error : geometry not define" << endl;
    if(!name.empty()){
        if(geom) __defines[name] = geom;
        geom = GeometryPtr();
    }
    return geom;
}


GeometryPtr 
GeomViewBuilder::parseGeometry(std::string& token){
    bool bracket = false;
    if(token == "{"){
        bracket = true;
        token = readToken();
    }
    GeometryPtr geom;
    GenericBuilder * b = NULL;
    if(token == "LIST"){
        GeometryArrayPtr geoms(new GeometryArray(0));
        GeometryPtr geom2;
        do{
            geom2 = parseGeometry();
            if(geom2){
                geoms->push_back(geom2);
#ifdef COMMENT_PARSE_OUTPUT
            __output << "*** Comment : Add Geometry to LIST. " << endl;
#endif
            }
        }while(!endListObject());
        if(!geoms->empty()){
            if(geoms->size() == 1)geom = geoms->getAt(0);
            else geom = GeometryPtr(new Group(geoms));
        }
    }
    else if(contains(token,"BEZ") || contains(token,"BBP")){
        b = new BezBuilder(*this);
    }
    else if (contains(token,"OFF") ){
        b = new OffBuilder(*this);
    }
    else if (contains(token,"MESH") ){
        b = new GVMeshBuilder(*this);
    }
    else if (contains(token,"QUAD") ){
        b = new QuadBuilder(*this);
    }
    else if (contains(token,"VECT") ){
        b = new VectBuilder(*this);
    }
    else if (contains(token,"SPHERE") ){
        b = new GVSphereBuilder(*this);
    }
    else if(!token.empty()){
        __output << "*** Error : Token : '" << token << "' not yet supported" << endl;
    }
    if(b != NULL){
        if(!b->setTag(token)) {
            __output << "*** Error : Invalid Token : '" << token << '\'' << endl;
        }
        else {
#ifdef COMMENT_PARSE_OUTPUT
            __output << "*** Comment : Token : '" << token << '\'' << endl;
#endif
            geom = b->parse();
            if(!geom || !geom->isValid())
                __output << "*** Error : parsed geometry not valid" << endl;
        }
        delete b;
        b = NULL;
    }
    if(bracket){
        if(!__token.empty())__token.clear();
        else if(endObject()){
            if(!__token.empty())__token.clear();
        }
    }
    return geom;
}

GeomViewBuilder::GeomViewBuilder(istream& input,ostream& output) :
    __output(output){
    __input.push(&input);
}

bool GeomViewBuilder::pushFile(const std::string& filename){
    string new_cwd = get_dirname(filename);     // gets the new working dir name
    if (new_cwd != "") {                       // defines the new cwd
        if (!chg_dir(new_cwd)){ 
            __output << "\tchg_dir() returned an error."; 
            __output << std::endl;
        }
    }
    ifstream * input = new ifstream(filename.c_str());
    if(!*input) return false;
    __input.push(input);
    return true;
}

bool GeomViewBuilder::popFile()
{
    if(__input.empty())return false;
    if(__input.size() > 1){
        istream * input = __input.top();
        delete input;
    }
    __input.pop();
    return true;
}

bool GeomViewBuilder::endFile()  
{ 
    if(!input().eof()){
        return false;
    }
    else {
        return true;
    }
}

bool GeomViewBuilder::endObject()
{
    if(!__token.empty())
        return (__token == "}" || __token == "<" || 
                __token == ":" || __token == "appearance" || 
                __token == "define");
    if(input().eof())return true;
    __token = readToken();
    if (__token == "}" || __token == "<" || 
        __token == ":" || __token == "appearance" || 
        __token == "define")return true;
    else return input().eof();
}

bool GeomViewBuilder::endListObject()
{
    if(!__token.empty())
        return (__token == "}" || __token == "appearance" || 
                __token == "define");
    if(input().eof())return true;
    __token = readToken();
    if (__token == "}" || __token == "appearance" || 
        __token == "define")  return true;
    else return input().eof();
}

/* ----------------------------------------------------------------------- */
std::string 
GeomViewBuilder::readline()
{
    char a;
    string s;
    input().read(&a,1);
    while(a != '\n' && !input().eof()){
        s.push_back(a);
        input().read(&a,1);
    }
    return s;
}

std::string 
GeomViewBuilder::processToken()
{
    string token;
    if(!__token.empty())token = __token;
    else token = readToken();
    __token.clear();
    return token;
}

std::string 
GeomViewBuilder::readToken()
{
    if(!__token.empty()){
        if(endObject())return __token;
        string t = __token;
        __token.clear();
        return t;
    }
    string delimiter = " \t\n";
    char a;
    input() >> a;
    while(a == '#'){
        if(endFile())return string();
        if(a == '#')
#ifdef COMMENT_OUTPUT
            __output << "*** Comment : ";
#endif
            while(a != '\n' && !endFile()){
                input().read(&a,1);
#ifdef COMMENT_OUTPUT
                __output << a;
#endif
            }
            input() >> a;
    }
    string token;
    while(!contains(delimiter,a) && a != '#' && a != '}' && !endFile()){
        token.push_back(a);
        input().read(&a,1);
    }
    if(a == '}') {
        __token = "}";
        if(token.empty()) token = "}";
    }
    while(a == '#'){
        if(endFile())break;
        if(a == '#')
#ifdef COMMENT_OUTPUT
            __output << "*** Comment : ";
#endif
            while(a != '\n' && !endFile()){
                input().read(&a,1);
#ifdef COMMENT_OUTPUT
                __output << a;
#endif
            }
            input() >> a;
    }
    return token;
}

float GeomViewBuilder::readFloat()
{   
    float a; 
    string t = readToken(); 
    stringstream st(t);
    st >> a;
    // a = atof(t.c_str());
    return a; 
}
int 
GeomViewBuilder::readInt()
{ 
    int a; 
    string t = readToken(); 
    stringstream st(t);
    st >> a;
//  a = atoi(t.c_str());
    return a; 
}

Vector2 
GeomViewBuilder::readVector2()
{ 
    float a = readFloat();
    float b = readFloat();
    return Vector2(a,b);
}

Vector3
GeomViewBuilder::readVector3()
{ 
    float a = readFloat();
    float b = readFloat();
    float c = readFloat();
    return Vector3(a,b,c);
}

Vector4 GeomViewBuilder::readVector4()
{ 
    float a = readFloat();
    float b = readFloat();
    float c = readFloat();
    float d = readFloat();
    return Vector4(a,b,c,d);
}

Vector3
GeomViewBuilder::readVector3(int n)
{ 
    if(n < 0 || n == 3) return readVector3();
    if(n == 0) return Vector3::ORIGIN;
    float * a = new float[n];
    for(int i = 0 ; i < n ; i++ ) a[i]= readFloat();
    Vector3 result;
    if(n == 1)result = Vector3(a[0],0,0);
    else if(n == 2)result = Vector3(a[0],a[1],0);
    else result = Vector3(a[0],a[1],a[2]);
    delete [] a;
    return result;
}

Vector4 GeomViewBuilder::readVector4(int n)
{ 
    if(n < 0 || n == 3) return readVector4();
    if(n == 0) return Vector4::ORIGIN;
    float * a = new float[n+1];
    for(int i = 0 ; i <= n ; i++ ) a[i]= readFloat();
    Vector4 result;
    if(n == 0)result = Vector4(0,0,0,a[0]);
    else if(n == 1)result = Vector4(a[0],0,0,a[1]);
    else if(n == 2)result = Vector4(a[0],a[1],0,a[2]);
    else result = Vector4(a[0],a[1],a[2],a[n]);
    delete [] a;
    return result;
}

Color4  
GeomViewBuilder::readUColor4()
{
    float a = readFloat();
    float b = readFloat();
    float c = readFloat();
    float d = readFloat();
    uchar_t e,f,g,h;
    if(a > 0.0 && a < 1.0){
        e = (uchar_t)a*255;f = (uchar_t)b*255;
        g = (uchar_t)c*255;h = (uchar_t)d*255;
    }
    else {
        e = (uchar_t)a;f = (uchar_t)b;
        g = (uchar_t)c; h = (uchar_t)d;
    }
    return Color4(e,f,g,h);
}

Color3  GeomViewBuilder::readUColor3()
{
    float a = readFloat();
    float b = readFloat();
    float c = readFloat();
    uchar_t e,f,g;
    if(a > 0.0 && a < 1.0){
        e = (uchar_t)a*255; f = (uchar_t)b*255;
        g = (uchar_t)c*255;
    }
    else {
        e = (uchar_t)a; f = (uchar_t)b;
        g = (uchar_t)c; 
    }
    return Color3(e,f,g);
}

Color4  GeomViewBuilder::readColor4()
{
    float a = readFloat();
    float b = readFloat();
    float c = readFloat();
    float d = readFloat();
    uchar_t e,f,g,h;
    e = (uchar_t)a*255;f = (uchar_t)b*255;
    g = (uchar_t)c*255;h = (uchar_t)d*255;
    return Color4(e,f,g,h);
}

Color3 GeomViewBuilder::readColor3()
{
    float a = readFloat();
    float b = readFloat();
    float c = readFloat();
    uchar_t e,f,g;
    e = (uchar_t)a*255; f = (uchar_t)b*255;
    g = (uchar_t)c*255;
    return Color3(e,f,g);
}

Index GeomViewBuilder::readIndex()
{
    int s = readInt();
    Index ind(s);
    int val;
    for(int i = 0 ; i < s ; i++){
        val = readInt();
        ind.setAt(i,val);
    }
    return ind;
}




/* ----------------------------------------------------------------------- */

BezBuilder::BezBuilder(GeomViewBuilder& b):
GenericPointBuilder(b),
__Nu(3),
__Nv(3){
}

void BezBuilder::clear(){
    GenericPointBuilder::clear();
    __Nu = 3;
    __Nv = 3;
}

/*
 *  [ST]BBP -or- [C]BEZ<Nu><Nv><Nd>[_ST]
 *          # Nu, Nv are u- and v-direction 
 *          # polynomial degrees in range 1..6
 *          # Nd = dimension: 3->3-D, 4->4-D (rational)
 *          # (The '<' and '>' do not appear in the input.)
 *          # Nu,Nv,Nd are each a single decimal digit.
 *          # BBP form implies Nu=Nv=Nd=3 so BBP = BEZ333.
 */
bool 
BezBuilder::setTag(const string& t){
    if(t == "BBP")return true;
    if(t == "STBBP"){
        __textured = true;
        return true;
    }
    string::const_iterator _it = t.begin();
    if(*_it == 'C'){ _it++; __colored = true; }
    if(string(_it,_it+3) != "BEZ")return false;
    _it = _it+3;
    __Nu = *_it - '0';
    if(__Nu < 1 || __Nu > 6) return false;
    _it++;
    __Nv = *_it - '0';
    if(__Nv < 1 || __Nv > 6) return false;
    _it++;
    int __Nd = *_it - '0';
    if(__Nd < 3 || __Nd > 4) return false;
    else __dim4 = (__Nd == 4);
    if(_it != t.end()){
        if(string(_it,t.end()) == "_ST")
            __textured = true;
    }
    return true;
}
/*
 *      # Any number of patches follow the header
 *          # (Nu+1)*(Nv+1) patch control points
 *          # each 3 or 4 floats according to header
 *  vertex(u=0,v=0)  vertex(1,0) ... vertex(Nu,0)
 *  vertex(0,1)            ... vertex(Nu,1)
 *  ...
 *  vertex(0,Nv)           ... vertex(Nu,Nv)
 *
 *          # ST texture coordinates if mentioned in header
 *  S(u=0,v=0)  T(0,0)  S(0,Nv) T(0,Nv)
 *  S(Nu,0) T(Nu,0) S(Nu,Nv) T(Nu,Nv)
 *
 *          # 4-component float (0..1) R G B A colors
 *          # for each patch corner if mentioned in header
 *  RGBA(0,0)   RGBA(0,Nv)
 *  RGBA(Nu,0)  RGBA(Nu,Nv)
 */
GeometryPtr 
BezBuilder::parse(){

#ifdef COMMENT_PARSE_OUTPUT
    __data.output() << "*** Comment : BezBuilder : " << __Nu << 'x' 
           << __Nv << " Dim4 : " <<  (__dim4?"True":"False")
           << " Textured : " << (__textured?"True":"False")
           << " Colored : " << (__colored?"True":"False")
            << endl;
#endif
    GeometryArrayPtr group = GeometryArrayPtr(new GeometryArray(0));
    Vector4 p;
    bool valid = true;
    while(!__data.endObject()){
        Point4MatrixPtr ctrls = Point4MatrixPtr(new Point4Matrix(__Nu+1,__Nv+1));
        valid = true;
        for(int i = 0 ; valid && i <= __Nu ; i++){
            if(__data.endObject() )
                { valid = false; break; }
            for(int j = 0 ; valid && j <= __Nv ; j++){
                if(__data.endObject()  )
                    { valid = false; break; }
                else {
                    if(isDim4()) p = __data.readVector4();
                    else p = Vector4(__data.readVector3(),1);
                    if(!p.isValid()){
                        __data.output() << "*** Error : BezBuilder : Point not valid " << endl;
                    }
                    else ctrls->setAt(i,j,p);
                }
            }
        }
        if(!valid) break;
        if(__textured)
            for(int t = 0 ; valid && t < 4 ; t ++){
                if(__data.endObject()  )
                { valid = false; break; }
                __data.readVector2();
            }
        if(!valid) break;
        if(__colored)
            for(int t = 0 ; valid && t < 4 ; t ++){
                if(__data.endObject())
                { valid = false; break; }
                __data.readUColor4();
            }
        group->push_back(GeometryPtr(new BezierPatch(ctrls)));
    }
    if(group->empty())return GeometryPtr();
    else if (group->size() == 1 )return group->getAt(0);
    else return GeometryPtr(new Group(group));
}

/* ----------------------------------------------------------------------- */
OffBuilder::OffBuilder(GeomViewBuilder& b):
GenericPointBuilder(b),
__n(-1){
}

void OffBuilder::clear(){
    GenericPointBuilder::clear();
    __n = -1;
}

//  [ST][C][N][4][n]OFF # Header keyword
bool 
OffBuilder::setTag(const string& t){

    string::const_iterator _it = t.begin();
    if(*_it == 'S'){ _it++;_it++; __textured = true; }
    if(*_it == 'C'){ _it++; __colored = true; }
    if(*_it == 'N'){ _it++; __normals = true; }
    if(*_it == '4'){ _it++; __dim4 = true; }
    if(*_it == 'n'){ _it++; __n = 0; }
    if(string(_it,_it+3) != "OFF")return false;
    return true;
}
/*
 * [Ndim]       # Space dimension of vertices, present only if nOFF
 * NVertices  NFaces  NEdges   # NEdges not used or checked
 * 
 * x[0]  y[0]  z[0] # Vertices, possibly with normals,
 *          # colors, and/or texture coordinates, in that order,
 *          # if the prefixes N, C, ST
 *          # are present.
 *          # If 4OFF, each vertex has 4 components,
 *          # including a final homogeneous component.
 *          # If nOFF, each vertex has Ndim components.
 *          # If 4nOFF, each vertex has Ndim+1 components.
 * ...
 * x[NVertices-1]  y[NVertices-1]  z[NVertices-1]
 * 
 *              # Faces
 *              # Nv = # vertices on this face
 *              # v[0] ... v[Nv-1]: vertex indices
 *              #       in range 0..NVertices-1
 * Nv  v[0] v[1] ... v[Nv-1]  colorspec
 * ...
 *              # colorspec continues past v[Nv-1]
 *              # to end-of-line; may be 0 to 4 numbers
 *              # nothing: default
 *              # integer: colormap index
 *              # 3 or 4 integers: RGB[A] values 0..255
 *          # 3 or 4 floats: RGB[A] values 0..1
 */
GeometryPtr 
OffBuilder::parse(){
    GeometryPtr geometry;
#ifdef COMMENT_PARSE_OUTPUT
    __data.output() << "*** Comment : OffBuilder : " 
             << " Dim4 : " <<  (__dim4?"True":"False")
             << " Textured : " << (__textured?"True":"False")
             << " Colored : " << (__colored?"True":"False")
             << " Normals : " << (__normals?"True":"False")
             << " N : " << __n
             << endl;
#endif
    if(__n != -1) __n = __data.readInt();
    int NVertices = __data.readInt();
    int NFaces = __data.readInt();
    int NEdges = __data.readInt();
    Vector3 p;
    Vector3 n;
    Color4 c;
    Vector3 t;
    bool valid = true;
    Point3ArrayPtr points(new Point3Array(NVertices));
    Point3ArrayPtr normals;
    Color4ArrayPtr colors;
    if(__normals)normals = Point3ArrayPtr(new Point3Array(NVertices));
    if(__colored)colors = Color4ArrayPtr(new Color4Array(NVertices));
    for(int i = 0 ; valid && i < NVertices ; i++){
        if(__data.endObject())
        { 
            __data.output() << "*** Error : OffBuilder : End of file after " << i << " points of " << NVertices <<"."  << endl;
            valid = false; break; 
        }
        if(__dim4){
            if(__n != -1)
                p = __data.readVector4(__n).project();
            else p = __data.readVector4().project();
        }
        else {
            if(__n != -1)
                p = __data.readVector3(__n);
            else p = __data.readVector3();
        }
        if(!p.isValid()){
            __data.output() << "*** Error : OffBuilder : Point not valid " << endl;
            points->setAt(i,Vector3::ORIGIN);
        }
        else {
            points->setAt(i,p);
        }
        
        if(__data.endObject())
        { valid = false; break; }
        if(__normals){
            n = __data.readVector3();
            if(!n.isValid()){
                __data.output() << "*** Error : OffBuilder : Normal not valid " << endl;
                normals->setAt(i,Vector3::OZ);
            }
            else normals->setAt(i,n);
        }
        if(__data.endObject())
        { valid = false; break; }
        if(__colored){
            c = __data.readColor4();
            colors->setAt(i,c);
        }
        if(__data.endObject())
        { valid = false; break; }
        if(__textured){
            if(__dim4)t = __data.readVector3();
            else t = Vector3(__data.readVector2(),0);
        }
    }
    if(__data.endObject()){ valid = false; }
    IndexArrayPtr indices(new IndexArray(NFaces));
    for(int j = 0 ; valid && j < NFaces ; j++){
        if(__data.endObject()){ 
            __data.output() << "*** Error : OffBuilder : End of file after " << j << " faces of " << NFaces <<"."  << endl;
            indices = IndexArrayPtr(new IndexArray(indices->begin(),indices->begin()+j));
            break; 
        }
        Index ind = __data.readIndex();
        // if(__colored) __data.readColor4();
        indices->setAt(j,ind);
    }
    if(valid){
        geometry = GeometryPtr(new FaceSet(points,indices,normals));
    }
    return geometry;
}

/* ----------------------------------------------------------------------- */

GVMeshBuilder::GVMeshBuilder(GeomViewBuilder& b):
GenericPointBuilder(b),
__grid(false),
__uwrapped(false),
__vwrapped(false),
__n(-1){
}

void GVMeshBuilder::clear(){
    GenericPointBuilder::clear();
    __grid = false;
    __uwrapped = false;
    __vwrapped = false;
    __n = -1;
}

// [U][C][N][Z][4][u][v][n]MESH # Key word
bool 
GVMeshBuilder::setTag(const string& t){

    string::const_iterator _it = t.begin();
    if(*_it == 'U'){ _it++; __textured = true; }
    if(*_it == 'C'){ _it++; __colored = true; }
    if(*_it == 'N'){ _it++; __normals = true; }
    if(*_it == 'Z'){ _it++; __grid = true; }
    if(*_it == '4'){ _it++; __dim4 = true; }
    if(*_it == 'u'){ _it++; __uwrapped = true; }
    if(*_it == 'v'){ _it++; __vwrapped = true; }
    if(*_it == 'n'){ _it++; __n = 0; }
    if(string(_it,_it+4) != "MESH")return false;
    if(__dim4 && __grid){
        __data.output() << "*** Error : GVMeshBuilder : Incompatible option 4 and Z for MESH" << endl;
        return false;
    }
    return true;
}

/*
 * [Ndim]           # Space dimension, present only if nMESH$
 * Nu Nv            # Mesh grid dimensions$
 *                    # Nu*Nv vertices, in format specified$
 *                    # by initial key word$
 * vertex(u=0,v=0)  vertex(1,0)  ... vertex(Nu-1,0)$
 * vertex(0,1) ...    vertex(Nu-1,1)$
 * ...$
 * vertex(0,Nv-1) ... vertex(Nu-1,Nv-1)$
*/
#define IND(u,v) (v)*Nu+u

GeometryPtr 
GVMeshBuilder::parse(){
    GeometryPtr geometry;
#ifdef COMMENT_PARSE_OUTPUT
    __data.output() << "*** Comment : GVMeshBuilder : " 
             << " Dim4 : " <<  (__dim4?"True":"False")
             << " Textured : " << (__textured?"True":"False")
             << " Colored : " << (__colored?"True":"False")
             << " Normals : " << (__normals?"True":"False")
             << " U wrapped : " << (__uwrapped?"True":"False")
             << " V wrapped : " << (__vwrapped?"True":"False")
             << " N : " << __n
             << endl;
#endif
    if(__n != -1) __n = __data.readInt();
    int Nu = __data.readInt();
    int Nv = __data.readInt();
    Vector3 p;
    Vector3 n;
    Color4 c;
    Vector3 t;
    bool valid = true;
    Point3ArrayPtr points;
    RealArray2Ptr grid;
    if(!__grid)
        points = Point3ArrayPtr(new Point3Array(Nu*Nv));
    else 
        grid = RealArray2Ptr(new RealArray2(Nu,Nv));
    Point3ArrayPtr normals;
    if(__normals)normals = Point3ArrayPtr(new Point3Array(Nu*Nv));
    for(int i = 0 ; valid && i < Nu*Nv ; i++){
        if(__data.endObject())
            { valid = false; break; }
        if(__grid){
            real_t a = __data.readFloat();
            int col = i/Nu;
            grid->setAt(i-col*Nu,col,a);
        }
        else {
            if(__dim4){
                Vector4 p2;
                if(__n != -1){
                    p2 = __data.readVector4(__n);
                }
                else p2 = __data.readVector4();
                p = Vector3(p2.x(),p2.y(),p2.z());
            }
            else {
                if(__n != -1)
                    p = __data.readVector3(__n);
                else p = __data.readVector3();
            }
            if(!p.isValid()){
                __data.output() << "*** Error : GVMeshBuilder : Point not valid " << endl;
                points->setAt(i,Vector3::ORIGIN);
            }
            else points->setAt(i,p);
        }
        if(__data.endObject())
            { valid = false; break; }
        if(__normals){
            n = __data.readVector3();
            if(!n.isValid()){
                __data.output() << "*** Error : GVMeshBuilder : Normal not valid " << endl;
                normals->setAt(i,Vector3::OZ);
            }
            else normals->setAt(i,n);
        }
        if(__data.endObject())
            { valid = false; break; }
        if(__colored){
            c = __data.readColor4();
        }
        if(__data.endObject())
            { valid = false; break; }
        if(__textured){
            t = __data.readVector3();
        }
    }
    if(valid){
        if(__grid){
            geometry = GeometryPtr(new ElevationGrid(grid));
        }
        else {
            int NFaces = (Nu-1)*(Nv-1)*2;
            if(__uwrapped)
                NFaces += (Nv-1)*2;
            if(__vwrapped)
                NFaces += (Nu-1)*2;
            Index3ArrayPtr indices(new Index3Array(NFaces));
            int i = 0;
            for(int j = 0 ; j < (Nu-1) ; j++){
                for(int k = 0 ; k < (Nv-1) ; k++){
                    indices->setAt(i,Index3(IND(j,k),IND(j+1,k),IND(j+1,k+1)));i++;
                    indices->setAt(i,Index3(IND(j,k),IND(j+1,k+1),IND(j,k+1)));i++;
                }
            }
            if(__uwrapped){
                for(int k = 0 ; k < (Nv-1) ; k++){
                    indices->setAt(i,Index3(IND(Nu-1,k),IND(0,k),IND(0,k+1)));i++;
                    indices->setAt(i,Index3(IND(Nu-1,k),IND(0,k+1),IND(Nu-1,k+1)));i++;
                }
            }
            if(__vwrapped){
                for(int j = 0 ; j < (Nu-1) ; j++){
                    indices->setAt(i,Index3(IND(j,Nv-1),IND(j+1,Nv-1),IND(j+1,0)));i++;
                    indices->setAt(i,Index3(IND(j,Nv-1),IND(j+1,0),IND(j,0)));i++;
                }
            }
            
            if(!__normals)
                geometry = GeometryPtr(new TriangleSet(points,indices));
            else 
                geometry = GeometryPtr(new TriangleSet(points,indices,normals));
        }
    }
    return geometry;
}

#undef IND

/* ----------------------------------------------------------------------- */

QuadBuilder::QuadBuilder(GeomViewBuilder& b):
GenericPointBuilder(b){
}

// [C][N][4]QUAD  -or-  [C][N][4]POLY          # Key word
bool 
QuadBuilder::setTag(const string& t){

    string::const_iterator _it = t.begin();
    if(*_it == 'C'){ _it++; __colored = true; }
    if(*_it == 'N'){ _it++; __normals = true; }
    if(*_it == '4'){ _it++; __dim4 = true; }
    if(string(_it,_it+4) != "QUAD")return false;
    return true;
}

/*
 *   vertex  vertex  vertex  vertex  # 4*N vertices for some N
 *   vertex  vertex  vertex  vertex
 */
GeometryPtr 
QuadBuilder::parse(){
#ifdef COMMENT_PARSE_OUTPUT
    __data.output() << "*** Comment : QuadBuilder : " 
        << " Dim4 : " <<  (__dim4?"True":"False")
        << ", Colored : " << (__colored?"True":"False")
        << ", Normals : " << (__normals?"True":"False")
        << endl;
#endif
    Vector3 p;
    Vector3 n;
    Color4 c;
    bool valid = true;
    Point3ArrayPtr points(new Point3Array);
    Point3ArrayPtr normals;
    if(__normals)normals = Point3ArrayPtr(new Point3Array);
    while(valid && !__data.endObject()){
        if(__dim4){
            Vector4 p2 = __data.readVector4();
            p = Vector3(p2.x(),p2.y(),p2.z());
        }
        else p = __data.readVector3();
        if(p.isValid())points->push_back(p);
        if(__normals){
            if(__data.endObject()){ 
                __data.output() << "*** Error : QuadBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
                valid = false; break; }
            n = __data.readVector3();
            if(n.isValid())normals->push_back(n);
        }
        if(__colored){
            if(__data.endObject()){ 
                __data.output() << "*** Error : QuadBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
                valid = false; break; }
            c = __data.readColor4();
        }
    }
    if(valid){
        uint_t nbfaces = points->size()/4;
        if(nbfaces*4 != points->size()){
            __data.output() << "*** Error : QUAD: Number of vertices not multiple of 4 : " << points->size() << endl;
        }
        Index4ArrayPtr indices(new Index4Array(nbfaces));
        for(uint_t it = 0; it < nbfaces; it++)
            indices->setAt(it,Index4(4*it,4*it+1,4*it+2,4*it+3));
        if(!__normals)
            return GeometryPtr(new QuadSet(points,indices));
        else 
            return GeometryPtr(new QuadSet(points,indices,normals));
    }
    else return GeometryPtr();
}


/* ----------------------------------------------------------------------- */

VectBuilder::VectBuilder(GeomViewBuilder& b):
GenericBuilder(b),
__dim4(false){
}

// [4]VECT
bool 
VectBuilder::setTag(const string& t){

    string::const_iterator _it = t.begin();
    if(*_it == '4'){ _it++; __dim4 = true; }
    if(string(_it,_it+4) != "VECT")return false;
    return true;
}
/*
 * NPolylines  NVertices  NColors$
 * $
 * Nv[0] ... Nv[NPolylines-1]     # number of vertices$
 *                                            # in each polyline$
 * $
 * Nc[0] ... Nc[NPolylines-1]     # number of colors supplied$
 *                                            # in each polyline$
 * $
 * Vert[0] ... Vert[NVertices-1]  # All the vertices$
 *                                            # (3*NVertices floats)$
 * $
 * Color[0] ... Color[NColors-1]  # All the colors$
 *                                            # (4*NColors floats, RGBA)$
*/
GeometryPtr 
VectBuilder::parse(){
    GeometryArrayPtr group(new GeometryArray(0));
#ifdef COMMENT_PARSE_OUTPUT
    __data.output() << "*** Comment : VectBuilder : " 
        << " Dim4 : " <<  (__dim4?"True":"False")
        << endl;
#endif
    int NPolylines = __data.readInt();
    if(__data.endObject()){ 
        __data.output() << "*** Error : VectBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
        return GeometryPtr(); }
    int NVertices = __data.readInt();
    if(__data.endObject()){ 
        __data.output() << "*** Error : VectBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
        return GeometryPtr(); }
    int NColors = __data.readInt();
    if(__data.endObject()){ 
        __data.output() << "*** Error : VectBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
        return GeometryPtr(); }
    bool valid = true;
    int * NPointsP = new int [NPolylines];
    int * NColorsP = new int [NPolylines];
    { for(int i = 0; valid && i < NPolylines ; i++){
        if(__data.endObject())
        { valid = false; 
          __data.output() << "*** Error : VectBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
          break; }
        NPointsP[i] = __data.readInt(); }
    }
    { for(int i = 0; valid && i < NPolylines ; i++){
        if(__data.endObject())
        { valid = false; 
          __data.output() << "*** Error : VectBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
          break; }
        NColorsP[i] = __data.readInt(); }
    }
    Vector3 p;
    Color4 c;
    Point3ArrayPtr points(new Point3Array);
    for(int i = 0 ; valid && i < NVertices ; i++){
        if(__data.endObject())
        { valid = false; 
          __data.output() << "*** Error : VectBuilder[" << __LINE__ << "] : Invalid end of object." << endl;
          break; }
        if(__dim4){
            Vector4 p2 = __data.readVector4();
            p = Vector3(p2.x(),p2.y(),p2.z());
        }
        else p = __data.readVector3();
        if(p.isValid())points->push_back(p);
        else {
            __data.output() << "*** Error : VectBuilder[" << __LINE__ << "] : Invalid Point " << endl;
            points->push_back(Vector3::ORIGIN);
        }
    }
    if(valid){
        for(int j = 0 ; valid && j < NColors ; j++){
            c = __data.readColor4();
        }
    }
    if(valid){
        uint_t sum = 0;
        { for(int k = 0 ; valid && k < NPolylines ; k++)
            sum += (NPointsP[k]<0?-NPointsP[k]:NPointsP[k]); }
        if(sum != points->size()){
            __data.output() << "*** Error : VectBuilder : Number of points does not correspond " 
                << sum << " -- " << points->size()
                << endl;
            return GeometryPtr();
        }
        Point3Array::iterator it = points->begin();
        for(int k = 0 ; it != points->end() && k < NPolylines ; k++){
            bool closed = (NPointsP[k] < 0);
            int nbp = (NPointsP[k]<0?-NPointsP[k]:NPointsP[k]);
            if(nbp == 1){
                group->push_back(GeometryPtr(new PointSet(Point3ArrayPtr(new Point3Array(it,it+1)))));
                it++;
            }
            else if(nbp > 1){
                Point3ArrayPtr lpoints(new Point3Array(it,it+nbp));
                if(closed)lpoints->push_back(*it);
                group->push_back(GeometryPtr(new Polyline(lpoints)));
                it = it+nbp;
            }
        }
        if(group->empty())return GeometryPtr();
        else if (group->size() == 1 )return group->getAt(0);
        else return GeometryPtr(new Group(group));
    }
    return GeometryPtr();
}


/* ----------------------------------------------------------------------- */

GVSphereBuilder::GVSphereBuilder(GeomViewBuilder& b):
GenericBuilder(b){
}

// SPHERE
bool 
GVSphereBuilder::setTag(const string& t){
    if(t == "SPHERE") return true;
    return false;
}
/* 
 * Radius
 * Xcenter Ycenter Zcenter
 */
GeometryPtr 
GVSphereBuilder::parse(){
#ifdef COMMENT_PARSE_OUTPUT
    __data.output() << "*** Comment : GVSphereBuilder" << endl;
#endif
    real_t radius = __data.readFloat();
    Vector3 center = __data.readVector3();
    if(center == Vector3::ORIGIN)
    return GeometryPtr(new Sphere(radius));
    else return GeometryPtr(new Translated(center,GeometryPtr(new Sphere(radius))));
}


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE
