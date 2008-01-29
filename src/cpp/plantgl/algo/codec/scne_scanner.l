%{ //-*-c++-*-
/* -*-c++-*-
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


// #define LEXDEBUG

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>

#include <plantgl/scenegraph/container/pointmatrix.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#include <list>
#include <fstream>
#include "codec_config.h"
#ifdef STL_EXTENSION
	#include <sstream>
#else
	#include <strstream>
#endif

#include "scne_parser.h"

#ifdef BISON_HPP
#include "scne_parser.hpp"
#else
#include "scne_parser.cpp.h"
#endif

#include "scne_scanner.h"
#include <plantgl/tool/gscanner_imp.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/util_hashmap.h>

#include "scne_binaryparser.h"

using namespace std;
/* ----------------------------------------------------------------------- */


extern "C" {
  int scne_yywrap() {
    return 1; // if 0 then it is assumed that the scanning continues
  }
// redefined since lex uses the option -P and the default
// yywrap is nolonger provided by the lex library -lfl
int yywrap();

}


/* ----------------------------------------------------------------------- */
class geommacro {
        public :
        std::string __name;
        std::vector<std::string> __arg;
        std::string value;

        bool parseArg(const std::string& text,
                      std::vector<std::string>& args,
                      std::string::const_iterator b_ ,
                      std::string::const_iterator e) {
                        bool goon = false;
                        std::string::const_iterator b = b_;
                    std::string::const_iterator _nb;
                        do{
                         goon = false;
                         while(b != e && (*b == ' ' || *b == '\t'|| *b == '\n') )b++;
                         if(b == e)return true;
                         _nb = b;
                         std::string arg;
                         if(*b == '"'){
                          _nb++;b++;
                          while (b != e && *b != '"'  ||
                                   (*b == '"'  && b != b_ && *(b-1) == '\\'))b++;
                         }
                         else{
                          while(b != e && *b != ' ' &&
                                   *b != '\t' && *b != '\n' &&
                                   ((*b != ',')  ||
                                   (*b == ','  && b != b_ && *(b-1) == '\\')) )b++;
                         }
                         if(b != _nb ){
                                arg = std::string(_nb,b);
                                size_t pos;
                                while((pos = arg.find("\\,"))!=std::string::npos)arg.replace(pos,2,",");
                                while((pos = arg.find("\\\""))!=std::string::npos)arg.replace(pos,2,"\"");
                                args.push_back(arg);
                         }
                         else if(*b == '"' && *(b-1) == '"')args.push_back("");
                         else return false;
                         if(b != e && *b == '"')b++;
                         while(b != e && ( *b == ' ' ||
                                  *b == '\t' || *b == '\n' || *b == ',' ) ){
                                  if(*b == ',')goon =true;
                                  b++;
                         }
                        }while (goon);
                        return true;
        }

        bool parseValue(const std::string& text, std::string::const_iterator i){
                size_t p = distance(text.begin(),i);
                size_t p1;
                if((p1 = text.find('{',p)) != std::string::npos){
                        size_t p2 = 0;
                        if((p2 = text.find_last_of('}',p)) == std::string::npos)p2 = text.size();
                        value = std::string(text.begin()+(p1+1),text.begin()+p2);
                }
                else value = std::string(i,text.end());
                return true;
        }

        bool build(const std::string& text){
                std::string::const_iterator _b = text.begin();
                while(_b != text.end() && (*_b == ' ' || *_b == '\t'|| *_b == '\n') )_b++;
                if(_b == text.end())return false;
                std::string::const_iterator _nb = _b;
                while(_b != text.end() && *_b != ' ' && *_b != '\n' && *_b != '\t' && *_b != '(')_b++;
                __name = std::string(_nb,_b);
                if(_b == text.end())return false;
                while(_b != text.end() && (*_b == ' ' || *_b == '\t'|| *_b == '\n') )_b++;
                if(_b == text.end())return false;
                if(*_b != '(')parseValue(text,_b);
                else {
                        _b++;
                        size_t pos = text.find(')');
                        if(pos == std::string::npos)return false;
                        _nb = text.begin()+pos;
                        if(!parseArg(text,__arg,_b,_nb))return false;
                        if(_nb == text.end())return false;
                        parseValue(text,_nb+1);
                }
                return true;
        }

        void print(std::ostream& stream) const {
       stream << "Macro Name> '" << __name << '\'' << std::endl;
       stream << "Macro Args> ";
           for(std::vector<std::string>::const_iterator _i = __arg.begin(); _i != __arg.end(); _i++)
                stream << " '" << *_i << '\'';
           stream << std::endl;
       stream << "Macro Value> '" << value << '\'' << std::endl;
        }

        bool hasArg() const { return !__arg.empty(); }
        int argSize() const { return __arg.size(); }

        std::string apply(std::ostream& error,std::string args_ = std::string()){
            std::vector<std::string> args;
                parseArg(args_,args,args_.begin(),args_.end());
                if(args.size() != __arg.size()){
                        error << "Error : Wrong number of argument for macro " << __name
                              << " ( " << args.size() << " instead of " <<__arg.size() << " )" << std::endl;
                        error << "Error : Found ";
                        for(size_t i = 0; i < std::min(__arg.size(),args.size());i++)
                                error << " [" << __arg[i] << '=' << args[i] << ']';
                        error << std::endl;
                        return std::string();
                }
                std::string result(value.begin(),value.end());
                size_t pos = 0;
                size_t l = 0;
                for(size_t i = 0; i < __arg.size();i++){
                    pos = 0;
                    l = 0;
                        while(l < result.size() && (pos = result.find(__arg[i],l))!=std::string::npos){
                                result.replace(pos,__arg[i].size(),args[i]);
                                l = pos + args[i].size();
                        }
                }
                return result;
        }
};

static hash_map_string<geommacro> MacroTable;
static std::string current_macro;
static std::string current_macro_args;
static int parant = 0;

%}

