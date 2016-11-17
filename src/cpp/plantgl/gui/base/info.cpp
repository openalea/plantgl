/* -*-c++-*-
 *  ----------------------------------------------------------------------------
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
 *  ----------------------------------------------------------------------------
 */

#include "info.h"
#include <plantgl/tool/util_types.h>
#include <plantgl/tool/readline.h>
#include <plantgl/algo/opengl/util_glu.h>

#include <QtGui/qlabel.h>
#include <QtGui/qpainter.h>
#ifdef QT3_SUPPORT
#include <Qt3Support/q3listview.h>
#endif
#include <QtGui/qpushbutton.h>
#include <QtGui/qlayout.h>
#include <QtCore/qvariant.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qwhatsthis.h>
#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qapplication.h>
#include <QtCore/QSysInfo>
#include <QtCore/qtextcodec.h>
#include <QtGui/qevent.h>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QTreeWidget>

/* ----------------------------------------------------------------------- */

#ifndef  GL_SHADING_LANGUAGE_VERSION_ARB
#define  GL_SHADING_LANGUAGE_VERSION_ARB		0x8B8C
#endif

#ifdef _WIN32
#include <process.h>
#endif

#ifdef __GNUC__
#ifndef __MINGW32__
#include <sys/utsname.h>
#include <unistd.h>
#endif
#endif
#include <QtOpenGL/qgl.h>
//#include <GL/gl.h>

#ifdef  __GNUC__
#if defined(__MINGW32__) || defined(__APPLE__)
#else
#include <GL/glx.h>
#include <QtGui/QX11Info>
#endif
#endif

#ifdef _WIN32
#include <wingdi.h>
#include <winbase.h>
#endif

#include <plantgl/algo/opengl/util_glut.h>

#ifdef USE_READLINE
#include <stdio.h>
extern "C" {
#include <readline/readline.h>
}
#endif


/* ----------------------------------------------------------------------- */

static const char * icon_memory[] = {
/* width height num_colors chars_per_pixel */
"    32    32       71            2",
/* colors */
".. c #000000",".# c #010101",".a c #020202",".b c #040404",".c c #050505",
".d c #060606",".e c #080808",".f c #090909",".g c #0a0a0a",".h c #0c0c0c",
".i c #0d0d0d",".j c #0f0f0f",".k c #101010",".l c #111111",".m c #131313",
".n c #141414",".o c #151515",".p c #171717",".q c #181818",".r c #1a1a1a",
".s c #1b1b1b",".t c #1c1c1c",".u c #1e1e1e",".v c #1f1f1f",".w c #202020",
".x c #222222",".y c #232323",".z c #252525",".A c #262626",".B c #272727",
".C c #292929",".D c #2a2a2a",".E c #2b2b2b",".F c #2d2d2d",".G c #2e2e2e",
".H c #2f2f2f",".I c #303030",".J c #585858",".K c #808080",".L c #8c8c8c",
".M c #8d8d8d",".N c #8e8e8e",".O c #919191",".P c #939393",".Q c #969696",
".R c #979797",".S c #9a9a9a",".T c #9c9c9c",".U c #9e9e9e",".V c #a0a0a4",
".W c #a3a3a3",".X c #a4a4a4",".Y c #a7a7a7",".Z c #a9a9a9",".0 c #acacac",
".1 c #adadad",".2 c #b0b0b0",".3 c #b1b1b1",".4 c #b4b4b4",".5 c #b6b6b6",
".6 c #b9b9b9",".7 c #bababa",".8 c none",".9 c #bfbfbf","#. c #c0c0c0",
"## c #c1c1c3","#a c #d2d2d2","#b c #dadada","#c c #dcdcdc","#d c #e1e1e1",
"#e c #ffffff",
/* pixels */
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.I.I.8.8.8.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.I.V#b.J.I.9.H.H.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.I.K#b#e.I.8.H.H.J...8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.I#b#e.I.8.H.H.J...K.J.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.I.I.J.I.I.8.H.H.J...J.I.I.K.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.I.V#b.J.I.6.E.F.G.H.V.I.I#..H.H.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.I.K#b#e.I.4.B.C.D.E.F.G.V##.H.H.H.H.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.I#b#e.I.2.y.z.A.B.C.D.E.F.G.H.H.H.9...8.8.8",
".8.8.8.8.8.8.8.8.I.I.J.I.I.0.v.w.x.y.z.A.B.C.D.E.F.G.9.....8.8.8",
".8.8.8.8.8.8.8.I.V#b.J.I.Y.s.t.u.v.w.x.y.z.A.B.C.D.7.........8.8",
".8.8.8.8.8.8.I.K#c#e.I.W.p.q.r.s.t.u.v.w.x.y.z.A.5......#e...8.8",
".8.8.8.8.8.8.I#b#e.I.U.m.n.o.p.q.r.s.t.u.v.w.x.3......#e#e.J.8.8",
".8.8.8.8.I.I.J.I.I.S.j.k.l.m.n.o.p.q.r.s.t.u.1......#e#e#e#d.J.8",
".8.8.8.I.V#b#e.I.Q.g.h.i.j.k.l.m.n.o.p.q.r.Z..........#a#d.J.J.8",
".8.8.I.K#c#e.I.O.d.e.f.g.h.i.j.k.l.m.n.o.X......#e...8.I.J.J.8.8",
".8.8.I#b#e.I.M.a.b.c.d.e.f.g.h.i.j.k.l.V......#e#e.J.8.V.I.K.8.8",
".8.8.J.I.I.L.....#.a.b.c.d.e.f.g.h.i.T......#e#e#e#a.J.8.I.8.8.8",
".8.8.I.8.....L.......#.a.b.c.d.e.f.R..........#a#a.J.J.8.I.8.8.8",
".8.8.I.8.......L.........#.a.b.c.P......#e...8.I.J.J.V.8.8.8.8.8",
".8.8.8.8.........L...........#.N......#e#e.J.8.V.I.K.8.8.8.8.8.8",
".8.8.8.8.8.........L.........L......#b#b.3.L.J.8.I.8.8.8.8.8.8.8",
".8.8.8.8.8...........L.....L...........3.L.J.J.8.I.8.8.8.8.8.8.8",
".8.8.8.8.8.8...........L.L......#e...8.I.J.J.V.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.........I.J....#e#b.J.8.V.I.K.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.........J..#e#b.3.L.J.8.I.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.......J.....3.L.J.J.8.I.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8...J...I.8...J.J.V.8.8.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.V...K.8.8.8.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8...8.8.8.8.8.8.8.8.8.8.8.8.8.8.8",
".8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8...8.8.8.8.8.8.8.8.8.8.8.8.8.8.8"
};


/* ----------------------------------------------------------------------- */


const char* ViewSysInfo::info_logo[24] = {
"16 16 7 1",
". c None",
"d c #000000",
"# c #313031",
"b c #5a595a",
"e c #838183",
"c c #a4a1a4",
"a c #dedede",
"................",
"........#.......",
".......#a##b....",
"......#b##bb#...",
".....#a####c##..",
"....#b#######cd.",
"...#a#######cdc.",
"..#b#######cdacb",
".#a#######cdc.b.",
".b#ddddddcdacb#.",
".#ddddddedc.bc..",
"...ddddedacb#...",
"...dddedc.bc....",
"....dd#acb#.....",
".....dd.bc......",
"........#......."};


/* ----------------------------------------------------------------------- */


static const char * x_logo[] = {
/**/
"16 16 2 1",
/**/
"       s None  c None",
".      c red",
"                ",
"                ",
"                ",
" .....       .  ",
"  .....     .   ",
"   .....   .    ",
"    ..... .     ",
"     ... .      ",
"      . ...     ",
"     . .....    ",
"    .   .....   ",
"   .     .....  ",
"  .       ..... ",
"                ",
"                ",
"                "};


