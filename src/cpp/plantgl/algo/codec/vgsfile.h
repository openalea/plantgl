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


#ifndef __vgs_importer_h__
#define __vgs_importer_h__

#include "codec_config.h"

#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/scene/scene.h>

// #include <list>
#include <vector>
#include <plantgl/tool/util_hashmap.h>

PGL_BEGIN_NAMESPACE

typedef pgl_hash_map<uint_t,AppearancePtr> VGStarColorMap;

/* ----------------------------------------------------------------------- */

class VegeStarFile;

class CODEC_API VegeStarLine {
public :
        friend class VegeStarFile;
        VegeStarLine():shapeid(-1){}
        ~VegeStarLine(){}

        bool isValid() const { return shapeid != -1; }

        ShapePtr build(VGStarColorMap&) const ;

protected:
        GeometryPtr transform(GeometryPtr geom) const;
        int shapeid;
        Vector3 scaling,translation,rotation;
        Vector3 point1,point2,point3;
        Color3 color;
};

/* ----------------------------------------------------------------------- */

class CODEC_API VegeStarFile {
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

PGL_END_NAMESPACE

#endif