/*
   %option c++ is equivalent to compiling with flex++.
   %option yyclass only applies when generating a C++ scanner (-+
   option).  It informs flex that you have derived foo as a subclass
   of yyFlexLexer, so flex will place your actions in the member func­
   tion foo::yylex() instead of yyFlexLexer::yylex().  It also
   generates a yyFlexLexer::yylex() member function that emits a
   run-time error (by invoking yyFlexLexer::LexerError()) if called.

*/

%option c++
%option yyclass="GENERIC_LEXER"
%option yylineno
%option prefix="scne_yy"



/* lex states */

%x include
%x define
%x undef
%x ifdef
%x comment
%x echo
%x defarg
%x definarg

/*
R       [+-]?({D}"."{D}?)|({D}?"."{D})|({D}("."{D}?)?[Ee][+-]?{D})|("."{D}[Ee][+-]?{D})
*/

NAME    [a-zA-Z_]([0-9]|[a-zA-Z_])*
B       [ ]+
T       [\t]+
dig     [0-9]
D       {dig}+
num1    {dig}+\.?([eE][-+]?{dig}+)?
num2    {dig}*\.{dig}+([eE][-+]?{dig}+)?
R       {num1}|{num2}
I       {D}
FILE    \"[^\"\t\n]*\"

%%

 /* yyterminate() could be used directly within the lexer if
    used in stand alone mode instead of return TokQUIT */
"(#"              {Trace("comment");
                   BEGIN(comment); /* (# commentaire a la C++ #) */
                  }

"#".*$            {Trace("comment");/* # commentaire de ligne a la C++ */
                  }

:[iI]nclude       {TRACE; BEGIN(include);}
:[dD]ef           {TRACE; BEGIN(define);}
:[dD]efine        {TRACE; BEGIN(define);}
:[uU]ndef         {TRACE; BEGIN(undef);}
:[Ee]cho          {TRACE; BEGIN(echo);}

{B}               {TRACE;}
{T}               {_columno += strlen(YYText());TRACE;}

"?"               {TRACE; return '?';}
","               {TRACE; return ',';}
";"               {TRACE; return ';';}
"{"               {TRACE; return '{';}
"}"               {TRACE; return '}';}
"["               {TRACE; return '[';}
"]"               {TRACE; return ']';}
"<"               {TRACE; return '<';}
">"               {TRACE; return '>';}
"("               {TRACE; return '(';}
")"               {TRACE; return ')';}
"\""              {TRACE; return '"';}
\-                {TRACE; return TokMINUS;}
\+                {TRACE; return TokPLUS;}
\*                {TRACE; return TokTIMES;}
"/"               {TRACE; return TokSLASH;}

Shape             {TRACE; return TokShape;}
Inline            {TRACE; return TokInline;}

