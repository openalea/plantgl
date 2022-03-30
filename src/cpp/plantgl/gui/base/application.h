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


/*! \file view_application.h
    \brief Definition of the viewer class ViewerApplication.
*/

#ifndef __view_application_h__
#define __view_application_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include "appbuilder.h"
#include "flags.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <vector>
#include <string>

/* ----------------------------------------------------------------------- */


/**
      \class ViewerApplication
      \brief This class manage thread or not application for the viewer

*/

/* ----------------------------------------------------------------------- */

class ViewRayBuffer;
class ViewZBuffer;
class QEvent;
class ViewerBuilder;
class ThreadStateSaver;

class VIEW_API ViewerApplication {

public :

  /// Constructor.
  ViewerApplication();

  /// Destructor.
  virtual ~ViewerApplication();

  /// Return whether the viewer process is a multi-thread process.
  static bool isThreadUsed() ;

  /// Set use thread or not. Return whether it is possible or not.
  static bool useThread(bool);

  static void setBatchMode(bool);

  static void update();

  static void saveImage( const std::string& filename,
                         const std::string& format = "PNG",
                         bool  withAlpha = false );

  static ViewRayBuffer * castRays(const PGL(Vector3)& pos,
                                   const PGL(Vector3)& dir,
                                   const PGL(Vector3)& dx,
                                   const PGL(Vector3)& dy,
                                   int sx, int sy);

  static ViewZBuffer * grabZBuffer();

  static std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> grabZBufferPoints();
  static std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> grabZBufferPointsWithJitter(float jitter, int raywidth, bool mixcolor = true);

  static void showMessage(const std::string& caption, int timeout = 0);

  static int question(const std::string& caption,
                       const std::string& text,
                       const std::string& but0txt = "",
                       const std::string& but1txt = "",
                       const std::string& but2txt = "");
  typedef bool(*AbortFunc)();
  static void setAborter(AbortFunc);

  static std::string itemSelection(const std::string& caption,
                       const std::string& text,
                       const std::vector<std::string> & values,
                       bool& ok,
                       bool editable = false);

  static double doubleSelection(const std::string& caption,
                                   const std::string& text,
                                   double value,
                                   double minvalue,
                                   double maxvalue,
                                   bool& ok);

  static std::string getOpenFile(const std::string& caption,
                                 const std::string& filter,
                                 const std::string& startPath);

  static std::string getSaveFile(const std::string& caption,
                                 const std::string& filter,
                                 const std::string& startPath);

  static std::string getDirectory(const std::string& caption,
                                  const std::string& startPath);

  static void fullScreen(bool b = true);
  static void glFrameOnly(bool b = true);

  static void setAnimation(eAnimationFlag b = eStatic);

  // previous deprecated version
  attribute_deprecated inline static void animation(bool b = true)
  { setAnimation(b?eAnimatedPrimitives:eStatic); }

  static void setBgColor(int red, int green, int blue);

  /// Get the selection.
  static std::vector<uint_t> getSelection();
  static void setSelection(const std::vector<uint_t>&);
  static uint_t waitSelection(const std::string& caption = "");

  /// Get the redraw mode.
  static bool getRedrawPolicy();
  static void setRedrawPolicy(bool);

  /// wait for viewer to finish for time milliseconds. Return true if viewer finished.
  static bool wait( unsigned long time = ULONG_MAX);

  /// return true if viewer is running.
  static bool running();

  /// Stop the Viewer process.
  static bool stop();
  static void start();
  static void exit();

  static void setCamera(const PGL(Vector3)& pos);
  static void setCamera(const PGL(Vector3)& pos, real_t azimuth, real_t elevation);
  static void lookAt(const PGL(Vector3)& pos, const PGL(Vector3)& target);
  static void lookAt(const PGL(Vector3)& target);

  static void getCamera(PGL(Vector3)& pos, PGL(Vector3)& heading, PGL(Vector3)& up);

  static void setViewAngle(real_t angle);
  static real_t getViewAngle();

  static void setLightEnabled(bool);
  static bool isLightEnabled();

  static void setLightPosition(const PGL(Vector3)& position);
  static PGL(Vector3) getLightPosition();

  static void setLightAmbient(int red, int green, int blue);
  static void getLightAmbient(int& red, int& green, int& blue);

  static void setLightDiffuse(int red, int green, int blue);
  static void getLightDiffuse(int& red, int& green, int& blue);

  static void setLightSpecular(int red, int green, int blue);
  static void getLightSpecular(int& red, int& green, int& blue);

  static void setXYGrid(bool);
  static void setXZGrid(bool);
  static void setYZGrid(bool);
  static void setAxis(bool);
  static void setGridUnit(int);
  static void setGridSize(int);
  static void setGrid(bool, bool, bool, bool);

  static void activateCP(int, bool);
  static void setCP(int, double, double, double, double);

  static void glFrameSize(int,int);
  static void resize(int,int);
  static void move(int,int);
  static void setGeometry(int,int,int,int);

  static double getProjectionSize(int* nbpixel = NULL, double* pixelwidth = NULL);

  static void setPerspectiveCamera();
  static void setOrthographicCamera();

  template<class T>
  static void registerThreadStateSaver()
  { registerThreadStateSaverFatory(new ThreadStateSaverTFactory<T>()); }

  static void registerThreadStateSaverFatory(ThreadStateSaverFactory * tssf) ;
  static void cleanThreadStateSaverFatory() ;

  static unsigned int viewerId();

protected :

    static void _sendAnEvent(QEvent *);
    static void _setViewerBuilder(ViewerBuilder * builder);

};

/* ----------------------------------------------------------------------- */
#endif

