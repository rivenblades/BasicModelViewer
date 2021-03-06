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

#include "RenderObj.h"

RenderObj::RenderObj()
    : number_of_shapes(0)
{
	default_vertex_color = QVector3D(0.6, 0.6, 0.6);
	default_light.position = QVector3D(0.0, 0.0, -10.0);
	default_light.color_ambient = QVector3D(0.3, 0.3, 0.3);
	default_light.color_diffuse = QVector3D(0.3, 0.3, 0.3);
	default_light.color_specular = QVector3D(1.0, 1.0, 1.0);
}

RenderObj::~RenderObj()
{
}

bool RenderObj::LocalInitialize(QString obj_file_name)
{
    QVector3D light0_pos = QVector3D(0, 10, 10);
    QVector3D color_ambient = QVector3D(0.5, 0.5, 0.7);
    QVector3D color_diffuse = QVector3D(0.5, 0.5, 0.7);
    QVector3D color_specular = QVector3D(1.0, 1.0, 1.0);
    Light light0;
    light0.position = light0_pos;
    light0.color_ambient = color_ambient;
    light0.color_diffuse = color_diffuse;
    light0.color_specular = color_specular;

    parse_obj.ParseObjFile(obj_file_name.toUtf8().constData());

    this->number_of_shapes = parse_obj.names.size();
    generic_object = new GenericObject[this->number_of_shapes];

    for (GLuint i = 0; i < this->number_of_shapes; ++i) {
        generic_object[i].InitializeVertexPositionsAndIndices(parse_obj.vertex_positions[i],
                                                        parse_obj.vertex_indices[i]);

        if (parse_obj.vertex_normals[i].size() > 0)
            generic_object[i].InitializeVertexNormals(parse_obj.vertex_normals[i]);

        if (parse_obj.vertex_texture_coordinates[i].size() > 0)
            generic_object[i].InitializeVertexTextureCoordinates(parse_obj.vertex_texture_coordinates[i]);

        QVector3D generic_object_color = QVector3D(.0, 0.2, 0.7);
        generic_object[i].SetVertexColor(generic_object_color);
        GLfloat shininess_g = 100;
        generic_object[i].SetShininess(shininess_g);
        light0.position = light0_pos;
        generic_object[i].AddLight(light0);

        generic_object[i].LocalInitialize();
    }

	return !GLReturnedError("RenderObj::LocalInitialize - on exit");
}

void RenderObj::SetVertexColor(QVector3D &color_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].SetVertexColor(color_);
}

bool RenderObj::Draw(
	const QMatrix4x4 &projection,
	const QMatrix4x4 &modelview_cam,
	QMatrix4x4 modelview,
	QOpenGLShaderProgram *shader_
	)
{

    for (GLuint i = 0; i < this->number_of_shapes; ++i)
        if (!this->generic_object[i].Draw(projection, modelview_cam, modelview, shader_))
            return false;

    return true;
}

void RenderObj::AddLight(Light light_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].AddLight(light_);
}

GLuint RenderObj::GetNumberOfLights()
{
    if (this->number_of_shapes > 0)
        return generic_object[0].GetNumberOfLights();
    else
        return 0;
}

void RenderObj::EditLight(Light light_, GLuint idx_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
        generic_object[i].EditLight(light_, idx_);
}

void RenderObj::SetShininess(GLfloat shininess_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].SetShininess(shininess_);
}

void RenderObj::show_wire_frame(bool swf_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].show_wire_frame(swf_);
}

void RenderObj::show_normals(bool sn_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].show_normals(sn_);
}

QVector<bounding_box> RenderObj::GetBoundingBoxes()
{
    QVector<bounding_box> b_boxes;
    for (GLuint i = 0; i < this->number_of_shapes; ++i) {
        b_boxes.push_back(generic_object[i].GetBoundingBox());
    }
    return b_boxes;
}

void RenderObj::TakeDown()
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].TakeDown();

    delete[] generic_object;
}
