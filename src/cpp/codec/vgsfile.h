/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2002 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): Frederic Boudon (frederic.boudon@cirad.fr)
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

#ifndef __vgs_importer_h__
#define __vgs_importer_h__

#include "Tools/util_vector.h"
#include "GEOM/appe_color.h"
#include "GEOM/scne_shape.h"
#include "GEOM/scne_scene.h"

// #include <list>
#include <vector>
#include "Tools/util_hashmap.h"

GEOM_BEGIN_NAMESPACE

typedef STDEXT::hash_map<uint32_t,AppearancePtr> VGStarColorMap;

/* ----------------------------------------------------------------------- */

class VegeStarFile;

class VegeStarLine {
public :
        friend class VegeStarFile;
        VegeStarLine():shapeid(-1){}
        ~VegeStarLine(){}

        bool isValid() const { return shapeid != -1; }

        GeomShapePtr build(VGStarColorMap&) const ;

protected:
        GeometryPtr transform(GeometryPtr geom) const;
        int shapeid;
        TOOLS(Vector3) scaling,translation,rotation;
        TOOLS(Vector3) point1,point2,point3;
        Color3 color;
};

/* ----------------------------------------------------------------------- */

class VegeStarFile {
public :

        // Predefined Polygonal Shape
        static GeometryPtr SHAPE_11;
        static GeometryPtr SHAPE_12;
        static GeometryPtr SHAPE_13;

        // Surface
        static  GeometryPtr SHAPE_21;
        static  GeometryPtr SHAPE_22;
        static  GeometryPtr SHAPE_23;
        static  GeometryPtr SHAPE_24;

        // Volume
        static  GeometryPtr SHAPE_31;
        static  GeometryPtr SHAPE_32;

        static void initShape();
        static void setShape11(GeometryPtr geom);
        static void setShape12(GeometryPtr geom);
        static void setShape13(GeometryPtr geom);

        static GeometryPtr getShape11();
        static GeometryPtr getShape12();
        static GeometryPtr getShape13();

        enum VgstarItem {
         obj,
         EchX, EchY, EchZ,
         TransX, TransY, TransZ,
         RotX, RotY, RotZ,
         R, G, B,
         X1, Y1, Z1,
         X2, Y2, Z2,
         X3, Y3, Z3,
         Mask,
         Grp1, Grp2,
         Pmax, Alpha,  Teta,  Resp,
         VCmax, Jmax,
         Error
        };

        VegeStarFile(){initShape();};
        ~VegeStarFile(){};

        static ScenePtr read(const std::string& filename,
                             std::ostream& error);

        static GeometryPtr importPolygonFile(const std::string& filename,
                                             std::ostream& error);

        bool parse(const std::string& filename);
        bool parse(std::istream& stream);

        bool parseHeader(std::istream& stream);
        bool parseLine(std::istream& stream);

        ScenePtr build() const;

protected :

        std::vector<VgstarItem> attlist;
        std::vector<VegeStarLine> objlist;
};

/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

#endif
