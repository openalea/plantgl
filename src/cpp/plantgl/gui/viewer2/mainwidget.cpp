/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"

#include <QMouseEvent>
#include <QMenuBar>
#include <QtDebug>
#include <QFileDialog>

#include <math.h>

MainWidgetGL::MainWidgetGL(QWidget *parent) :
    QOpenGLWidget(parent),
    // texture(0),
    angularSpeed(0),
    zoom(5),
    center(0,0,0),
    renderer(d, t)

{

}

MainWidgetGL::~MainWidgetGL()
{
    clear();
}


void MainWidgetGL::setScene(const ScenePtr& _scene)
{
    clear();
    scene = _scene;
    update();
}

void MainWidgetGL::clear()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    renderer.clear();
    doneCurrent();
}

//! [0]
void MainWidgetGL::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidgetGL::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(0.0, -diff.y(), diff.x()).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

void MainWidgetGL::wheelEvent(QWheelEvent  *e) 
{
    zoom = qMax<qreal>(0,zoom + e->angleDelta().y()/120.);
    printf("zoom: %f\n",zoom);

    update();
}


//! [1]
void MainWidgetGL::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}
//! [1]

void MainWidgetGL::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.1, 0.1, 0.1, 1);

    //initShaders();
    // initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);

    renderer.init();
}


void MainWidgetGL::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = 0.01, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidgetGL::paintGL()
{
    static int frame = 0;

    if(frame ==0) {
        QString versionString(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
        qDebug() << "Driver Version String:" << versionString;
        qDebug() << "Current Context:" << QOpenGLContext::currentContext()->format();

    }

    qDebug() << Q_FUNC_INFO << "Rendering frame" << frame++;

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    QMatrix4x4 view;
    view.lookAt(center+QVector3D(-zoom,0,0), center, QVector3D(0,0,1));
    view.rotate(rotation);

    // QMatrix4x4 m = projection * view;

    /*
    for (QMatrix4x4 m : {projection, view})
        for(int i = 0; i < 4; i++)
            qDebug() << Q_FUNC_INFO
                     << "m" << m(i,0) << m(i,1) << m(i,2) << m(i,3);
    */

    renderer.setViewProjection(view, projection);

    scene->apply(renderer);

}

MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent),
    filename("")
    {
        mainwidgetgl = new MainWidgetGL(this);
        setCentralWidget(mainwidgetgl);
        QMenuBar * menu = new QMenuBar(this);
        QMenu * filemenu = new QMenu("File");
        QAction * open = filemenu->addAction("Open", this, SLOT(openFile()));
        menu->addMenu(filemenu);
        setMenuBar(menu);
    }

MainWidget::~MainWidget()
{
    delete mainwidgetgl;
}


void MainWidget::openFile()
{
  QString fname = QFileDialog::getOpenFileName(this,"Open PlantGL",filename,tr("PlantGL File")+" (*.geom;*.bgeom);;"+tr("All Files (*.*)"));
  if(!fname.isEmpty()){
     openGeomFile(fname);
  }
}

void MainWidget::openGeomFile(const QString& fname)
{
    filename = fname;
    ScenePtr scene = ScenePtr(new Scene(fname.toStdString()));
    setScene(scene);
}