/* ----------------------------------------------------------------------- */


static const char *linux_logo[]={
"16 16 141 2",
".h c #030305","#b c #080709","#k c #09090a",".5 c #0a090b",".O c #0a0a0f",
".i c #111116",".q c #121214",".W c #1b1b1c",".y c #1c1912",".j c #24232c",
"#Q c #242427",".g c #252429",".X c #25252d","#R c #2a1f10",".9 c #2c2c2f",
".o c #3b3a3e",".F c #413e3c",".S c #424245","#c c #434349","#P c #454549",
".G c #47474e",".a c #48474f",".n c #56555b","#l c #575248",".b c #585760",
"#1 c #6d6667",".z c #6d6d77",".v c #6f5e38",".4 c #737273","#O c #775a1c",
"#2 c #77767c","#6 c #796e66",".6 c #7d7d84","#7 c #817f82","#4 c #828084",
".p c #828184","#0 c #846f4d","#3 c #848387","#E c #857d71","#V c #868484",
"#5 c #88714b","#r c #896f1b",".C c #89795b",".r c #8a8a8d","#v c #8b660b",
".Z c #8e8d94","#a c #8e8e8e","#j c #8f8455","#g c #92741a","#Z c #948b86",
".P c #94949a","#U c #98804a",".N c #989897","#x c #a2a1a3",".J c #a4a3aa",
"#8 c #a7a6a8",".0 c #a8a8aa",".x c #a99431","#m c #ababac","#C c #b2a98d",
"#s c #b2b2b3","ab c #b2b2b4","af c #b4b4b5","ac c #bababc",".H c #bcbbbd",
"#H c #bda661",".# c #bebdc1",".K c #bebebf","#u c #bf8e0e","ag c #bfbec0",
"#d c #bfbfc0","aa c #c0bfc0","#w c #c1a033",".Y c #c1c0c3","#W c #c2c2c2",
"#Y c #c4c3c6",".D c #c69f1c",".c c #c6c5cc",".A c #c7c7c8",".E c #c8ab5e",
".k c #cbcbce","ad c #cccbcb",".w c #cead20","#L c #cf9f24",".e c #d1d1d1",
"ae c #d1d1d2","#h c #d3d2ce","#S c #d59e0d","#I c #d5d5d5","a# c #d5d5d6",
"#T c #d6a514",".L c #d7d5d1","#9 c #d7d7d8",".s c #dadadb","#M c #dca70f",
".7 c #dcdcdc","#K c #dec793","#F c #dfa90f","ah c #e0e0e0","#z c #e1c580",
"#p c #e2b435",".R c #e2e2e4","#N c #e4ae0f","#B c #e4b310","#t c #e5e0d2",
"#y c #e5e5e6",".Q c #e6e6e6","#o c #e9d9b1","#f c #e9e3d9","#. c #ededec",
"#n c #ededed","#q c #eebd11",".T c #eeeeed","#G c #efba11","#A c #f0bc11",
"#D c #f0f0ef","aj c #f0f0f0",".M c #f1f0ef",".V c #f1f1ef","ak c #f1f1f1",
".8 c #f2f2f2",".U c #f3f3f1",".1 c #f4f4f2",".l c #f4f4f4","#i c #f5f3ea",
"## c #f5f5f3",".2 c #f5f5f4","#e c #f5f5f5",".3 c #f6f6f4","#X c #f6f6f6",
".m c #f6f6fa",".d c #f7f7f7","ai c #f8f8f8","a. c #f9f9f9",".B c #fafafa",
"#J c #fbfbfb",".f c #fbfbfc",".u c #fcfcfc",".t c #fdfdfd",".I c #fefefe",
"Qt c None",
"QtQtQtQtQt.#.a.b.c.dQtQtQtQtQtQt",
"QtQtQtQt.f.g.h.i.j.k.lQtQtQtQtQt",
"QtQtQtQt.m.n.o.p.q.r.s.tQtQtQtQt",
"QtQtQtQt.u.v.w.x.y.z.A.BQtQtQtQt",
"QtQtQtQt.t.C.D.E.F.G.H.lQtQtQtQt",
"QtQtQt.I.J.K.L.M.N.O.P.Q.IQtQtQt",
"QtQtQt.R.S.T.U.V.e.W.X.Y.lQtQtQt",
"QtQtQt.Z.0.1.1.2.3.4.5.6.7.tQtQt",
"QtQt.8.9#..1####.2#a#b#c#d#eQtQt",
"QtQt#f#g#h.U#####i#j#k#l#m#nQtQt",
"Qt#o#p#q#r#s.3.2#t#u#v#w#x#yQtQt",
"Qt#z#A#q#B#C###D#E#F#G#q#H#I#JQt",
"Qt#K#L#M#N#O#P#Q#R#S#T#U#V#W#XQt",
"Qt#X#Y#Z#0#1#2#3#4#5#6#7#8#9a.Qt",
"Qt.t#na#aaabacadae.AafagahaiQtQt",
"QtQtQt#J#eaj.la..B#Xak.l.tQtQtQt"};


/* ----------------------------------------------------------------------- */


static const char * opengl_logo[] = {
"33 16 110 2",
"       c None",".      c #B5CEDE","+   c #5A94B5","@   c #296B8C","#   c #31738C",
"$      c #317394","%   c #316B8C","&   c #4A849C","*   c #F7F7EF","=   c #6B9CB5",
"-      c #296B84",";   c #004A63",">   c #083952",",   c #737373","'   c #948C94",
")      c #A5A5A5","!   c #B5B5B5","~   c #B5ADAD","{   c #948C8C","]   c #003952",
"^      c #397B9C","/   c #297394","(   c #00394A","_   c #9C9C9C",":   c #E7E7E7",
"<      c #F7F7F7","[   c #425A6B","}   c #C6C6C6","|   c #4284A5","1   c #216384",
"2      c #847B7B","3   c #A59CA5","4   c #EFEFEF","5   c #214A63","6   c #317B9C",
"7      c #397394","8   c #005273","9   c #183142","0   c #002939","a   c #185A7B",
"b      c #105A7B","c   c #00526B","d   c #186384","e   c #C6D6E7","f   c #002931",
"g      c #D6D6D6","h   c #739CBD","i   c #EFE7E7","j   c #00314A","k   c #002129",
"l      c #DEDEDE","m   c #EFEFE7","n   c #7B7B84","o   c #CEDEE7","p   c #CEC6C6",
"q      c #216B84","r   c #085273","s   c #085A7B","t   c #00425A","u   c #184252",
"v      c #DEE7EF","w   c #102931","x   c #085A73","y   c #314A52","z   c #8C8C8C",
"A      c #8CADC6","B   c #94B5CE","C   c #21637B","D   c #08526B","E   c #8C8484",
"F      c #528CAD","G   c #A59C9C","H   c #DED6D6","I   c #001021","J   c #94949C",
"K      c #082131","L   c #ADC6D6","M   c #082931","N   c #216B8C","O   c #31424A",
"P      c #004A6B","Q   c #8C848C","R   c #6B9CBD","S   c #001821","T   c #427B9C",
"U      c #002131","V   c #296384","W   c #6394B5","X   c #5A94AD","Y   c #C6BDBD",
"Z      c #5A636B","`   c #213942"," .  c #73A5BD","..  c #004263","+.  c #636B6B",
"@.     c #6BA5BD","#.  c #005A73","$.  c #105A73","%.  c #638CA5","&.  c #949494",
"*.     c #084A63","=.  c #314A5A","-.  c #A5BDCE",";.  c #BDBDBD",">.  c #4A84A5",
",.     c #CECECE","'.  c #FFF7F7",").  c #083142","!.  c #29394A","~.  c #106384",
"                                                                  ",
"            . + @ @ @ @ # $ $ $ $ % & *                           ",
"        = $ $ @ - ; > , ' ) ! ! ~ ) { ] % ^                       ",
"      ^ / @ ; ( _ :                     < : [ }                   ",
"    | $ % 1 2 3 <                           4 5                   ",
"  6 7 @ 1 8 9 0 a                             b c d d   e f g     ",
"h $ - 1 a _     i j                         = k l m b n o 0 p     ",
"$ @ q 1 r 4       s @ t u j v w u ] $ w x y b z   A B C . 0 p     ",
"/ @ q d b <       D d E g ; F G H r @ 3 F I s J   b d K L M p     ",
"] - @ N N g       O P J   b % Q z z d 4 R S T t   < . U B k p     ",
"  V @ @ % 7 W X k Y P ; & Z : T R ` s 4  .I   ; @ ..+.U @.#.$.d 0 ",
"  : t @ $ 7 %.} 4   t &.l <   4 l < *     :   *.4 i   4   < < < < ",
"      =.$ $ @ - @ -.} ;.                L >.Z ,.                  ",
"      '.).q @ - V V T = o         v h $ D K ~                     ",
"          4 ~ !.t ~.- $ $ $ # V 1 t !.&.:                         ",
"                                                                  "};


