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



#include <boost/python.hpp>

#include <plantgl/python/export_list.h>
#include <plantgl/algo/opengl/glskelrenderer.h>
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/algo/base/skelcomputer.h>
#include <plantgl/algo/base/bboxcomputer.h>
#include <plantgl/algo/opengl/glbboxrenderer.h>
#include <plantgl/algo/opengl/glctrlptrenderer.h>
#include <plantgl/scenegraph/appearance/texture.h>

#ifndef PGL_WITHOUT_QT

#include <QtOpenGL/qgl.h>
#include <plantgl/python/extract_widget.h>

#endif

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

/* ----------------------------------------------------------------------- */

GLRenderer::RenderingMode get_rd_mode(GLRenderer *rd) { return rd->getRenderingMode(); }

GLRenderer::SelectionId get_sel_mode(GLRenderer *rd) { return rd->getSelectionMode(); }

/*
QGLWidget * get_fgl_mode(GLRenderer * rd)
{ return rd->getGLFrame();}
*/

#ifndef PGL_WITHOUT_QT

void py_setGLFrame(GLRenderer *rd, boost::python::object widget) {
  rd->setGLFrame(extract_widget<QGLWidget>(widget)());
}

#endif

void export_GLRenderer() {
  scope glrenderer = class_<GLRenderer, bases<Action>, boost::noncopyable>
          ("GLRenderer", init<Discretizer &>(
                  "GLRenderer(Discretizer d [, QGLWidget *]) An action which draws objects of type of Geometry or of type of Material to the current GL context."))
          .def("clear", &GLRenderer::clear)
          .def("beginSceneList", &GLRenderer::beginSceneList)
          .def("endSceneList", &GLRenderer::endSceneList)
          .def("clearSceneList", &GLRenderer::clearSceneList)
#ifndef PGL_WITHOUT_QT
          .def("setGLFrame", &py_setGLFrame)
          .def("setGLFrameFromId", &GLRenderer::setGLFrameFromId)
#endif
          .add_property("renderingMode", &get_rd_mode, &GLRenderer::setRenderingMode)
          .add_property("selectionMode", &get_sel_mode, &GLRenderer::setSelectionMode)
                  // .add_property("frameGL",&get_fgl_mode,&GLRenderer::setGLFrame)
          .def("getDiscretizer", &GLRenderer::getDiscretizer, return_internal_reference<>())
          .def("registerTexture", &GLRenderer::registerTexture,
               (bp::arg("texture"), bp::arg("id"), bp::arg("erasePreviousIfExists") = true))
          .def("getTextureId", &GLRenderer::getTextureId);

  enum_<GLRenderer::RenderingMode>("RenderingMode")
          .value("Normal", GLRenderer::Normal)
          .value("Selection", GLRenderer::Selection)
          .value("DynamicPrimitive", GLRenderer::DynamicPrimitive)
          .value("DynamicScene", GLRenderer::DynamicScene)
          .value("Dynamic", GLRenderer::Dynamic)
          .export_values();

  enum_<GLRenderer::SelectionId>("SelectionId")
          .value("ShapeId", GLRenderer::ShapeId)
          .value("SceneObjectId", GLRenderer::SceneObjectId)
          .value("PrimitiveId", GLRenderer::PrimitiveId)
          .value("ShapeNPrimitiveIds", GLRenderer::ShapeNPrimitiveIds)
          .value("SceneObjectNPrimitive", GLRenderer::SceneObjectNPrimitive)
          .export_values();

}

void export_GLSkelRenderer() {
  class_<GLSkelRenderer, bases<GLRenderer>, boost::noncopyable>
          ("GLSkelRenderer",
           init<SkelComputer &>("GLSkelRenderer(SkelComputer s) An action which displays skeletons of shapes."));
}

void export_GLBBoxRenderer() {
  class_<GLBBoxRenderer, bases<GLRenderer>, boost::noncopyable>
          ("GLBBoxRenderer",
           init<BBoxComputer &>("GLBBoxRenderer(BBoxComputer b) An action which displays bounding boxes of shapes."));
}

AppearancePtr get_default_app() { return GLCtrlPointRenderer::DEFAULT_APPEARANCE; }

void set_default_app(AppearancePtr p) { GLCtrlPointRenderer::DEFAULT_APPEARANCE = p; }

void export_GLCtrlPointRenderer() {
  class_<GLCtrlPointRenderer, bases<GLRenderer>, boost::noncopyable>
          ("GLCtrlPointRenderer", init<Discretizer &>(
                  "GLCtrlPointRenderer(Discretizer d) An action which display the Control Points of Geometry objects."))
          .add_static_property("DEFAULT_APPEARANCE", &get_default_app, &set_default_app);
}

/* ----------------------------------------------------------------------- */
