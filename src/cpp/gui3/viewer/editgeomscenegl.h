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
    \brief Definition of the viewer class ViewEditGeomSceneGL and ViewMultiGeomSceneGL.
*/

#ifndef __view_editgeomscenegl_h__
#define __view_editgeomscenegl_h__

/* ----------------------------------------------------------------------- */

#include "geomscenegl.h"
#include "../base/util_qwidget.h"
#include <scenegraph/appearance/material.h>


class MaterialEditor;

class VIEW_API  ViewEditMatDialog : public ViewDialog {
  Q_OBJECT

public :
  ViewEditMatDialog(QWidget * parent=0, 
					const char * name=0, 
					bool modal=FALSE, 
					WFlags f=0);
  ~ViewEditMatDialog(){};

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

  MaterialEditor * __matedit;
  PGL(MaterialPtr) __appe;
  PGL(MaterialPtr) __default;
  PGL(AppearancePtr)* __clipboard;
};


/* ----------------------------------------------------------------------- */

/**   
   \class ViewMultiGeomSceneGL
   \brief A GL Display Manager for Geom Scene that can edit material.

*/
class VIEW_API  ViewEditGeomSceneGL : public ViewMultiGeomSceneGL
{
  Q_OBJECT

  friend class ViewEditMatDialog;

  public :

  
  /// Constructor.
  ViewEditGeomSceneGL(ViewCameraGL * camera=0,
		       ViewLightGL * light=0,
		       QGLWidget * parent=0, 
		       const char * name=0);

  /// Destructor.
  virtual ~ViewEditGeomSceneGL();

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
  ViewEditMatDialog * __mateditor;

};

/* ----------------------------------------------------------------------- */
class ViewApproximationForm;

class VIEW_API  ViewMultiscaleEditGeomSceneGL : public ViewEditGeomSceneGL
{
  Q_OBJECT

    public :

  
  /// Constructor.
  ViewMultiscaleEditGeomSceneGL(ViewCameraGL * camera=0,
		       ViewLightGL * light=0,
		       QGLWidget * parent=0, 
		       const char * name=0);

  /// Destructor.
  virtual ~ViewMultiscaleEditGeomSceneGL();

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