/* ----------------------------------------------------------------------- */


const char *ViewSysInfo::qt_logo[]={
"16 16 7 1",
"b c #000000",
"d c #002000",
"e c #3f3f00",
"# c #3f5f00",
"c c #3f7f00",
"a c #7f9f00",
". c #7fbf00",
"................",
"................",
"......####......",
"....abbbbbbc....",
"...abda..cebc...",
"...bd....c.eb...",
"..#ba...dbccb#..",
"..#b...dbd..b#..",
"..#b..dbbe..b#..",
"..#bccbdebeab#..",
"...be.c..ebbb...",
"...cbec..#bbe...",
"....cbbbbbbeba..",
"......####..a...",
"................",
"................"};


/* ----------------------------------------------------------------------- */


const char * ViewSysInfo::tools_logo[34] = {
"16 16 17 1",
"       c None",
".      c #000000",
"+      c #808080",
"@      c #646464",
"#      c #DCDCDC",
"$      c #C3C3C3",
"%      c #A0A0A0",
"&      c #B8B8B8",
"*      c #787878",
"=      c #FFFFFF",
"-      c #EDEDED",
";      c #585858",
">      c #C2C2C2",
",      c #B7B7B7",
"'      c #E7E7E7",
")      c #303030",
"!      c #CFCFCF",
"   ......       ",
"   .+@#$%..     ",
"    .&*@#$%.    ",
"...  ..&@#$.    ",
".=$..  .@##%.   ",
".=-#$.;@>##,.   ",
".@=-######'$%.. ",
")=@*=--##'$!$%%.",
".$#=@@@*%$-#!$,%",
" ..$#$$$*@*=-#!$",
"   ...%#$$%@*=-#",
"      ....-#%@*=",
"          ..-#%@",
"            ..-#",
"              ..",
"                "};


/* ----------------------------------------------------------------------- */


static const char * wheel_logo[] = {
"16 16 17 1",
"       c None",
".      c #6D6810",
"+      c #010100",
"@      c #FFFFFE",
"#      c #F5EAB4",
"$      c #E1CD7C",
"%      c #000000",
"&      c #C7B23C",
"*      c #FFFFFF",
"=      c #DEDEDE",
"-      c #E9E2E1",
";      c #AC9E2F",
">      c #E6DEE6",
",      c #CDC6CD",
"'      c #A4A1A4",
")      c #C5C2C5",
"!      c #C5C6C5",
"           .    ",
"          +@..  ",
"         .@##$. ",
"      %  .#++&&+",
"   % %*+.##++&+ ",
"  %*%*=-++$&&;+ ",
" %***=>,,'+.;+  ",
"  %*=>,,''+ +   ",
" %*=>%%%'''%    ",
"%*=>)%*%'')!%   ",
" %>!,%%%'!,%    ",
"  %,'''')!%     ",
" %,''''!,=>%    ",
"  %'%')!%>%     ",
"   % %>% %      ",
"      %         "};