AmapSymbol        {TRACE; return TokAmapSymbol;}
AsymmetricHull    {TRACE; return TokAsymmetricHull;}
AxisRotated       {TRACE; return TokAxisRotated;}
BezierCurve       {TRACE; return TokBezierCurve;}
BezierCurve2D     {TRACE; return TokBezierCurve2D;}
BezierPatch       {TRACE; return TokBezierPatch;}
Box               {TRACE; return TokBox; }
Cone              {TRACE; return TokCone;}
Cylinder          {TRACE; return TokCylinder;}
Disc              {TRACE; return TokDisc;}
ElevationGrid     {TRACE; return TokElevationGrid;}
EulerRotated      {TRACE; return TokEulerRotated;}
ExtrudedHull      {TRACE; return TokExtrudedHull;}
Extrusion         {TRACE; return TokExtrusion;}
FaceSet           {TRACE; return TokFaceSet;}
Frustum           {TRACE; return TokFrustum;}
Group             {TRACE; return TokGroup;}
IFS               {TRACE; return TokIFS;}
NurbsCurve        {TRACE; return TokNurbsCurve;}
NurbsCurve2D      {TRACE; return TokNurbsCurve2D;}
NurbsPatch        {TRACE; return TokNurbsPatch;}
Oriented          {TRACE; return TokOriented;}
Paraboloid        {TRACE; return TokParaboloid;}
PointSet          {TRACE; return TokPointSet;}
PointSet2D        {TRACE; return TokPointSet2D;}
Polyline          {TRACE; return TokPolyline;}
Polyline2D        {TRACE; return TokPolyline2D;}
QuadSet           {TRACE; return TokQuadSet;}
Revolution        {TRACE; return TokRevolution;}
Swung             {TRACE; return TokSwung;}
Scaled            {TRACE; return TokScaled;}
Sphere            {TRACE; return TokSphere;}
Tapered           {TRACE; return TokTapered;}
Translated        {TRACE; return TokTranslated;}
TriangleSet       {TRACE; return TokTriangleSet;}

Text			  {TRACE; return TokText;}
Font			  {TRACE; return TokFont;}

Material          {TRACE; return TokMaterial;}
MonoSpectral      {TRACE; return TokMonoSpectral;}
MultiSpectral     {TRACE; return TokMultiSpectral;}
Texture           {TRACE; return TokImageTexture;}
ImageTexture      {TRACE; return TokImageTexture;}

Appearance        {TRACE; return TokAppearance;}
Geometry          {TRACE; return TokGeometry;}
Scene             {TRACE; return TokScene;}

