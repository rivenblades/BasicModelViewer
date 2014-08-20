/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *  Control and act on the signals received from window.
 */

#ifndef __VIEWERWINDOW_H__
#define __VIEWERWINDOW_H__

#include <QGLWidget>
#include <QDialog>
#include <QVector>
#include <QVector2D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QBasicTimer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <AuxiliaryMethods/HelperFunctions.h>
#include <ViewerCore/ViewerCamera.h>
#include <ViewerCore/ShadersList.h>
#include <Objects/RenderObj.h>
#include <Objects/DrawingRoom.h>
#include <RenderScene.h>

class RenderScene;

class ViewerWindow : public QGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit ViewerWindow(const QGLFormat &format, QWidget *parent = 0);
    ~ViewerWindow();

    int xRotation() const { return xRot; }
    int yRotation() const { return yRot; }
    int zRotation() const { return zRot; }

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    void timerEvent(QTimerEvent *e);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void closeEvent(QCloseEvent *e);

    /* Mouse events */
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

    /* Key events */
    void keyPressEvent(QKeyEvent *e);

    void initializeShaders();
    void initializeTextures(QString img_name, QString img_extension);

private:
    QBasicTimer timer;
    ShadersList shaders_list;

    GLuint texture_t;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;

    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;

    /* Camera details */
    ViewerCamera camera;
    CameraType cam_type;
    QVector3D cam_position_initial, camera_position;
    QVector3D cam_lookat_initial, camera_lookat;
    QVector3D cam_up_initial, camera_up;
    qreal aspect_ratio;
    float fov_init, fov, fov_min, fov_max;
    float near_clip_plane, far_clip_plane;

    /* Object for rendering the scene */
    RenderScene render_scene;
};

#endif // END OF __VIEWERWINDOW_H__