#define TYPEITEMINFO(item, type) \
	item->setText( 0, tr( #type ) );  \
    item->setText( 1, QString::number(sizeof(type)*8 )+" bits" );  \

#define TYPEITEM(item, itemfather, type) \
    item = new QTreeWidgetItem( itemfather , item); \
	TYPEITEMINFO(item, type)


/* ----------------------------------------------------------------------- */

ViewSysInfo::ViewSysInfo( QWidget* parent, QGLWidget * frameGL, const char* name, bool modal, Qt::WindowFlags fl )
  : QDialog( parent, fl )
{
/* ----------------------------------------------------------------------- */
	if(name)setObjectName(name);
    QString qname = name;
	setModal(modal);
    QPixmap image0( ( const char** ) info_logo );
    resize( 563, 503 );
    setWindowTitle( tr( "Technical Characteristics" ) );
    setWindowIcon( image0 );
    // setIconText( tr( "Technical Characteristics" ) );

    QPushButton * SaveButton = new QPushButton( this );
    SaveButton->setGeometry( QRect( 200, 460, 104, 28 ) );
    SaveButton->setText( tr( "&Save" ) );
    QObject::connect(SaveButton ,SIGNAL(clicked()),this,SLOT(saveAsFile()));

    CancelButton = new QPushButton( this );
    CancelButton->setGeometry( QRect( 440, 460, 104, 28 ) );
    CancelButton->setText( tr( "&Cancel" ) );
    QObject::connect(CancelButton ,SIGNAL(clicked()),this,SLOT(reject()));

    OkButton = new QPushButton( this );
    OkButton->setGeometry( QRect( 320, 460, 104, 28 ) );
    OkButton->setText( tr( "&Ok" ) );
    QObject::connect(OkButton ,SIGNAL(clicked()),this,SLOT(accept()));

    Title = new QLabel( this );
    Title->setGeometry( QRect( 80, 20, 400, 40 ) );
    QFont font1(  Title->font() );
    font1.setFamily( "adobe-helvetica" );
    font1.setPointSize( 15 );
    font1.setBold( TRUE );
    Title->setFont( font1 );
	Title->setText( (name ? qname : tr( "PlantGL Viewer" ) ) );

    Icon = new QLabel( this );
    Icon->setGeometry( QRect( 10, 10, 60, 60 ) );
    Icon->setPixmap( QPixmap(icon_memory) );
    Icon->setScaledContents( TRUE );

/* ----------------------------------------------------------------------- */
    AttView = new QTreeWidget( this );
    AttView->setColumnCount(2);
    QStringList header;
    header << tr("Information") << tr("Value");
    AttView->setHeaderLabels(header);
    AttView->setSortingEnabled(false);

    RootItem = new QTreeWidgetItem( AttView, 0 );
    RootItem->setText( 0, (name ? qname : tr( "PlantGL Viewer" ) ) );
    AttView->addTopLevelItem(RootItem);
    AttView->expandItem(RootItem);

/* ----------------------------------------------------------------------- */

        QString proc;
        QString machine_name;
        QString sys_name;
        QString sys_version;
        QString sys_release;
        int num_proc = 0;

#if defined(__GNUC__)
#ifndef __MINGW32__
    struct utsname buf;
    uname(&buf);
    proc = QString(buf.machine);
    machine_name = QString(buf.nodename);
    sys_version = QString(buf.version);
    sys_release = QString(buf.release);
    sys_name = QString(buf.sysname);
#endif
#endif

#ifdef _WIN32
        LPSYSTEM_INFO lpSystemInfo = new SYSTEM_INFO;
        GetSystemInfo(lpSystemInfo);
        switch(lpSystemInfo->wProcessorArchitecture){
        case PROCESSOR_ARCHITECTURE_INTEL:
          proc = "Intel ";
          switch(lpSystemInfo->wProcessorLevel%10){
          case 3:
            proc += "i386";
            break;
          case 4:
            proc += "i486";
            break;
          case 5:
            proc += "Pentium";
            break;
          case 6:
            proc += "Pentium 4";
            break;
          default:
            proc += QString::number(lpSystemInfo->wProcessorLevel);
          }
        case PROCESSOR_ARCHITECTURE_MIPS:
          proc = "MIPS ";
          switch(lpSystemInfo->wProcessorLevel){
          case 4:
            proc += "R4000";
            break;
          default:
            proc += QString::number(lpSystemInfo->wProcessorLevel);
          }
        case PROCESSOR_ARCHITECTURE_ALPHA:
          proc = "Alpha "+QString::number(lpSystemInfo->wProcessorLevel);
        case PROCESSOR_ARCHITECTURE_PPC:
          proc = "PowerPC ";
          switch(lpSystemInfo->wProcessorLevel%10){
          case 1:
            proc += "601";
            break;
          case 3:
            proc += "603";
            break;
          case 4:
            proc += "604";
            break;
          case 6:
            proc += "603+";
            break;
          case 9:
            proc += "604+";
            break;
          case 20:
            proc += "620";
            break;
          default:
            proc += QString::number(lpSystemInfo->wProcessorLevel);
          }
        }
        num_proc = (int)(lpSystemInfo->dwNumberOfProcessors);

        char lpBuffer[MAX_COMPUTERNAME_LENGTH+1]; // address of name buffer
        DWORD nSize(MAX_COMPUTERNAME_LENGTH+1);
        if(GetComputerNameA(lpBuffer,&nSize))
                for(int i = 0 ; i < nSize ; i++){
                        machine_name += QChar(lpBuffer[i]);
                }


        LPOSVERSIONINFO lpVersionInformation = new OSVERSIONINFO;
        lpVersionInformation->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(GetVersionEx(lpVersionInformation)){
                if(lpVersionInformation->dwPlatformId == VER_PLATFORM_WIN32s)
                        sys_name = "Windows 3.1";
                else if(lpVersionInformation->dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
                        sys_name = "Windows 9x";
                else if(lpVersionInformation->dwPlatformId == VER_PLATFORM_WIN32_NT)
                        sys_name = "Windows NT";
                sys_name += " (";
                QChar a = QChar(lpVersionInformation->szCSDVersion[0]);
                for(int i = 1 ; i < 128 && a != '\0'; i++){
                        sys_name += a;
                        a = QChar(lpVersionInformation->szCSDVersion[i]);
                }
                sys_name += ")";

                sys_release = QString::number(lpVersionInformation->dwMajorVersion)
                                + '.' + QString::number(lpVersionInformation->dwMinorVersion) ;
                sys_version = QString::number(lpVersionInformation->dwBuildNumber);
        }
        else sys_name = "Windows";

#endif

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * item0 = new QTreeWidgetItem( RootItem, RootItem );
    item0->setText( 0, tr( "Machine" ) );
    item0->setText( 1,  machine_name  );
    item0->setIcon( 0, image0 );
    AttView->collapseItem(item0);

    QTreeWidgetItem * item = new QTreeWidgetItem( item0 );
    item->setText( 0, tr( "Processor" ) );
    item->setText( 1, proc );

    if(num_proc>0){
     item = new QTreeWidgetItem( item0, item );
     item->setText( 0, tr( "Number of processor" ) );
     item->setText( 1, QString::number(num_proc) );
    }

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * item1 = new QTreeWidgetItem( RootItem, item0 );
    item1->setText( 0, tr( "System" ) );
    item1->setText( 1, sys_name);
    AttView->collapseItem(item1);

    QPixmap wheel_pix( ( const char** ) wheel_logo );
    if(sys_name == "Linux"){
      QPixmap linux_pix( ( const char** ) linux_logo );
      item1->setIcon( 0, linux_pix );
    }
    else item1->setIcon( 0, wheel_pix );


    item = new QTreeWidgetItem( item1 );
    item->setText( 0, tr( "Release" ) );
    item->setText( 1, sys_release );

    item = new QTreeWidgetItem( item1, item );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1, sys_version );

#ifdef _WIN32
    int cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SLANGUAGE,NULL,0);
    LPSTR lpLCData = new char[cchData];
    cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SLANGUAGE,lpLCData,cchData);
    QString lang = lpLCData;
    lang += " [";
    cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SENGLANGUAGE,NULL,0);
    delete [] lpLCData;
    lpLCData = new char[cchData];
    cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SENGLANGUAGE,lpLCData,cchData);
    lang += lpLCData;
    lang +=']';
    delete [] lpLCData;
    item = new QTreeWidgetItem( item1, item );
    item->setText( 0, tr( "Language" ) );
    item->setText( 1, lang );
//    item->setText( 1, QString(QTextCodec::codecForLocale()->name() ) );
#endif

   item = new QTreeWidgetItem( item1, item );
   item->setText( 0, tr( "Word Size" ) );
   item->setText( 1, QString::number(QSysInfo::WordSize)+" "+tr( "bits" ) );

   item = new QTreeWidgetItem( item1, item );
   item->setText( 0, tr( "Byte Order" ) );
   item->setText( 1, tr( (QSysInfo::ByteOrder? "Big Endian" : "Little Endian") ) );

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * item1b = new QTreeWidgetItem( RootItem, item1 );
    item1b->setText( 0, tr( "Process" ) );
    item1b->setIcon( 0, wheel_pix );
    AttView->collapseItem(item1b);

#if defined (_MSC_VER)
#define getpid _getpid
#endif

    item = new QTreeWidgetItem( item1b );
    AttView->collapseItem(item);

    item->setText( 0, tr( "Id" ) );
    item->setText( 1, QString::number(getpid()) );


/* ----------------------------------------------------------------------- */

    const char* c_date =  __DATE__ ;
    const char* c_time = __TIME__ ;

    QTreeWidgetItem * item2 = new QTreeWidgetItem( RootItem, item1b );
    item2->setText( 0, tr( "Compilation" ) );
    item2->setIcon( 0, wheel_pix );
    AttView->collapseItem(item2);

    item = new QTreeWidgetItem( item2 );
    AttView->collapseItem(item);
    item->setText( 0, tr( "Date" ) );
    item->setText( 1, tr(( QString(c_date) + " "+tr("at")+" " +  QString(c_time) ).toAscii().data()) );

/* ----------------------------------------------------------------------- */

    QString c_version;
    QString c_name;
    QString architect="";

#ifdef __GNUC__
    c_name = "GNU C++";
    c_version = __VERSION__ ;
#endif

#ifdef _WIN32
    item = new QTreeWidgetItem( item2, item );
    item->setText( 0, tr( "Build Mode" ) );
#ifdef _DEBUG
    item->setText( 1, tr( "Debug"  ) );
#else
    item->setText( 1, tr( "Release"  ) );
#endif

#if defined (_MSC_VER)
    c_name = "Microsoft Visual C++";
    c_version = QString::number(_MSC_VER/100.0 - 6) +" ("+ QString::number(_MSC_VER)+')' ;
#endif

#ifdef _M_ALPHA
    architect = QString("DEC ALPHA");
#endif
#ifdef _M_IX86
    int m_i86 = _M_IX86 + 86;
    architect = "i" + QString::number(m_i86);
#endif
#ifdef _M_MPPC
    int m_mppc = _M_MPPC;
    architect = "Mac Power PC " + QString::number(m_mppc);
#endif
#ifdef _M_PPC
      int m_ppc = _M_PPC;
      architect = "Power PC " + QString::number(m_ppc);
#endif
#ifdef _M_MRX000
      int m_mrx = _M_MRX000;
      architect = "R" + QString::number(m_mrx);
#endif