Angle             {TRACE; return TokAngle;}
AngleList         {TRACE; return TokAngleList;}
Axis              {TRACE; return TokAxis;}
Azimuth           {TRACE; return TokAzimuth;}
BaseRadius        {TRACE; return TokBaseRadius;}
BBoxCenter        {TRACE; return TokBBoxCenter;}
BBoxSize          {TRACE; return TokBBoxSize;}
Bold			  {TRACE; return TokBold;}
Bottom            {TRACE; return TokBottom;}
BottomShape       {TRACE; return TokBottomShape;}
CCW               {TRACE; return TokCCW;}
CrossSection      {TRACE; return TokCrossSection;}
CtrlPointList     {TRACE; return TokCtrlPointList;}
CtrlPointMatrix   {TRACE; return TokCtrlPointMatrix;}
ColorList         {TRACE; return TokColorList;}
ColorIndexList    {TRACE; return TokColorIndexList;}
ColorPerVertex    {TRACE; return TokColorPerVertex;}
Degree            {TRACE; return TokDegree;}
Depth             {TRACE; return TokDepth;}
GeometryList      {TRACE; return TokGeometryList;}
Elevation         {TRACE; return TokElevation;}
Family	          {TRACE; return TokFamily;}
File[Nn]ame       {TRACE; return TokFileName;}
FontStyle         {TRACE; return TokFontStyle;}
Height            {TRACE; return TokHeight;}
HeightList        {TRACE; return TokHeightList;}
Horizontal        {TRACE; return TokHorizontal;}
Id                {TRACE; return TokId;}
IndexList         {TRACE; return TokIndexList;}
Italic			  {TRACE; return TokItalic;}
KnotList          {TRACE; return TokKnotList;}
Mipmaping         {TRACE; return TokMipmaping;}
NegXHeight        {TRACE; return TokNegXHeight;}
NegXRadius        {TRACE; return TokNegXRadius;}
NegYHeight        {TRACE; return TokNegYHeight;}
NegYRadius        {TRACE; return TokNegYRadius;}
NormalList        {TRACE; return TokNormalList;}
NormalIndexList   {TRACE; return TokNormalIndexList;}
NormalPerVertex   {TRACE; return TokNormalPerVertex;}
Orientation       {TRACE; return TokOrientation;}
ParentId          {TRACE; return TokParentId;}
PointList         {TRACE; return TokPointList;}
Position		  {TRACE; return TokPosition;}
PosXHeight        {TRACE; return TokPosXHeight;}
PosXRadius        {TRACE; return TokPosXRadius;}
PosYHeight        {TRACE; return TokPosYHeight;}
PosYRadius        {TRACE; return TokPosYRadius;}
Primary           {TRACE; return TokPrimary;}
Primitive         {TRACE; return TokPrimitive;}
Profile			  {TRACE; return TokProfile;}
ProfileList       {TRACE; return TokProfileList;}
Radius            {TRACE; return TokRadius;}
Roll              {TRACE; return TokRoll;}
Scale             {TRACE; return TokScale;}
ScreenCoordinates {TRACE; return TokScreenCoordinates;}
Secondary         {TRACE; return TokSecondary;}
ShapeFactor       {TRACE; return TokShapeFactor;}
Size              {TRACE; return TokSize;}
Skeleton          {TRACE; return TokSkeleton;}
Slices            {TRACE; return TokSlices;}
Stacks            {TRACE; return TokStacks;}
Stride            {TRACE; return TokStride;}
String            {TRACE; return TokString;}
Solid             {TRACE; return TokSolid;}
Taper             {TRACE; return TokTaper;}
Top               {TRACE; return TokTop;}
TopShape          {TRACE; return TokTopShape;}
TopRadius         {TRACE; return TokTopRadius;}
Transfo           {TRACE; return TokTransfo;}
TransfoList       {TRACE; return TokTransfoList;}
Translation       {TRACE; return TokTranslation;}
TexCoordList      {TRACE; return TokTexCoordList;}
TexCoordIndexList {TRACE; return TokTexCoordIndexList;}
UDegree           {TRACE; return TokUDegree;}
UKnotList         {TRACE; return TokUKnotList;}
UStride           {TRACE; return TokUStride;}
VDegree           {TRACE; return TokVDegree;}
Vertical          {TRACE; return TokVertical;}
VKnotList         {TRACE; return TokVKnotList;}
VStride           {TRACE; return TokVStride;}
XSpacing          {TRACE; return TokXSpacing;}
YSpacing          {TRACE; return TokYSpacing;}

AxisRotation      {TRACE; return TokAxisRotation;}
EulerRotation     {TRACE; return TokEulerRotation;}
BaseOrientation   {TRACE; return TokBaseOrientation;}

Ambient           {TRACE; return TokAmbient;}
Diffuse           {TRACE; return TokDiffuse;}
Emission          {TRACE; return TokEmission;}
Filter            {TRACE; return TokFilter; }
Reflectance       {TRACE; return TokReflectance;}
Shininess         {TRACE; return TokShininess;}
Specular          {TRACE; return TokSpecular;}
Transmittance     {TRACE; return TokTransmittance;}
Transparency      {TRACE; return TokTransparency;}

Black             {TRACE; return TokBlack;}
White             {TRACE; return TokWhite;}
Red               {TRACE; return TokRed;}
Green             {TRACE; return TokGreen;}
Blue              {TRACE; return TokBlue;}
Cyan              {TRACE; return TokCyan;}
Magenta           {TRACE; return TokMagenta;}
Yellow            {TRACE; return TokYellow;}

True              {TRACE; VAL->bool_t = new bool(true); return TokBool;}
False             {TRACE; VAL->bool_t = new bool(false); return TokBool;}

:[Qq]uit          {TRACE; return TokQuit;}
:[Ll]ist          {TRACE; return TokList;}

Pi                {TRACE; return TokPi;}
OX                {TRACE; return TokOX;}
OY                {TRACE; return TokOY;}
OZ                {TRACE; return TokOZ;}
:[Pp]ow           {TRACE; return TokPow;}
:[Cc]os           {TRACE; return TokCos;}
:[Ss]in           {TRACE; return TokSin;}
:[Tt]an           {TRACE; return TokTan;}
:[Aa]cos          {TRACE; return TokACos;}
:[Aa]sin          {TRACE; return TokASin;}
:[Aa]tan          {TRACE; return TokATan;}
:[Dd]ot           {TRACE; return TokDot;}
:[Aa]ngle         {TRACE; return TokAngleF;}
:[Cc]ross         {TRACE; return TokCross;}
:[Nn]orm          {TRACE; return TokNorm;}
:[Dd]irection     {TRACE; return TokDirection;}

