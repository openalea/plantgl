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

/*! \file view_editgeomscenegl.h
    \brief Definition of the viewer class ViewEditGeomSceneGL3 and ViewMultiGeomSceneGL3.
*/

#ifndef __view_editgeomscenegl_h__
#define __view_editgeomscenegl_h__

/* ----------------------------------------------------------------------- */

#include "geomscenegl.h"
#include "../base/util_qwidget.h"
#include <plantgl/scenegraph/appearance/material.h>


class MaterialEditor3;

class VIEW3_API  ViewEditMatDialog3 : public ViewDialog3 {
  Q_OBJECT

public :
  ViewEditMatDialog3(QWidget * parent=0, 
					const char * name=0, 
					bool modal=FALSE, 
					WFlags f=0);
  ~ViewEditMatDialog3(){};

  void setMaterial(PGL(MaterialPtr) appe);
  void setClipboardMaterial(PGL(AppearancePtr)* clipboard);
  const PGL(MaterialPtr)& getMaterial() const
  { return __appe; }

signals:
  void valueChanged();

public slots:

  void importClipboard();
  void apply();
  void reset();
  void ok();
  void cancel();

protected:

  MaterialEditor3 * __matedit;
  PGL(MaterialPtr) __appe;
  PGL(MaterialPtr) __default;
  PGL(AppearancePtr)* __clipboard;
};


/* ----------------------------------------------------------------------- */

/**   
   \class ViewMultiGeomSceneGL3
   \brief A GL Display Manager for Geom Scene that can edit material.

*/
class VIEW3_API  ViewEditGeomSceneGL3 : public ViewMultiGeomSceneGL3
{
  Q_OBJECT

  friend class ViewEditMatDialog3;

  public :

  
  /// Constructor.
  ViewEditGeomSceneGL3(ViewCameraGL3 * camera=0,
		       ViewLightGL3 * light=0,
		       QGLWidget * parent=0, 
		       const char * name=0);

  /// Destructor.
  virtual ~ViewEditGeomSceneGL3();

  virtual void clear();

  bool hasSameMaterial() const;
  bool hasSameMaterial(PGL(ScenePtr),PGL(AppearancePtr)) const;
  PGL(AppearancePtr) getSelectedAppearance() const;
  PGL(AppearancePtr) getSelectedAppearance(PGL(ScenePtr)) const;
  /// Add Edit Entries
  virtual bool addEditEntries(QPopupMenu * menu);
  void setAppearance(PGL(ScenePtr) scene,PGL(AppearancePtr) appe) const;

public slots:
  void editMaterial();
  virtual void applyEdition();
  void dissociateMaterial();
  void copyMaterial();
  void pasteMaterial();


protected:

  /// The clipboard for appearance.
  PGL(AppearancePtr) __appeclipboard;
  ViewEditMatDialog3 * __mateditor;

};

/* ----------------------------------------------------------------------- */
class ViewApproximationForm;

class VIEW3_API  ViewMultiscaleEditGeomSceneGL3 : public ViewEditGeomSceneGL3
{
  Q_OBJECT

    public :

  
  /// Constructor.
  ViewMultiscaleEditGeomSceneGL3(ViewCameraGL3 * camera=0,
		       ViewLightGL3 * light=0,
		       QGLWidget * parent=0, 
		       const char * name=0);

  /// Destructor.
  virtual ~ViewMultiscaleEditGeomSceneGL3();

  /// Add Edit Entries
  virtual bool addEditEntries(QPopupMenu * menu);

public slots:

  void editMacroMaterial();
  void editMultiScaleGeometry();
  void computeMultiScaleGeometry();
  virtual void applyEdition();
  void updateMaterial(const QColor& col);

protected:

  ViewApproximationForm * __appform;

  /// The clipboard for appearance.
  PGL(MaterialPtr) __matmacro;
};

/* ----------------------------------------------------------------------- */
#endif