#endif

    QTreeWidgetItem * item2bis = new QTreeWidgetItem( item2, item);
    item2bis->setText( 0, tr( "C++ Compiler" ) );
    item2bis->setText( 1, c_name);
    AttView->collapseItem(item2bis);

    item = new QTreeWidgetItem( item2bis);
    item->setText( 0, tr( "Version" ) );
    item->setText( 1,  c_version  );

#ifdef _WIN32
    item = new QTreeWidgetItem( item2bis, item );
    item->setText( 0, tr( "Run-Time Type Information" ) );
#ifdef _CPPRTTI
    item->setText( 1, tr( "Enable"  ) );
#else
    item->setText( 1, tr( "Disable"  ) );
#endif
    item = new QTreeWidgetItem( item2bis, item );
    item->setText( 0, tr( "Exception Handling" ) );
#ifdef _CPPUNWIND
    item->setText( 1, tr( "Enable"  ) );
#else
    item->setText( 1, tr( "Disable"  ) );
#endif
    item = new QTreeWidgetItem( item2bis, item );
    item->setText( 0, tr( "Compiled on" ) );
    item->setText( 1, architect  );

#endif
#ifdef __GNUC__
    item = new QTreeWidgetItem( item2bis, item );
    item->setText( 0, tr( "Compilation Optimization" ) );
#ifdef __OPTIMIZE__
    item->setText( 1, tr( "Enable"  ) );
#else
    item->setText( 1, tr( "Disable"  ) );
#endif
#endif

#ifdef __cplusplus
#if __cplusplus != 1
    item = new QTreeWidgetItem( item2bis, item );
    item->setText( 0, tr( "C++ Standard's Version" ) );
	item->setText( 1, QString::number(__cplusplus) );
#endif
#endif

    QTreeWidgetItem * item2tris = new QTreeWidgetItem( item2bis, item );
    item2tris->setText( 0, tr( "Types Size" ) );

    QTreeWidgetItem * item2tris1 = new QTreeWidgetItem( item2tris );
    item2tris1->setText( 0, tr( "Basic Types" ) );
    item = new QTreeWidgetItem( item2tris1);
	TYPEITEMINFO(item,  char)
	TYPEITEM(item, item2tris1, int)
	TYPEITEM(item, item2tris1, short int)
	TYPEITEM(item, item2tris1, long int)
	TYPEITEM(item, item2tris1, float)
	TYPEITEM(item, item2tris1, double)
	TYPEITEM(item, item2tris1, size_t)

    QTreeWidgetItem * item2tris2 = new QTreeWidgetItem( item2tris, item2tris1 );
    item2tris2->setText( 0, tr( "Custom Types" ) );
    item = new QTreeWidgetItem( item2tris2);
	TYPEITEMINFO(item, uchar_t)
	TYPEITEM(item, item2tris2, int16_t)
	TYPEITEM(item, item2tris2, uint16_t)
	TYPEITEM(item, item2tris2, int32_t)
	TYPEITEM(item, item2tris2, uint32_t)
	TYPEITEM(item, item2tris2, int_t)
	TYPEITEM(item, item2tris2, uint_t)
	TYPEITEM(item, item2tris2, real_t)

/* ----------------------------------------------------------------------- */

    QPixmap qt_pix( ( const char** ) qt_logo );

    QTreeWidgetItem * item3 = new QTreeWidgetItem( RootItem, item2 );
    item3->setIcon( 0, qt_pix );
    item3->setText( 0, tr( "Qt Library" ) );
    AttView->collapseItem(item3);

    item = new QTreeWidgetItem( item3 );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1, tr( qVersion() ) );
    item = new QTreeWidgetItem( item3, item );
    item->setText( 0, tr( "Thread Support" ) );
#ifdef QT_THREAD_SUPPORT
    item->setText( 1, tr( "Enable" ) );
#else
    item->setText( 1, tr( "Disable" ) );
#endif
#ifdef _WIN32
    item = new QTreeWidgetItem( item3, item );
    item->setText( 0, tr( "Qt DLL" ) );
#ifdef QT_DLL
    item->setText( 1, tr( "Yes" ) );
#else
    item->setText( 1, tr( "No" ) );
#endif
#endif
    QTreeWidgetItem * item3font = new QTreeWidgetItem( item3, item );
    item3font->setText( 0, tr( "Default Font" ) );
    item = new QTreeWidgetItem( item3font);
#if QT_VERSION < 300
    item->setText( 0, tr( "Charset" ) );
    item->setText( 1, QFont::encodingName(QApplication::font().charSet()));
    item = new QTreeWidgetItem( item3font,item);
#endif
    item->setText( 0, tr( "Family" ) );
    item->setText( 1, QApplication::font().family());
    item = new QTreeWidgetItem( item3font,item);
    item->setText( 0, tr( "Size" ) );
    item->setText( 1, QString::number(QApplication::font().pointSize()));

/* ----------------------------------------------------------------------- */

    QPixmap opengl_pix( ( const char** ) opengl_logo );

    const QGLContext * context = NULL;
    bool _defaultgl = false;
    if(frameGL != NULL){
                context = frameGL->context();
    }
    if(context == NULL){
                context = QGLContext::currentContext();
                if(context == 0){
                        _defaultgl = true;
                        frameGL = new QGLWidget(this);
                        frameGL->setGeometry(0,0,0,0);
                        frameGL->makeCurrent();
                        context = QGLContext::currentContext();
                }
    }

    QTreeWidgetItem * item4 = new QTreeWidgetItem( RootItem, item3 );
    item4->setIcon( 0, opengl_pix );

    if( frameGL != NULL){
                QString _clname = frameGL->metaObject()->className();
                if(_clname != "QGLWidget" )
                        item4->setText( 0, tr( "GL Widget")+" (" + _clname +')'   );
                else item4->setText( 0, tr( "GL Widget" ) );
    }
    else {
                item4->setText( 0, tr( "GL Widget"  ) );
    }

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * itemfgl1 = new QTreeWidgetItem( item4, item );
    itemfgl1->setText( 0, tr( "GL Context" ) );
    if(_defaultgl)itemfgl1->setText( 1, tr( "Default" ) );

    item = new QTreeWidgetItem( itemfgl1 );
    item->setText( 0, tr( "Valid" ) );
    item->setText( 1, tr( (context->isValid() ? "Yes" : "No" ) ));

    item = new QTreeWidgetItem( itemfgl1, item );
    item->setText( 0, tr( "Shared" ) );
    item->setText( 1, tr( (context->isSharing() ? "Yes" : "No" ) ));

/* ----------------------------------------------------------------------- */

    QGLFormat _glformat;
    if(context != NULL) _glformat = context->format();
    else _glformat = QGLFormat::defaultFormat();

    QTreeWidgetItem * itemfgl2 = new QTreeWidgetItem( item4, itemfgl1 );
    itemfgl2->setText( 0, tr( "GL Format" ) );
    if(_defaultgl)itemfgl2->setText( 1, tr( "Default" ) );

    item = new QTreeWidgetItem( itemfgl2 );
    item->setText( 0, tr( "Direct Rendering" ) );
    item->setText( 1, tr( (_glformat.directRendering() ? "Enable" : "Disable" ) ));
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Double Buffer" ) );
    item->setText( 1, tr( (_glformat.doubleBuffer() ? "Enable" : "Disable" )  ) );
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Depth Buffer" ) );
    item->setText( 1, tr( (_glformat.depth() ? "Enable" : "Disable" )  ) );
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Alpha channel" ) );
    item->setText( 1, tr( (_glformat.alpha() ? "Enable" : "Disable" )  ) );
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Accumulation buffer" ) );
    item->setText( 1, tr( (_glformat.accum() ? "Enable" : "Disable" )  ) );
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Stencil buffer" ) );
    item->setText( 1, tr( (_glformat.stencil() ? "Enable" : "Disable" )  ) );
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Stereo buffering" ) );
    item->setText( 1, tr( (_glformat.stereo() ? "Enable" : "Disable" )  ) );
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Overlay Plane" ) );
    item->setText( 1, tr( (_glformat.hasOverlay() ? "Enable" : "Disable" )  ) );
    item = new QTreeWidgetItem( itemfgl2, item );
    item->setText( 0, tr( "Plane" ) );
    item->setText( 1, QString::number(_glformat.plane() ) );

