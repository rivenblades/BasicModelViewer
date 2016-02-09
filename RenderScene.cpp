/*
 * This file is part of Basic Model Viewer (visualization package)
 *
 * Copyright (c) 2015, Lakshman Anumolu
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "RenderScene.h"

RenderScene::RenderScene()
{
}

RenderScene::~RenderScene()
{
}

bool RenderScene::InitializeScene()
{
    if (!this->InitializeShaders())
        return false;

    // Initialize default objects
    if (!this->drawing_room.LocalInitialize())
        return false;

    if (!this->cubehandle_obj.LocalInitialize("../Resources/cubehandle0fix.obj"))
        return false;
    this->cubehandle_obj.show_wire_frame(false);
    this->cubehandle_obj.show_normals(false);

    if (!this->bunny_obj.LocalInitialize("../Resources/bunny.obj"))
        return false;
    this->bunny_obj.show_wire_frame(false);
    this->bunny_obj.show_normals(false);

    Light light_for_bunny;
    light_for_bunny.position = QVector3D(0.0, 0.5, 0.0);
    light_for_bunny.color_ambient = QVector3D(0.1, 0.6, 0.6);
    light_for_bunny.color_diffuse = QVector3D(0.1, 0.6, 0.6);
    light_for_bunny.color_specular = QVector3D(1.0, 1.0, 1.0);

    this->bunny_obj.EditLight(light_for_bunny);

    if (!this->teapot_obj.LocalInitialize("../Resources/teapot.obj"))
        return false;
    /*if (!this->teapot_obj.LocalInitialize("../personal_resources/3.obj"))
        return false;*/
    this->teapot_obj.show_wire_frame(true);

    /* Edit default light info */
    Light light_for_teapot;
    light_for_teapot.position = QVector3D(0.0, 0.0, 10.0);
    light_for_teapot.color_ambient = QVector3D(0.75, 0.6, 0.5);
    light_for_teapot.color_diffuse = QVector3D(0.75, 0.6, 0.5);
    light_for_teapot.color_specular = QVector3D(1.0, 1.0, 1.0);

    this->teapot_obj.EditLight(light_for_teapot);

    //obj_models.AddObj(&cubehandle_obj);
    //obj_models.AddObj(&teapot_obj);

    // Initialize 50 teapots
    srand(time(NULL));
    teapot_objs.resize(1);
    for (int i = 0; i < teapot_objs.size(); ++i) {
      QVector3D obj_positions_offset = QVector3D(rand() % 100 - 50,
                                                 rand() % 100 - 50,
                                                 rand() % 100 - 50);
      teapot_objs[i].SetVertexPositionsGlobalOffset(obj_positions_offset);
      if (!teapot_objs[i].LocalInitialize("../Resources/teapot.obj"))
        return false;
      obj_models.AddObj(&teapot_objs[i]);
    }
    int clones_max_count = 500000;
    positions_global_offset.resize(clones_max_count);
    for (int i = 0; i < clones_max_count; ++i) {
        positions_global_offset[i] = QVector3D(rand() % 1000 - 500,
                                               rand() % 1000 - 500,
                                               rand() % 1000 - 500);
    }

    return true;
}

bool RenderScene::InitializeShaders()
{
    return shaders_list.InitializeDefaultShaders();
}

bool RenderScene::Draw(
    const QMatrix4x4 &projection,
    const QMatrix4x4 &modelview_cam,
    QMatrix4x4 modelview,
    QOpenGLShaderProgram *shader_
    )
{
    if (!drawing_room.Draw(projection, modelview_cam, modelview, shader_))
        return false;

    /*if (!cubehandle_obj.Draw(projection, modelview_cam, modelview, shader_))
        return false;*/

    /*if (!this->bunny_obj.Draw(projection, modelview_cam, modelview, shader_))
        return false;*/

    QMatrix4x4 mv_tmp_ = modelview;
    int num_objects = positions_global_offset.size();
    for (int i = 0; i < num_objects; ++i) {
        modelview.translate(positions_global_offset[i]);
        if (!this->obj_models.Draw(projection, modelview_cam, modelview, shader_))
          return false;
        modelview = mv_tmp_;
    }

    //modelview.translate(QVector3D(5.0, 5.0,-2.0));
    /*if (!teapot_obj.Draw(projection, modelview_cam, modelview, shader_))
        return false;*/
    modelview = mv_tmp_;

    /*modelview.translate(QVector3D(0.0, 0.0, -15.0));
    if (!this->test1_pov.Draw(projection, modelview_cam, modelview, shader_))
        return false;
    modelview = mv_tmp_;*/

    return true;
}

void RenderScene::TakeDown()
{
    this->shaders_list.TakeDown();
    this->drawing_room.TakeDown();
    this->cubehandle_obj.TakeDown();
    this->bunny_obj.TakeDown();
    this->teapot_obj.TakeDown();
    for (int i = 0; i < teapot_objs.size(); ++i)
      teapot_objs[i].TakeDown();
    teapot_objs.clear();
    //this->test1_pov.TakeDown();
}