{NAME}            {TRACE;
                   std::string n(YYText());
                   hash_map_string<geommacro>::iterator i = MacroTable.find(n);
                   if(i == MacroTable.end()){
                     VAL->string_t = new std::string(n);
                     return TokName;
                   }
                                   else {
                                    if(!i->second.hasArg()){
                                                std::string res = i->second.apply(lostream);
                                                if(!res.empty()){
                                                std::stringstream * fs = new std::stringstream(res);
                         if (!*fs) {
                           delete fs;
                           lostream << "*** SCANNER : error: cannot process macro: " << n << std::endl;
                         }
                         else {
                           if (!pushMacroStream(*fs, n.c_str())) {
                              lostream << "error: Includes nested too deeply (max depth = "
                                      << MAX_INCLUDE_DEPTH << ")" << std::endl;
                           }
                                 }
                                                 }
                                          }
                                          else {
                                                current_macro = n;
                                                BEGIN(defarg);
                       //
                                          }
                                        }
                  }
<defarg>[\t ]+      {_columno += strlen(YYText());TRACE;}
<defarg>\n          {_columno = 1;TRACE;}
<defarg>"("                     {TRACE;parant = 0; BEGIN(definarg);}
<defarg>^"("        {TRACE;
                                         lostream << "error: Call to macro " << current_macro
                                                          << " without argument" << std::endl;BEGIN(INITIAL);}
<definarg>"("           {TRACE;
                                         parant++;
                                         current_macro_args += std::string(YYText());}
<definarg>")"       {TRACE;
                                          if(parant>0){
                                                parant--;
                                                current_macro_args += std::string(YYText());
                                          }
                                          else {
//                                     lostream << "Instanciation of " << current_macro << " with (" << current_macro_args << ')' << std::endl;
                                       hash_map_string<geommacro>::iterator i = MacroTable.find(current_macro);
                                           std::string res = i->second.apply(lostream,current_macro_args);
//                                         lostream << "Instanciation = " << res << std::endl;
                                           if(!res.empty()){
                                                std::stringstream * fs = new std::stringstream(res);
                        if (!*fs) {
                           delete fs;
                           lostream << "*** SCANNER : error: cannot process macro: " << current_macro << std::endl;
                        }
                        else {
                          if (!pushMacroStream(*fs,current_macro.c_str())) {
                                                  printf("test\n");
                             lostream << "error: Includes nested too deeply (max depth = "
                                      << MAX_INCLUDE_DEPTH << ")" << std::endl;
                           }
                                 }
                                           }
                                           current_macro_args= "";
                                           current_macro= "";
                                           BEGIN(INITIAL);
                                          }
                                        }
<definarg>[^()\n]+  {TRACE;current_macro_args += std::string(YYText());}
<definarg>\n        {TRACE;_columno=1;}

{FILE}            {TRACE; std::string _str(YYText());
                   VAL->string_t = new std::string(_str.begin()+1,_str.end()-1);
                   return TokFile;
                  }
{I}               {TRACE;
                   VAL->int32_o = new int_t(atoi(YYText()));
                   return TokInt;
                  }
{R}               {TRACE; VAL->real_o = new real_t(atof(YYText()));
                   return TokReal;
                  }


<undef>[\t ]+      {_columno += strlen(YYText());TRACE;}
<undef>[^\n]+      {TRACE; // reading the string
                                   geommacro m;
                                   m.build(YYText());
                                   MacroTable.erase(m.__name);
                                   BEGIN(INITIAL);
                  }
<undef>\n        {TRACE;_columno=1;}


<define>[\t ]*\n  {TRACE;_columno=1;
                   current_macro= "";
                   BEGIN(INITIAL);}

<define>[^\n]+\n  { TRACE; // reading the string
                                    bool endm = true;
                                    std::string t(YYText());
                                        if(*(t.end()-2) == '\\'){
                                                t = std::string(t.begin(),t.end()-2);
                                                t += '\n';
                                                endm = false;
                                        }
                                    if(current_macro.empty()){
                                           geommacro m;
                                           m.build(t);
                                           MacroTable[m.__name] = m;
                                           current_macro = m.__name;
                                        }
                                        else {
                                                MacroTable[current_macro].value.append(t);
                                        }
                                        if(endm) {
                                                current_macro= "";
                                                BEGIN(INITIAL);
                                        }
                  }


:[pP]rint[Dd]ef   {TRACE; for(hash_map_string<geommacro>::const_iterator _it = MacroTable.begin();
                                                          _it != MacroTable.end(); _it++)_it->second.print(lostream);}

