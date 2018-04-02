///////////////////////////////////////////////////////////////////////////
// FILE:                      render3D.cpp                               //
///////////////////////////////////////////////////////////////////////////
//                      BAHAMUT GRAPHICS LIBRARY                         //
//                        Author: Corbin Stark                           //
///////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Corbin Stark                                       //
//                                                                       //
// Permission is hereby granted, free of charge, to any person obtaining //
// a copy of this software and associated documentation files (the       //
// "Software"), to deal in the Software without restriction, including   //
// without limitation the rights to use, copy, modify, merge, publish,   //
// distribute, sublicense, and/or sell copies of the Software, and to    //
// permit persons to whom the Software is furnished to do so, subject to //
// the following conditions:                                             //
//                                                                       //
// The above copyright notice and this permission notice shall be        //
// included in all copies or substantial portions of the Software.       //
//                                                                       //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       //
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.//
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  //
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  //
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     //
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                //
///////////////////////////////////////////////////////////////////////////

#include <unordered_map>
#include <fstream>
#include "window.h"
#include "render3D.h"

//struct RenderGroup3D {
//	Model* model;
//	GLuint shaderID;
//};
//
//INTERNAL RenderGroup3D drawPool[MAX_MODELS];
//INTERNAL u16 modelcount;

//TODO: Replace this with my own map implementation
INTERNAL std::unordered_map<GLuint, std::vector<Model*>> drawPool;

INTERNAL Shader* defaultShader;
INTERNAL bool usingCustomProjection;
INTERNAL mat4 customProjection;
INTERNAL mat4 perspective;
INTERNAL Camera* cam;
INTERNAL f32 FOV;
INTERNAL f32 aspectRatio;

INTERNAL
void bind_mesh(Mesh mesh) {
	glBindVertexArray(mesh.vao);
	glEnableVertexAttribArray(0);	//vertices location
	glEnableVertexAttribArray(1);	//texture coords location
	glEnableVertexAttribArray(2);	//normals location
}

INTERNAL 
void unbind_mesh() {
	glDisableVertexAttribArray(2);	//normals location
	glDisableVertexAttribArray(1);	//texture coords location
	glDisableVertexAttribArray(0);	//vertices location
	glBindVertexArray(0);
}

INTERNAL
Mesh loadOBJ(const char* path) {
	Mesh mesh = { 0 };

	std::ifstream file(path);
	if (!file) {
		BMT_LOG(WARNING, "[%s] Path to .obj could not be opened!");
	}

	std::vector<vec3> vertices;
	std::vector<vec2> textures;
	std::vector<vec3> normals;
	std::vector<GLint> indices;

	GLfloat* vertex_array = NULL;
	GLfloat* normals_array = NULL;
	GLfloat* texture_array = NULL;
	GLint* index_array = NULL;

	while (true) {

	}

	return mesh;
}

INTERNAL
Mesh load3DS(const char* path) {
	Mesh mesh = { 0 };
	return mesh;
}

INTERNAL
Mesh loadFBX(const char* path) {
	Mesh mesh = { 0 };
	return mesh;
}

Model load_model(const char* path) {
	Model model = { 0 };
	Mesh mesh = { 0 };

	if (has_extension(path, "obj"))			mesh = loadOBJ(path);
	else if (has_extension(path, "3DS"))	mesh = load3DS(path);
	else if (has_extension(path, "FBX"))	mesh = loadFBX(path);
	else {
		BMT_LOG(WARNING, "[%s] Extension not supported!", path);
	}

	return model;
}

void dispose_model(Model& model) {

}

//TODO: Create giant arrays of data containing vertex information for cubes and spheres so that they can be drawn.

void init3D() {
	drawPool.clear();
	usingCustomProjection = false;
	customProjection = identity();
	FOV = 90;
	aspectRatio = get_window_width() / get_window_height();
	perspective = perspective_projection(FOV, aspectRatio, 0.1f, 999.0f);
}

void begin3D() {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, get_window_width(), get_window_height());
	drawPool.clear();
}

void begin3D(Camera* camera) {
	cam = camera;
	begin3D();
}

void begin3D(mat4 projection) {
	usingCustomProjection = true;
	customProjection = projection;
	begin3D();
}

void begin3D(Camera* camera, mat4 projection) {
	usingCustomProjection = true;
	customProjection = projection;
	cam = camera;
	begin3D();
}

void draw_model(Model& model) {
	drawPool[model.material.shader.ID].push_back(&model);
}

void draw_cube(f32 x, f32 y, f32 z, f32 width, f32 height, f32 depth) {
	//create cubeMesh
	//drawPool.emplace(std::pair<Mesh*, Model*>(&cubeMesh, &cubeModel));
}

void draw_sphere(f32 x, f32 y, f32 z, f32 radius) {
	//create sphereMesh
	//drawPool.emplace(std::pair<Mesh*, Model*>(&sphereMesh, &sphereModel));
}

void draw_billboard(f32 x, f32 y, f32 z, f32 width, f32 height, Texture tex) {
	//boardModel.texture = tex;
	//drawPool.emplace(std::pair<Mesh*, Model*>(&boardMesh, &boardModel));
}

void draw_polygon() {

}

void end3D() {
	//if(cam != NULL) 

	//TODO: Could be optimized.
	for (auto current : drawPool) {
		std::vector<Model*>* modelList = &current.second;
		Shader* currShader = &modelList->at(0)->material.shader;
		start_shader(currShader);
		//if (cam != NULL) loadMat4(currShader, "vw_matrix", create_view_matrix(*cam));
		load_mat4(currShader, "projection_matrix", perspective);

		for (u16 i = 0; i < modelList->size(); ++i) {
			Model* currentModel = modelList->at(i);
			bind_mesh(currentModel->mesh);
			bind_texture(currentModel->texture, 0);

			load_mat4(
				currShader,
				"transformation_matrix",
				create_transformation_matrix(currentModel->pos, currentModel->rotate, currentModel->scale)
			);

			unbind_texture(0);
			unbind_mesh();
		}

		stop_shader();
	}
}

void set_3D_render_viewport(u32 width, u32 height) {
	glViewport(0, 0, width, height);
	aspectRatio = width / height;
	perspective = perspective_projection(FOV, aspectRatio, 0.1f, 999.0f);

}

void set_FOV(f32 fieldOfView) {
	FOV = fieldOfView;
	aspectRatio = get_window_width() / get_window_height();
	perspective = perspective_projection(FOV, aspectRatio, 0.1f, 999.0f);
}

f32 get_FOV() {
	return FOV;
}