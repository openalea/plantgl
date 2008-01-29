/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
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

/*! \file view_event.h
    \brief Definition of the viewer class events.
*/

#ifndef __view_event_h__
#define __view_event_h__

/* ----------------------------------------------------------------------- */

#include <qevent.h>
#include <qcolor.h>
#include <qstringlist.h>
#include <vector>
#include "../gui3_config.h"
#include <plantgl/math/util_vector.h>

#ifdef QT_THREAD_SUPPORT
#include <qthread.h>
#endif

TOOLS_USING(Vector3)

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewEvent3 : public QCustomEvent {
public:

  ViewEvent3(int type):
	QCustomEvent(type),
	sent_event(false){}

  virtual ~ViewEvent3(){}

  bool sent_event;

};
/**
   \class ViewSceneChangeEvent3
   \brief An event to pass to glframe for changing scene.

*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewSceneChangeEvent3 : public ViewEvent3 {

  public :

  /// Constructor.
  ViewSceneChangeEvent3(int type = 0);

  /// Destructor.
  ~ViewSceneChangeEvent3();

  /// copy object.
  virtual ViewSceneChangeEvent3 * copy();

  const int& getSceneType() const;

protected:
  void setSceneType(const int& i);

private:

  int scene_type;
};

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewFileChangeEvent3 : public ViewEvent3 {

  public :

  /// Constructor.
  ViewFileChangeEvent3(const QString& file);

  /// Destructor.
  ~ViewFileChangeEvent3();

  /// new file to parse.
  QString filename;

};

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewImageSaveEvent3 : public ViewEvent3 {

  public :

  /// Constructor.
  ViewImageSaveEvent3(const QString& file,const QString& format, bool withAlpha);

  /// Destructor.
  ~ViewImageSaveEvent3();

  QString filename;
  QString format;
  bool withAlpha;
};

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewRefreshEvent3 : public ViewEvent3 {

  public :

  /// Constructor.
  ViewRefreshEvent3();

  /// Destructor.
  ~ViewRefreshEvent3();

};

/* ----------------------------------------------------------------------- */



/**
   \class ViewSelectRecoverEvent3
   \brief An event to get selection from the viewer. Usefull for inter thread communication.
*/
class ViewSelectRecoverEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewSelectRecoverEvent3();

  /// Constructor.
  ViewSelectRecoverEvent3(std::vector<uint_t> * e);

/// Destructor.
  ~ViewSelectRecoverEvent3();

  /// set the selection (for the viewer).
  void setSelection(const std::vector<uint_t>& a);

//private :

  /// Data Selection exchange beetwen the 2 process.
  std::vector<uint_t> * __exchange;

};

class ViewSelectionSet3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewSelectionSet3(const std::vector<uint_t>&);

  /// Destructor.
  ~ViewSelectionSet3();

  /// set the selection (for the viewer).
  const std::vector<uint_t>& getSelection() const;

protected :

  /// Data Selection exchange beetwen the 2 process.
  const std::vector<uint_t> __data;

};

/* ----------------------------------------------------------------------- */

/**
   \class ViewEndEvent3
   \brief An event to end the viewer. Usefull for inter thread communication.

*/
class ViewEndEvent3 : public ViewEvent3 {

    public :

  /// Constructor.
  ViewEndEvent3();

  /// Destructor.
  ~ViewEndEvent3();

};

/* ----------------------------------------------------------------------- */

/**
   \class ViewFullScreenEvent3
   \brief An event to set the viewer full screen. Usefull for inter thread communication.

*/
class ViewFullScreenEvent3 : public ViewEvent3 {

    public :

  /// Constructor.
  ViewFullScreenEvent3(bool b = true);

  /// Destructor.
  ~ViewFullScreenEvent3();

  bool value;
};

/* ----------------------------------------------------------------------- */

/**
   \class ViewGLFrameOnlyEvent3
   \brief An event to display GL Frame Only. Usefull for inter thread communication.

*/
class ViewGLFrameOnlyEvent3 : public ViewEvent3 {

    public :

  /// Constructor.
  ViewGLFrameOnlyEvent3(bool b = true);

  /// Destructor.
  ~ViewGLFrameOnlyEvent3();

  bool value;
};

/* ----------------------------------------------------------------------- */


/**
   \class ViewQuestionEvent3
   \brief An event to end the viewer. Usefull for inter thread communication.

*/
class ViewQuestionEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewQuestionEvent3();
  ViewQuestionEvent3(const QString& _caption,
				 const QString& _text,
				 const QString& _but0txt = QString::null,
				 const QString& _but1txt = QString::null,
				 const QString& _but2txt = QString::null,
				 int * result = NULL);

  /// Destructor.
  ~ViewQuestionEvent3();

  int * result;

  QString caption;
  QString text;
  QString but0txt;
  QString but1txt;
  QString but2txt;

};