/* ----------------------------------------------------------------------- */

    if( frameGL !=0 ){
                const QGLContext * ocontext = frameGL->overlayContext();
                QTreeWidgetItem * itemfgl3 = new QTreeWidgetItem( item4, itemfgl2 );
                itemfgl3->setText( 0, tr( "Overlay GL Context" ) );
                if(ocontext == NULL) itemfgl3->setText( 1, tr( "None" ) );
                else {
                        if(_defaultgl)itemfgl3->setText( 1, tr( "Default" ) );
                        item = new QTreeWidgetItem( itemfgl3 );
                        item->setText( 0, tr( "Valid" ) );
                        item->setText( 1, tr( (context->isValid() ? "Yes" : "No" ) ));
                        item = new QTreeWidgetItem( itemfgl3, item );
                        item->setText( 0, tr( "Shared" ) );
                        item->setText( 1, tr( (context->isSharing() ? "Yes" : "No" ) ));

/* ----------------------------------------------------------------------- */

                        QGLFormat _oglformat = ocontext->format();

                        QTreeWidgetItem * itemfgl4 = new QTreeWidgetItem( item4, itemfgl3 );
                        itemfgl4->setText( 0, tr( "Overlay GL Format" ) );
                        if(_defaultgl)itemfgl4->setText( 1, tr( "Default" ) );
                        item = new QTreeWidgetItem( itemfgl4 );
                        item->setText( 0, tr( "Direct Rendering" ) );
                        item->setText( 1, tr( (_oglformat.directRendering() ? "Enable" : "Disable" ) ));
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Double Buffer" ) );
                        item->setText( 1, tr( (_oglformat.doubleBuffer() ? "Enable" : "Disable" )  ) );
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Depth Buffer" ) );
                        item->setText( 1, tr( (_oglformat.depth() ? "Enable" : "Disable" )  ) );
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Alpha channel" ) );
                        item->setText( 1, tr( (_oglformat.alpha() ? "Enable" : "Disable" )  ) );
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Accumulation buffer" ) );
                        item->setText( 1, tr( (_oglformat.accum() ? "Enable" : "Disable" )  ) );
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Stencil buffer" ) );
                        item->setText( 1, tr( (_oglformat.stencil() ? "Enable" : "Disable" )  ) );
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Stereo buffering" ) );
                        item->setText( 1, tr( (_oglformat.stereo() ? "Enable" : "Disable" )  ) );
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Overlay Plane" ) );
                        item->setText( 1, tr( (_oglformat.hasOverlay() ? "Enable" : "Disable" )  ) );
                        item = new QTreeWidgetItem( itemfgl4, item );
                        item->setText( 0, tr( "Plane" ) );
                        item->setText( 1, QString::number(_oglformat.plane() )   );
                }
    }

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * item5 = new QTreeWidgetItem( RootItem, item4 );
    item5->setIcon( 0, opengl_pix );
    item5->setText( 0, tr( "OpenGL" ) );
    AttView->collapseItem(item5);

    item = new QTreeWidgetItem( item5 );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1, QString((char*)glGetString(GL_VERSION)) ) ;
     fprintf( stderr, "Version OpenGL: %s\n", glGetString( GL_VERSION ) );
    item = new QTreeWidgetItem( item5, item );
    item->setText( 0, tr( "Vendor" ) );
    item->setText( 1, QString((char*)glGetString(GL_VENDOR)) );
    fprintf( stderr, "Vendor OpenGL: %s\n", glGetString( GL_VENDOR ) );
    item = new QTreeWidgetItem( item5, item );
    item->setText( 0, tr( "Renderer" ) );
    item->setText( 1, QString((char*)glGetString(GL_RENDERER) ) );


    QStringList ext = QString((char*)glGetString(GL_EXTENSIONS)).split ( ' ');

	if(ext.contains("GL_ARB_shading_language_100")){
		item = new QTreeWidgetItem( item5, item );
		item->setText( 0, tr( "Shading Language" ) );
		QString shading((char*)glGetString(GL_SHADING_LANGUAGE_VERSION_ARB));
		if(glGetError() == GL_NO_ERROR)
			item->setText( 1, shading );
		else
			item->setText( 1, "1.051" );
	}

    QTreeWidgetItem * item5bis = new QTreeWidgetItem( item5, item );
    item5bis->setText( 0, tr( "Extension(s)" ) );
    AttView->collapseItem(item5bis);

    if(!ext.isEmpty()){
                item = new QTreeWidgetItem( item5bis );
                item->setText( 0,  ext[0]  );
                for(uint i = 1 ; i < ext.count() ; i++){
                        item = new QTreeWidgetItem( item5bis, item );
                        item->setText( 0, ext[i]  );
                }
    }
    else item5bis->setText( 0, tr( "None" ) );

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * item6 = new QTreeWidgetItem( RootItem, item5 );
    item6->setText( 0, tr( "OpenGL Utility Library (GLU)" ) );
    item6->setIcon( 0, opengl_pix );
    AttView->collapseItem(item6);

    item = new QTreeWidgetItem( item6 );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1,  QString(gluGeomGetString(GLU_VERSION))  );

    ext = QString(gluGeomGetString(GLU_EXTENSIONS)).split ( ' ');
    if(!ext.isEmpty()){
                QTreeWidgetItem * item6bis = new QTreeWidgetItem( item6, item );
                item6bis->setText( 0, tr( "Extension(s)" ) );
                AttView->collapseItem(item6bis);

                item = new QTreeWidgetItem( item6bis );
                item->setText( 0,  ext[0]  );
                for(uint i = 1 ; i < ext.count() ; i++){
                        item = new QTreeWidgetItem( item6bis, item );
                        item->setText( 0, ext[i]  );
                }
    }
    // else item6bis->setText( 1, tr( "None" ) );

    LastItem = item6;

/* ----------------------------------------------------------------------- */

#ifdef GLUT_API_VERSION

    QTreeWidgetItem * item7 = new QTreeWidgetItem( RootItem, item6 );
    item7->setText( 0, tr( "OpenGL Utility Toolkit Library (Glut)" ) );
    item7->setIcon( 0, opengl_pix );
    AttView->collapseItem(item7);

    item = new QTreeWidgetItem( item7 );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1, QString::number(GLUT_API_VERSION)  );
#ifdef GLUT_XLIB_IMPLEMENTATION
    item = new QTreeWidgetItem( item7, item );
    item->setText( 0, tr( "X Lib Implementation" ) );
    item->setText( 1, QString::number(GLUT_XLIB_IMPLEMENTATION)  );
#endif

    LastItem = item7;

#endif

/* ----------------------------------------------------------------------- */

#ifdef _WIN32
    QTreeWidgetItem * item8 = new QTreeWidgetItem( RootItem, LastItem );
    item8->setText( 0, tr( "OpenGL Windows Extension (WGL)" ) );
    item8->setIcon( 0, opengl_pix );
    AttView->collapseItem(item8);
    QTreeWidgetItem * item8bis = new QTreeWidgetItem( item8 );
    AttView->collapseItem(item8bis);
    item8bis->setText( 0, tr( "Extension(s)" ) );

    PROC f = wglGetProcAddress("wglGetExtensionsStringEXT");
    if(f != NULL){
		typedef const char*(APIENTRY * glExtFunc)(void);
        glExtFunc wglGetExtensionsString ;
        wglGetExtensionsString = (glExtFunc)f;
        if(wglGetExtensionsString != NULL){

            const char * msg = (*wglGetExtensionsString)();
            QStringList ext = QString(msg).split ( ' ' );
            if(!ext.isEmpty()){
                item = new QTreeWidgetItem( item8bis );
                item->setText( 0, ext[0] );
                for(uint i = 1 ; i < ext.count() ; i++){
                    item = new QTreeWidgetItem( item8bis, item );
                    item->setText( 0, ext[i] );
                }
            }
            else item8bis->setText( 1, tr( "None" ) );
        }
        else item8bis->setText( 1, tr( "None" ) );
    }
    else item8bis->setText( 1, tr( "None" ) );

    LastItem = item8;
