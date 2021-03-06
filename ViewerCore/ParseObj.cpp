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

#include "ParseObj.h"

ParseObj::ParseObj()
{
}

ParseObj::~ParseObj()
{
}

void ParseObj::ParseObjFile(const char* file_name)
{
	this->LoadObjFile(
		file_name,
		this->vertex_positions,
		this->vertex_normals,
		this->vertex_texture_coordinates,
		this->vertex_indices,
		this->names
		);
}

void ParseObj::LoadObjFile(
	const char* file_name,
	QVector<QVector<QVector3D> > &vertex_positions,
	QVector<QVector<QVector3D> > &vertex_normals,
	QVector<QVector<QVector2D> > &vertex_texture_coordinates,
	QVector<QVector<unsigned int> > &vertex_indices,
	QVector<std::string> &names
	)
{
	std::vector<tinyobj::shape_t> shapes;

	std::string err = tinyobj::LoadObj(shapes, file_name);

	//qDebug() << "In file " << file_name << " No. of shapes = " << shapes.size();

	vertex_positions.resize(shapes.size());
	vertex_normals.resize(shapes.size());
	vertex_texture_coordinates.resize(shapes.size());
	vertex_indices.resize(shapes.size());
	names.resize(shapes.size());

	for (unsigned int i = 0; i < shapes.size(); ++i) {
		vertex_positions[i].resize(shapes[i].mesh.positions.size() / 3);
		vertex_normals[i].resize(shapes[i].mesh.normals.size() / 3);
		vertex_texture_coordinates[i].resize(shapes[i].mesh.texcoords.size() / 2);
		vertex_indices[i].resize(shapes[i].mesh.indices.size());
		names[i] = shapes[i].name;

		//qDebug() << "No. of vertex positions = " << shapes[i].mesh.positions.size() / 3;
		//qDebug() << "No. of vertex normals = " << shapes[i].mesh.normals.size() / 3;
		//qDebug() << "No. of vertex texture coordinates = " << shapes[i].mesh.texcoords.size() / 2;
		//qDebug() << "No. of vertex indices = " << shapes[i].mesh.indices.size();

		for (unsigned int j = 0; j < shapes[i].mesh.positions.size() / 3; ++j) {
			vertex_positions[i][j] = QVector3D(
							shapes[i].mesh.positions[3*j + 0],
							shapes[i].mesh.positions[3*j + 1],
							shapes[i].mesh.positions[3*j + 2]
							);
		}

		for (unsigned int j = 0; j < shapes[i].mesh.normals.size() / 3; ++j) {
			vertex_normals[i][j] = QVector3D(
							shapes[i].mesh.normals[3*j + 0],
							shapes[i].mesh.normals[3*j + 1],
							shapes[i].mesh.normals[3*j + 2]
							);
		}

		for (unsigned int j = 0; j < shapes[i].mesh.texcoords.size() / 2; ++j) {
			vertex_texture_coordinates[i][j] = QVector2D(
								shapes[i].mesh.texcoords[2*j + 0],
								shapes[i].mesh.texcoords[2*j + 1] * -1
								);
		}

		for (unsigned int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
			vertex_indices[i][j] = shapes[i].mesh.indices[j];
		}
	}
}