<echo>{T}+     {_columno += strlen(YYText());TRACE;}
<echo>[^\t\n]+ {TRACE; // reading the string
                                lostream << "> " << YYText() << std::endl;
                BEGIN(INITIAL);
                }
<include>{B}+
<include>{T}+     {_columno += strlen(YYText());TRACE;}
<include>[^\t\n]+ {TRACE; // reading the filename
                   std::string _str(YYText());
                   std::string::iterator _beg = _str.begin(); while(_beg!=(_str.end()-1)&&*_beg==' ')_beg++;
                   std::string::iterator _end = _str.end()-1; while(_end!=_beg&&*_end==' ')_end--;
                   if(_end==_beg)BEGIN(INITIAL);
                   else {
                       if(*_beg=='\"')_beg++;
                       if(*_end=='\"')_end--;
                       _str= std::string(_beg,_end+1);
                       if(!BinaryParser::isAGeomBinaryFile(_str)){
                           std::istream* fs = new std::ifstream(_str.c_str());
                           if (!*fs) {
                               delete fs;
                               lostream << "*** SCANNER : error: cannot include file: " << _str.c_str() << '"' << std::endl;
                               lostream << "*** SCANNER : error: skipping the include command. " << std::endl;
                           }
                           else
                               if (!pushStream(*fs, _str.c_str())) {
                                   lostream << "*** SCANNER : error :Includes nested too deeply (max depth = "
                                            << MAX_INCLUDE_DEPTH << ")" << std::endl;
                               }
                           BEGIN(INITIAL);
                       }
                       else {
                           BEGIN(INITIAL);
                           VAL->string_t = new std::string(_str);
                           return TokBinaryInclude;
                       }
                   }
                  }

:[iI]fdef[^\n]+   {TRACE;
                    std::string a(YYText());
                    std::string::const_iterator _it = a.begin() + 6;
                    while(_it != a.end() && ( *_it == ' ' || *_it == '\t'))_it++;
                    if(_it == a.end())lostream << "error: '" << a << '\'' << std::endl;
                    std::string::const_iterator _it2 = _it;
                    while(_it2 != a.end() && *_it2 != ' ' && *_it2 != '\t' && *_it2 != '\n')_it2++;
                    std::string n(_it,_it2);
                    if(MacroTable.find(n) == MacroTable.end()){
                            BEGIN(ifdef);
                    }
              }
:[iI]fndef[^\n]+   {TRACE;
                    std::string a(YYText());
                    std::string::const_iterator _it = a.begin() + 7;
                    while(_it != a.end() && ( *_it == ' ' || *_it == '\t'))_it++;
                    if(_it == a.end())lostream << "error: '" << a << '\'' << std::endl;
                    std::string::const_iterator _it2 = _it;
                    while(_it2 != a.end() && *_it2 != ' ' && *_it2 != '\t' && *_it2 != '\n')_it2++;
                    std::string n(_it,_it2);
                    if(MacroTable.find(n) != MacroTable.end()){
                            BEGIN(ifdef);
                    }
              }

<ifdef>:[eE]lse   {TRACE;BEGIN(INITIAL);}
<ifdef>:[eE]ndif  {TRACE;BEGIN(INITIAL);}
<ifdef>[^\n]+     {TRACE;}
<ifdef>\n         {TRACE;_columno=1;}

:[eE]ndif         {TRACE;}
:[eE]lse          {TRACE;BEGIN(ifdef);}

<comment>[^#\n]*        /* eat anything that's not a '#' */
<comment>"#"+[^#)\n]*   /* eat up '#'s not followed by ')'s */
<comment>\n
<comment>"#"+")"  { BEGIN(INITIAL); }
<comment><<EOF>>  {lostream << "*** SCANNER : unexpected end in comment." << std::endl;
                   if (!popStream()) {
                                        yyterminate();
                                        MacroTable.clear();
                                        }
                  }

\r              {TRACE;}
\n              {_columno=1;TRACE;}
<<EOF>>         {Trace("EOF");if (!popStream()){MacroTable.clear(); yyterminate();}}
.               {Trace("unmatched");
#ifdef LEXDEBUG
                 printChar(*YYText());
#endif
                 return TokError;}

%%

std::string lexerVersion(){
  int maj = YY_FLEX_MAJOR_VERSION;
  int min = YY_FLEX_MINOR_VERSION;
  std::string _version = number(maj)+'.'+number(min);
  return _version;
}

