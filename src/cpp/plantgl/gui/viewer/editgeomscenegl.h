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


/*! \file view_editgeomscenegl.h
    \brief Definition of the viewer class ViewEditGeomSceneGL and ViewMultiGeomSceneGL.
*/

#ifndef __view_editgeomscenegl_h__
#define __view_editgeomscenegl_h__

/* ----------------------------------------------------------------------- */

#include "geomscenegl.h"
#include "../base/util_qwidget.h"
#include <plantgl/scenegraph/appearance/material.h>


class MaterialEditor;
class QPushButton;

class VIEW_API  ViewEditMatDialog : public ViewDialog {
  Q_OBJECT

public :
  ViewEditMatDialog(QWidget * parent=0,
                    const char * name=0,
                    bool modal=false,
                    Qt::WindowFlags f = Qt::Widget);
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
  QPushButton * __clipboardButton;
};

PGL(MaterialPtr) VIEW_API getMaterialFromDialog(QWidget * parent = NULL,
                                                const char * caption = "",
                                                PGL(MaterialPtr) initial = PGL(MaterialPtr()));

int VIEW_API editMaterialInDialog(PGL(MaterialPtr) initial,
                                   QWidget * parent = NULL,
                                   const char * caption = "");

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
               QOpenGLBaseWidget * parent=0,
               const char * name=0, 
               PGLOpenGLFunctionsPtr ogl = NULL);

  /// Destructor.
  virtual ~ViewEditGeomSceneGL();

  virtual void clear();

  bool hasSameMaterial() const;
  bool hasSameMaterial(PGL(ScenePtr),PGL(AppearancePtr)) const;
  PGL(AppearancePtr) getSelectedAppearance() const;
  PGL(AppearancePtr) getSelectedAppearance(PGL(ScenePtr)) const;
  /// Add Edit Entries
  virtual bool addEditEntries(QMenu * menu);
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
namespace Ui {
class ViewApproximationForm;
}

class VIEW_API  ViewMultiscaleEditGeomSceneGL : public ViewEditGeomSceneGL
{
  Q_OBJECT

    public :


  /// Constructor.
  ViewMultiscaleEditGeomSceneGL(ViewCameraGL * camera=0,
               ViewLightGL * light=0,
               QOpenGLBaseWidget * parent=0,
               const char * name=0, 
               PGLOpenGLFunctionsPtr ogl = NULL);

  /// Destructor.
  virtual ~ViewMultiscaleEditGeomSceneGL();

  /// Add Edit Entries
  virtual bool addEditEntries(QMenu * menu);

public slots:

  void editMacroMaterial();
  void editMultiScaleGeometry();
  void computeMultiScaleGeometry();
  virtual void applyEdition();
  void updateMaterial(const QColor& col);

protected:

  QDialog * __appdialog;
  Ui::ViewApproximationForm * __appform;

  /// The clipboard for appearance.
  PGL(MaterialPtr) __matmacro;
};

/* ----------------------------------------------------------------------- */
#endif

