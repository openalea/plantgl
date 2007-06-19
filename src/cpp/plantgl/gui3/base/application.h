/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
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

/*! \file view_application.h
    \brief Definition of the viewer class ViewerApplication3.
*/

#ifndef __view_application_h__
#define __view_application_h__

/* ----------------------------------------------------------------------- */

#include "../gui3_config.h"
#include <plantgl/math/util_vector.h>
#include <vector>
#include <string>

/* ----------------------------------------------------------------------- */


/**
      \class ViewerApplication3
      \brief This class manage thread or not application for the viewer

*/

/* ----------------------------------------------------------------------- */

class ViewRayBuffer3;
class ViewZBuffer3;
class QCustomEvent;
class ViewerBuilder3;


class VIEW3_API ViewerApplication3 {

public :

  /// Constructor.
  ViewerApplication3();

  /// Destructor.
  virtual ~ViewerApplication3();

  /// Return whether the viewer process is a multi-thread process.
  static bool isThreadUsed() ;

  /// Set use thread or not. Return whether it is possible or not.
  static bool useThread(bool);

  static void setBashMode(bool);

  static void update();

  static void saveImage( const std::string& filename, 
					     const std::string& format = "PNG",
						 bool  withAlpha = false );

  static ViewRayBuffer3 * castRays(const TOOLS(Vector3)& pos, 
	                               const TOOLS(Vector3)& dir,
								   const TOOLS(Vector3)& dx, 
								   const TOOLS(Vector3)& dy,
								   int sx, int sy);

  static ViewZBuffer3 * grabZBuffer();

  static int question(const std::string& caption,
					   const std::string& text,
					   const std::string& but0txt = "",
					   const std::string& but1txt = "",
					   const std::string& but2txt = "");

  static std::string itemSelection(const std::string& caption,
					   const std::string& text,
					   const std::vector<std::string> & values,
					   bool& ok,
					   bool editable = false);

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

  static void animation(bool b = true);
  static void setBgColor(int red, int green, int blue);

  /// Get the selection.
  static const std::vector<uint32_t> getSelection();
  static void setSelection(const std::vector<uint32_t>&);

  /// wait for viewer to finish for time milliseconds. Return true if viewer finished.
  static bool wait( unsigned long time = ULONG_MAX);

  /// return true if viewer is running.
  static bool running();

  /// Stop the Viewer process.
  static bool stop();
  static void start();
  static void exit();

  static void setCamera(const TOOLS(Vector3)& pos);
  static void setCamera(const TOOLS(Vector3)& pos, real_t azimuth, real_t elevation);
  static void lookAt(const TOOLS(Vector3)& pos, const TOOLS(Vector3)& target);
  static void lookAt(const TOOLS(Vector3)& target);

  static void setXYGrid(bool);
  static void setXZGrid(bool);
  static void setYZGrid(bool);
  static void setAxis(bool);
  static void setGridUnit(int);
  static void setGridSize(int);
  static void setGrid(bool, bool, bool, bool);

  static void glFrameSize(int,int);
  static void resize(int,int);
  static void move(int,int);
  static void setGeometry(int,int,int,int);

  static double getProjectionSize(int* nbpixel = NULL, double* pixelwidth = NULL); 

  static void setPerspectiveCamera(); 
  static void setOrthographicCamera(); 

protected :

	static void _sendAnEvent(QCustomEvent *);
	static void _setViewerBuilder(ViewerBuilder3 * builder);

};

/* ----------------------------------------------------------------------- */
#endif