#endif

/* ----------------------------------------------------------------------- */

#ifdef __GNUC__
#if defined(__MINGW32__) || defined(__APPLE__)
#else

    Display * dpy = QX11Info::display();

  if(dpy != NULL){
    QTreeWidgetItem * item8 = new QTreeWidgetItem( RootItem, LastItem );
    item8->setText( 0, tr( "OpenGL X Extension (GLX)" ) );
    item8->setIcon( 0, opengl_pix );
    AttView->collapseItem(item8);

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * item81 = new QTreeWidgetItem( item8 );
    item81->setText( 0, tr( "Client" ) );
    AttView->collapseItem(item81);


    const char * msg = glXGetClientString(dpy,GLX_VERSION);
    item = new QTreeWidgetItem( item81 );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1, msg );

    msg = glXGetClientString(dpy,GLX_VENDOR);
    item = new QTreeWidgetItem( item81, item );
    item->setText( 0, tr( "Vendor" ) );
    item->setText( 1, msg  );


    QTreeWidgetItem * item81bis = new QTreeWidgetItem( item81, item );
    item81bis->setText( 0, tr( "Extension(s)" ) );
    AttView->collapseItem(item81bis);

    msg = glXGetClientString(dpy,GLX_EXTENSIONS);
    ext = QString(msg).split ( ' ' );
    if(!ext.isEmpty()){
                item = new QTreeWidgetItem( item81bis );
                item->setText( 0, ext[0] );
                for(uint i = 1 ; i < ext.count() ; i++){
                        item = new QTreeWidgetItem( item81bis, item );
                        item->setText( 0, ext[i] );
                }
    }
    else item81bis->setText( 1, tr( "None" ) );

/* ----------------------------------------------------------------------- */

    QTreeWidgetItem * item82 = new QTreeWidgetItem( item8, item81 );
    item82->setText( 0, tr( "Server" ) );
    AttView->collapseItem(item82);


    QTreeWidgetItem * itemscreen = item82;

    int smax = ScreenCount(dpy);
    for(int screennumber = 0 ; screennumber < smax ; screennumber++){

                QTreeWidgetItem * itemscreen = new QTreeWidgetItem( item82, itemscreen );
                itemscreen->setText( 0, tr( "Screen") +" " + QString::number(screennumber));
                itemscreen->setText( 1, ( screennumber == DefaultScreen(dpy) ? tr("Default") : "") );
                AttView->collapseItem(itemscreen);

                msg = glXQueryServerString(dpy,screennumber,GLX_VERSION);
                item = new QTreeWidgetItem( itemscreen );
                item->setText( 0, tr( "Version" ) );
                item->setText( 1, msg  );

                msg = glXQueryServerString(dpy,screennumber,GLX_VENDOR);
                item = new QTreeWidgetItem( itemscreen, item );
                item->setText( 0, tr( "Vendor" ) );
                item->setText( 1,  msg  );

                msg = glXQueryServerString(dpy,screennumber,GLX_EXTENSIONS);
                QTreeWidgetItem * itemscreenbis = new QTreeWidgetItem( itemscreen, item );
                itemscreenbis->setText( 0, tr( "Extension(s)" ) );
                AttView->collapseItem(itemscreenbis);

                ext = QString(msg).split ( ' ' );
                if(!ext.isEmpty()){
                        item = new QTreeWidgetItem( itemscreenbis );
                        item->setText( 0,  ext[0] );
                        for(uint i = 1 ; i < ext.count() ; i++){
                                item = new QTreeWidgetItem( itemscreenbis, item );
                                item->setText( 0,  ext[i] );
                        }
                }
                else itemscreenbis->setText( 1, tr( "None" ) );

    }

        LastItem = item8;

/* ----------------------------------------------------------------------- */

    QPixmap x_pix( ( const char** ) x_logo );
    QTreeWidgetItem * item9 = new QTreeWidgetItem( RootItem, item8 );
    item9->setIcon( 0, x_pix );
    item9->setText( 0, tr( "Server" ) );
    AttView->collapseItem(item9);

    item = new QTreeWidgetItem( item9 );
    item->setText( 0, tr( "Display Name" ) );
    item->setText( 1, QString(DisplayString(dpy))  );

    item = new QTreeWidgetItem( item9, item );
    item->setText( 0, tr( "Vendor" ) );
    item->setText( 1, QString(ServerVendor(dpy)) );

    item = new QTreeWidgetItem( item9, item );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1, QString::number(VendorRelease(dpy)) );

    QTreeWidgetItem * item91 = new QTreeWidgetItem( item9, item );
    item91->setText( 0, tr( "Protocol" ) );
    item91->setText( 1, QString::number(ProtocolVersion(dpy))+ '.' +
                                                        QString::number(ProtocolRevision(dpy)) ) ;
    AttView->collapseItem(item91);

    item = new QTreeWidgetItem( item91 );
    item->setText( 0, tr( "Version" ) );
    item->setText( 1, QString::number(ProtocolVersion(dpy)) );
    item = new QTreeWidgetItem( item91, item );
    item->setText( 0, tr( "Revision" ) );
    item->setText( 1, QString::number(ProtocolRevision(dpy)) );

    item = new QTreeWidgetItem( item9, item91 );
    item->setText( 0, tr( "Number of Screen" ) );
    item->setText( 1, QString::number(smax) );

    QTreeWidgetItem * item92 = new QTreeWidgetItem( item9, item );
    item92->setText( 0, tr( "Screens" ) );
    AttView->collapseItem(item92);

    itemscreen =  NULL ;

    for(int sn = 0 ; sn < smax ; sn++){
                if(itemscreen) itemscreen =  new QTreeWidgetItem( item92 , itemscreen ) ;
                else itemscreen =  new QTreeWidgetItem( item92 ) ;
                itemscreen->setText( 0, tr( "Screen")+ " "+ QString::number(sn)+
                                                                        ( sn == DefaultScreen(dpy) ? " ("+tr("Default")+")" : "") );

                item = new QTreeWidgetItem( itemscreen );
                item->setText( 0, tr( "Resolution") ) ;
                item->setText( 1, QString::number(DisplayWidth(dpy,sn)) +" x "  +
                                                          QString::number(DisplayHeight(dpy,sn))+ " pixels ("+
                                                          QString::number(DisplayWidthMM(dpy,sn)) +" x "  +
                                                          QString::number(DisplayHeightMM(dpy,sn))+ " mm)" );
                item = new QTreeWidgetItem( itemscreen, item );
                item->setText( 0, tr( "Default Depth" ) );
                item->setText( 1, QString::number(DefaultDepth(dpy,sn)) );
                item = new QTreeWidgetItem( itemscreen, item );
                item->setText( 0, tr( "Number of Entries in Default Colormap" ) );
                item->setText( 1, QString::number(DisplayCells(dpy,sn)) );
                Screen * _screen = ScreenOfDisplay(dpy,sn );
                if(_screen != NULL){
                        int i = DoesBackingStore(_screen);
                        QString msg;
                        if(i == 0)msg = "When Mapped";
                        else if(i == 1) msg = "Not Useful";
                        else msg = "Always";
                        item = new QTreeWidgetItem( itemscreen, item );
                        item->setText( 0, tr( "Backing Store" ) );
                        item->setText( 1,  msg ) ;
                        item = new QTreeWidgetItem( itemscreen, item );
                        item->setText( 0, tr( "Save Unders" ) );
                        item->setText( 1,  (DoesSaveUnders(_screen)? "Supported" : "Not Supported") );
                }
    }

    QTreeWidgetItem * item93 = new QTreeWidgetItem( item9, item92 );
    item93->setText( 0, tr( "Connection Number" ) );
    item93->setText( 1, QString::number(ConnectionNumber(dpy)) );

    int * count_return = new int(0);
    XPixmapFormatValues * formatlist = XListPixmapFormats(dpy, count_return);
    if(*count_return!=0){
                item93 = new QTreeWidgetItem( item9, item93 );
                item93->setText( 0, tr( "Pixmap Format" ) );
                item93->setText( 1, QString::number(*count_return) );
                AttView->collapseItem(item93);

                QTreeWidgetItem * item931 = NULL;
                for(int u = 0 ; u < *count_return ; u++ ){
                        if(item931)item931 = new QTreeWidgetItem( item93 , item931);
                        else item931 = new QTreeWidgetItem( item93);

                        item931->setText( 0, tr( "Format")+ " " + QString::number(u) );
                        AttView->collapseItem(item931);

                        item = new QTreeWidgetItem( item931 );
                        item->setText( 0, tr( "Depth" ) );
                        item->setText( 1,  QString::number(formatlist[u].depth) );
                        item = new QTreeWidgetItem( item931 , item );
                        item->setText( 0, tr( "Bits per pixel" ) );
                        item->setText( 1, QString::number(formatlist[u].bits_per_pixel) ) ;
                        item = new QTreeWidgetItem( item931 , item );
                        item->setText( 0, tr( "Scanline Pad" ) );
                        item->setText( 1, QString::number(formatlist[u].scanline_pad)  );

                }
                XFree(formatlist);
    }
    delete count_return;

    item = new QTreeWidgetItem( item9, item93 );
    item->setText( 0, tr( "Max Request Size" ) );
    item->setText( 1,  QString::number(XMaxRequestSize(dpy))+" bytes of 4"  );
    item = new QTreeWidgetItem( item9, item );
    item->setText( 0, tr( "Image Byte Order" ) );
    item->setText( 1, (ImageByteOrder(dpy) == 0 ?  "Least-Significant Bit First ("
                : "Most-Significant Bit First (")+QString::number(ImageByteOrder(dpy))+")"  );

    QTreeWidgetItem * item94 = new QTreeWidgetItem( item9, item );
    item94->setText( 0, tr( "Bitmap" ) );

    item = new QTreeWidgetItem( item94 );
    item->setText( 0, tr( "Unit" ) );
    item->setText( 1,  QString::number(BitmapUnit(dpy)) );
    item = new QTreeWidgetItem( item94, item);
    item->setText( 0, tr( "Bit Order" ) );
    item->setText( 1, (BitmapBitOrder(dpy) == 0 ?  "Least-Significant Bit First ("
                             : "Most-Significant Bit First (")+QString::number(ImageByteOrder(dpy))+")" );
    item = new QTreeWidgetItem( item94 , item);
    item->setText( 0, tr( "Pad" ) );
    item->setText( 1, QString::number(BitmapPad(dpy)) );


    LastItem = item9;

  }
