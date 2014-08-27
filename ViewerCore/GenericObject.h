/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *	Wrapper class to render an object, given its vertex attributes and
 *	associated shader.
 */

#ifndef __GENERICOBJECT_H__
#define __GENERICOBJECT_H__

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>

#include <ViewerCore/Object.h>
#include <ViewerCore/BoundingBox.h>
#include <ViewerCore/ShadersList.h>

class GenericObject : protected Object, public BoundingBox
{
public:
	GenericObject();
	~GenericObject();

	bool LocalInitialize(); /* Call this function after initializing vertex data */
	
	bool InitializeVertexPositionsAndIndices(
		QVector<QVector3D> &vertex_positions_,
		QVector<GLuint> &vertex_indices_
		);

	bool InitializeVertexNormals(
		QVector<QVector3D> &vertex_normals_
		);

	bool InitializeVertexTextureCoordinates(
		QVector<QVector2D> &vertex_texture_coordinates_
		);

	void SetVertexColor(QVector3D &color_);

	bool InitializeVertexColor(
		QVector<QVector3D> &vertex_color_
		);

	bool Draw(
		const QMatrix4x4 &projection,
		const QMatrix4x4 &modelview_cam,
		QMatrix4x4 modelview,
		QOpenGLShaderProgram *shader_
		);

	// Add light
	void AddLight(Light &light_);

	/* Get number of lights */
	GLuint GetNumberOfLights();

	/* Edit light details, given the light number. If light number
	   is not provided, first light will be edited. In case no light
	   exists, provided light will be added to the scene */
	void EditLight(Light &light_details_, GLuint idx_ = 0);

	// Set object shininess parameter
	void SetShininess(GLfloat &shininess_);

	void show_wire_frame(bool swf_);
	void show_normals(bool sn_);

	void include_ambience(bool ia_);
	void include_diffuse(bool id_);
	void include_specular(bool is_);

	// Delete all created vectors, associated shaders
	void TakeDown();

private:
	GLuint num_vertices;
	GLuint num_vertex_indices;

	bool vertex_positions_available;
	bool vertex_normals_available;
	bool vertex_texture_coordinates_available;
	bool vertex_indices_available;
	bool vertex_color_available;

	bool show_normals_;
	
	bool include_ambience_;
	bool include_diffuse_;
	bool include_specular_;

	QVector3D vertex_global_color;

	/* bounding box */
	bounding_box b_box_;

	// Compute and store triangle normals
	void ComputeTriangleNormals();

	// Compute vertex normals as average of neighboring triangles normal vectors.
	void ComputeVertexNormals();

	// Returns all adjacent triangle indices so that for any vertex location
	// we can know triangle indices that should be used for normal calculation purposes.
	QVector<GLuint> AdjacentTriangleIndices(GLuint w, GLuint h);

	// Build vertices that act as end points to normal vectors.
	void BuildNormalVisualizationGeometry();

	// Set vertex_global_color to all vertices
	void InitializeVerticesColorToGlobal();

	ShadersList shaders_list;
	bool is_shader_for_normals_initialized;

	typedef Object super;
};

#endif // END OF __GENERICOBJECT_H__
