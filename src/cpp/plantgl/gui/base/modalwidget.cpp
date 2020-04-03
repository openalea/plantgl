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


#include "modalwidget.h"
#include "modalscenegl.h"
#include "light.h"
#include "icons.h"

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qtoolbar.h>
#else
    #include <QtGui/qmenu.h>
    #include <QtGui/qtoolbar.h>
#endif

ViewRenderingModeActions::ViewRenderingModeActions(ViewModalRendererGL * renderer, const char * name):
  QObject(renderer)
{
  if(name)setObjectName(name);
  if(renderer){
    QPixmap volume(ViewerIcon::getPixmap(ViewerIcon::solid));
    QPixmap wire(ViewerIcon::getPixmap(ViewerIcon::wire));
    QPixmap skeleton(ViewerIcon::getPixmap(ViewerIcon::skeleton));
    QPixmap ctrlpoint(ViewerIcon::getPixmap(ViewerIcon::ctrlpoint));
    QPixmap bbox(ViewerIcon::getPixmap(ViewerIcon::bbox));

    QActionGroup * mActionGroup = new QActionGroup(this);
    mActionGroup->setExclusive(true);

    idVolume   = new QAction(volume,  tr("&Volume"), this);
    idVolume->setShortcut(Qt::CTRL+Qt::Key_M);
    QObject::connect(idVolume,SIGNAL(triggered()), renderer, SLOT(setRenderVolume()));
    idVolume->setCheckable(true);
    idVolume->setWhatsThis(tr("<b>Volume Rendering</b><br><br>"
    "Change the Rendering Mode to <b>Volume</b>.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > Volume</b><br>"));

    idWire     = new QAction(wire,    tr("&Wire"), this);
    idWire->setShortcut(Qt::CTRL+Qt::Key_W);
    QObject::connect(idWire,SIGNAL(triggered()), renderer, SLOT(setRenderWire()));
    idWire->setCheckable(true);
    idWire->setWhatsThis(tr("<b>Wire Rendering</b><br><br>"
    "Change the Rendering Mode to <b>Wire</b>.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > Wire</b><br>"));

    idSkeleton = new QAction(skeleton,tr("S&keleton"),  this);
    idWire->setShortcut(Qt::CTRL+Qt::Key_K);
    QObject::connect(idSkeleton,SIGNAL(triggered()), renderer, SLOT(setRenderSkeleton()));
    idSkeleton->setCheckable(true);
    idSkeleton->setWhatsThis(tr("<b>Skeleton Rendering</b><br><br>"
    "Change the Rendering Mode to <b>Skeleton</b>.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > Skeleton</b><br>"));

    idVolWire =  new QAction(volume,  tr("Volu&me and Wire"),   this);
    QObject::connect(idVolWire,SIGNAL(triggered()), renderer, SLOT(setRenderVolumenWire()));
    idVolWire->setCheckable(true);
    idVolWire->setWhatsThis(tr("<b>Volume and Wire Rendering</b><br><br>"
    "Change the Rendering Mode to <b>Volume and Wire</b>.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > Volume and Wire</b><br>"));

    mActionGroup->addAction(idVolume);
    mActionGroup->addAction(idWire);
    mActionGroup->addAction(idSkeleton);
    mActionGroup->addAction(idVolWire);

    idCtrlPoints = new QAction(ctrlpoint, tr("&Control Points"),this);
    QObject::connect(idCtrlPoints,SIGNAL(triggered()), renderer, SLOT(setRenderCtrlPoint()));
    idCtrlPoints->setCheckable(true);
    idCtrlPoints->setChecked(renderer->isCtrlPointRenderingEnable());
    idCtrlPoints->setWhatsThis(tr("<b>Control Points Rendering</b><br><br>"
    "Set <b>Control Points Rendering</b> enable/disable.<br><br>"
    "All the shapes define with some control points will display them.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > Control Points</b><br>"));

    idBBox       = new QAction(bbox,      tr("&Bounding Box"),this);
    QObject::connect(idBBox,SIGNAL(triggered()), renderer, SLOT(setRenderBBox()));
    idBBox->setCheckable(true);
    idBBox->setChecked(renderer->isBBoxRenderingEnable());
    idBBox->setWhatsThis(tr("<b>Bounding Box Rendering</b><br><br>"
    "Set <b>Bounding Box Rendering</b> enable/disable.<br><br>"
    "the Bounding Boxes of all shapes will be displayed.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > BBox</b><br>"));


    QObject::connect(renderer,SIGNAL(ctrlPointsRenderingChanged(bool)),idCtrlPoints,SLOT(setChecked(bool)));
    QObject::connect(renderer,SIGNAL(bboxRenderingChanged(bool)),idBBox,SLOT(setChecked(bool)));

    setRenderingMode(renderer->getRenderingMode());
    QObject::connect(renderer,SIGNAL(renderingModeChanged(const int)),
             this,SLOT(setRenderingMode(const int)));
  }
}

ViewRenderingModeActions::~ViewRenderingModeActions()
{
}


void ViewRenderingModeActions::fill(QMenu * menu) const
{
    menu->addAction(idVolume);
    menu->addAction(idWire);
    menu->addAction(idSkeleton);
    menu->addAction(idVolWire);
    menu->addSeparator();
    menu->addAction(idCtrlPoints);
    menu->addAction(idBBox);
}

void ViewRenderingModeActions::fill(QToolBar * bar) const
{
    bar->addAction(idVolume);
    bar->addAction(idWire);
    bar->addAction(idVolWire);
}

void ViewRenderingModeActions::fillAdditionnal(QToolBar * bar) const
{
    bar->addAction(idSkeleton);
    bar->addAction(idCtrlPoints);
    bar->addAction(idBBox);
}

void
ViewRenderingModeActions::setRenderingMode(const int i)
{
  idVolume->setChecked(i == 1);
  idWire->setChecked(i == 2);
  idSkeleton->setChecked(i == 3);
  idVolWire->setChecked(i == 4);
}