/* ----------------------------------------------------------------------- */

#ifdef USE_READLINE
  QPixmap tools_pix( ( const char** ) tools_logo );

  QTreeWidgetItem * item10 = new QTreeWidgetItem( RootItem, LastItem );
  item10->setText( 0, tr( "Readline" ) );
  item10->setIcon( 0, tools_pix );
  AttView->collapseItem(item10);

  item = new QTreeWidgetItem( item10 );
  item->setText( 0, tr( "Version" ) );
  item->setText( 1,  QString(rl_library_version)  );

  LastItem = item10;
#endif
/* ---------------------------------------------------------------------- */
#endif
#endif

  AttView->setGeometry( QRect( 80, 70, 460, 370 ) );
  AttView->setSortingEnabled(false);

/* ----------------------------------------------------------------------- */

}


/* ----------------------------------------------------------------------- */


/*
 *  Destroys the object and frees any allocated resources
 */
ViewSysInfo::~ViewSysInfo()
{
    // no need to delete child widgets, Qt does it all for us
}


/* ----------------------------------------------------------------------- */


/*
 *  Main event handler. Reimplemented to handle application
 *  font changes
 */
bool ViewSysInfo::event( QEvent* ev )
{
    bool ret = QDialog::event( ev );
    if ( ev->type() == QEvent::ApplicationFontChange ) {
        QFont font(  Title->font() );
        font.setFamily( "adobe-helvetica" );
        font.setPointSize( 15 );
        font.setBold( TRUE );
        Title->setFont( font );
    }
    return ret;
}

/* ----------------------------------------------------------------------- */

void ViewSysInfo::resizeEvent(QResizeEvent*e){
  QSize diffSize(103,127);
#ifdef QT3_SUPPORT
  AttView->resize(e->size()-diffSize);
#endif
  QPoint diffPos(e->size().width(),e->size().height());
  QPoint PosOk(243,43);
  QPoint PosCancel(123,43);
  OkButton->move(diffPos-PosOk);
  CancelButton->move(diffPos-PosCancel);
}


/* ----------------------------------------------------------------------- */
QTreeWidgetItem*
ViewSysInfo::addItem(const QString& name, const QString& val){
  QTreeWidgetItem * itemN = new QTreeWidgetItem( RootItem,LastItem );
  itemN->setText( 0, name  );
  itemN->setText( 1, val  );
  QPixmap tools_pix( ( const char** ) tools_logo );
  itemN->setIcon( 0, tools_pix );
  AttView->collapseItem(itemN);
  LastItem = itemN;
  return itemN;
}

QTreeWidgetItem*
ViewSysInfo::addItem(const QPixmap& pix,
                     const QString& name,
                     const QString& val){
  QTreeWidgetItem * itemN = new QTreeWidgetItem( RootItem,LastItem );
  itemN->setText( 0, name );
  itemN->setText( 1, val );
  itemN->setIcon( 0, pix );
  AttView->collapseItem(itemN);
  LastItem = itemN;
  return itemN;
}

void
ViewSysInfo::saveAsFile()  {
   QString file = QFileDialog::getSaveFileName ( this , "Save");
   if(!file.isEmpty())saveAsFile(file);
}


QTreeWidgetItem* nextSibling(QTreeWidgetItem* item ){
    QTreeWidgetItem* parent = item->parent();
    if (parent == NULL) return NULL;
    int idchild = parent->indexOfChild(item);
    if (idchild >= (parent->childCount()-1)) return NULL;
    else return parent->child(idchild+1);
}

void
ViewSysInfo::saveAsFile(const QString& file) const {
        QFile f(file);
		if(f.open(QIODevice::WriteOnly)){
                QTextStream stream(&f);
                QString indent;
                QTreeWidgetItem* current = RootItem;

                while(current != NULL){
                        stream << indent << current->text(0) << '\t' << current->text(1);
#ifdef _WIN32
                        stream << char(13) << endl;
#else
                        stream <<  endl;
#endif
                        QTreeWidgetItem* nextsibling;
                        if(current->childCount() != 0){
                                current = current->child(0);
                                indent += '\t';
                        }
                        else if((nextsibling = nextSibling(current)) != NULL)current = nextsibling;
                        else {
                                while(current != NULL && ((nextsibling = nextSibling(current)) == NULL)){
                                        current = current->parent();
                                        indent.truncate(indent.length ()-1);
                                }
                                if(current != NULL) current = nextsibling;
                        }
                }
                f.close();
        }
}
