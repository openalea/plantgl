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

#include "view_geomscenegl.h"
#include "util_qwidget.h"
#include "appe_material.h"


class MaterialEditor;

class GEOM_API  ViewEditMatDialog : public ViewDialog {
  Q_OBJECT

public :
  ViewEditMatDialog(QWidget * parent=0, 
					const char * name=0, 
					bool modal=FALSE, 
					WFlags f=0);
  ~ViewEditMatDialog(){};

  void setMaterial(GEOM(MaterialPtr) appe);
  void setClipboardMaterial(GEOM(AppearancePtr)* clipboard);
  const GEOM(MaterialPtr)& getMaterial() const
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
  GEOM(MaterialPtr) __appe;
  GEOM(MaterialPtr) __default;
  GEOM(AppearancePtr)* __clipboard;
};


/* ----------------------------------------------------------------------- */

/**   
   \class ViewMultiGeomSceneGL
   \brief A GL Display Manager for Geom Scene that can edit material.

*/
class GEOM_API  ViewEditGeomSceneGL : public ViewMultiGeomSceneGL
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
  bool hasSameMaterial(GEOM(ScenePtr),GEOM(AppearancePtr)) const;
  GEOM(AppearancePtr) getSelectedAppearance() const;
  GEOM(AppearancePtr) getSelectedAppearance(GEOM(ScenePtr)) const;
  /// Add Edit Entries
  virtual bool addEditEntries(QPopupMenu * menu);
  void setAppearance(GEOM(ScenePtr) scene,GEOM(AppearancePtr) appe) const;

public slots:
  void editMaterial();
  virtual void applyEdition();
  void dissociateMaterial();
  void copyMaterial();
  void pasteMaterial();


protected:

  /// The clipboard for appearance.
  GEOM(AppearancePtr) __appeclipboard;
  ViewEditMatDialog * __mateditor;

};

/* ----------------------------------------------------------------------- */
class ViewApproximationForm;

class GEOM_API  ViewMultiscaleEditGeomSceneGL : public ViewEditGeomSceneGL
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
  GEOM(MaterialPtr) __matmacro;
};

/* ----------------------------------------------------------------------- */
#endif