/* ----------------------------------------------------------------------- */


/**
   \class ViewQuestionEvent3
   \brief An event to end the viewer. Usefull for inter thread communication.

*/
class ViewItemSelectionEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewItemSelectionEvent3(const QString& _caption,
				 const QString& _text,
				 const QStringList& _values,
				 bool _editable = false,
				 QString * res = NULL,
				 bool * ok = NULL);

  /// Destructor.
  ~ViewItemSelectionEvent3();

  QString * result;
  bool * ok;

  QString caption;
  QString text;
  QStringList values;
  bool editable;

};

/* ----------------------------------------------------------------------- */

class ViewAnimationEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewAnimationEvent3(bool a);

  /// Destructor.
  ~ViewAnimationEvent3();

  bool mode;
};

/* ----------------------------------------------------------------------- */

class ViewBgColorEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewBgColorEvent3(const QColor& c);

  /// Destructor.
  ~ViewBgColorEvent3();

  QColor color;
};

/* ----------------------------------------------------------------------- */

class ViewGridEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewGridEvent3(bool _xy, bool _yz, bool _xz, 
				bool _axis, int _size, int _unit, int _def);

  /// Destructor.
  ~ViewGridEvent3();

  bool xy;
  bool yz;
  bool xz;
  bool axis;
  int size;
  int unit;
  int def;
};

/* ----------------------------------------------------------------------- */

class ViewCameraEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewCameraEvent3(const Vector3& pos, const Vector3& target,
			      float azimuth, float elevation, int def);

  /// Destructor.
  ~ViewCameraEvent3();

  Vector3 pos;
  Vector3 target;
  float azimuth;
  float elevation;
  int def;
};

/* ----------------------------------------------------------------------- */

class ViewPosEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewPosEvent3(int _x, int _y, int _w, int _h, int _def);

  /// Destructor.
  ~ViewPosEvent3();

  int x;
  int y;
  int w;
  int h;
  int def;
};

/* ----------------------------------------------------------------------- */

class ViewFileSelEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewFileSelEvent3(const QString& caption,
				   const QString& startPath,
				   const QString& filter = QString::null,
				   bool existing = true, 
				   bool dir = true,
				   QString * res = NULL);

  /// Destructor.
  ~ViewFileSelEvent3();

  QString *  result;
  QString caption;
  QString startPath;
  QString filter;
  bool existing;
  bool dir;
};

/* ----------------------------------------------------------------------- */
class ViewRayBuffer3;

class ViewRayBuffEvent3 : public ViewEvent3 {

public :

  /// Constructor.
	ViewRayBuffEvent3(const TOOLS(Vector3)& pos,
				   const TOOLS(Vector3)& dir,
		           const TOOLS(Vector3)& dx, 
		           const TOOLS(Vector3)& dy,
		           int sx, int sy,
				   ViewRayBuffer3 ** res = NULL);
	
	/// Destructor.
	~ViewRayBuffEvent3();
	
	ViewRayBuffer3 **  result;
	const TOOLS(Vector3) pos;
	const TOOLS(Vector3) dir;
	const TOOLS(Vector3) dx; 
	const TOOLS(Vector3) dy;
    int sx;
	int sy;
};

/* ----------------------------------------------------------------------- */
class ViewZBuffer3;

class ViewZBuffEvent3 : public ViewEvent3 {

public :

  /// Constructor.
	ViewZBuffEvent3(ViewZBuffer3 ** res = NULL);
	
	/// Destructor.
	~ViewZBuffEvent3();
	
	ViewZBuffer3 **  result;
};

/* ----------------------------------------------------------------------- */

class ViewProjSizeEvent3 : public ViewEvent3 {

public :

  /// Constructor.
	ViewProjSizeEvent3 (double * _size = NULL, int * _nbpix = NULL, double * _pixwidth = NULL);
	
	/// Destructor.
	~ViewProjSizeEvent3();
	
    int * nbpixel;
	double * pixelwidth;
	double * size;
};

/* ----------------------------------------------------------------------- */

class ViewCameraProjEvent3 : public ViewEvent3 {

public :

  /// Constructor.
  ViewCameraProjEvent3(bool _mode);

  /// Destructor.
  ~ViewCameraProjEvent3();

  bool mode;
};

/* ----------------------------------------------------------------------- */

#endif

