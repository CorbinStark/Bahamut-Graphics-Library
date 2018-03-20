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
void bindMesh(Mesh mesh) {
	glBindVertexArray(mesh.vao);
	glEnableVertexAttribArray(0);	//vertices location
	glEnableVertexAttribArray(1);	//texture coords location
	glEnableVertexAttribArray(2);	//normals location
}

INTERNAL 
void unbindMesh() {
	glDisableVertexAttribArray(2);	//normals location
	glDisableVertexAttribArray(1);	//texture coords location
	glDisableVertexAttribArray(0);	//vertices location
	glBindVertexArray(0);
}

INTERNAL
Mesh loadOBJ(const char* path) {
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
}

Model loadModel(const char* path) {
	Model model = { 0 };
	Mesh mesh = { 0 };

	if (has_extension(path, "obj"))			mesh = loadOBJ(path);
	//else if (has_extension(path, "ogg"))  mesh = loadOGG(filename);
	//else if (has_extension(path, "flac")) mesh = loadFLAC(filename);
	//else if (has_extension(path, "mp3"))  mesh = loadMP3(filename);
	else {
		BMT_LOG(WARNING, "[%s] Extension not supported!", path);
	}
	return model;
}

void disposeModel(Model& model) {

}

//TODO: Create giant arrays of data containing vertex information for cubes and spheres so that they can be drawn.

void init3D() {
	drawPool.clear();
	usingCustomProjection = false;
	customProjection = identity();
	FOV = 90;
	aspectRatio = getWindowWidth() / getWindowHeight();
	perspective = perspective_projection(FOV, aspectRatio, 0.1f, 999.0f);
}

void begin3D() {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, getWindowWidth(), getWindowHeight());
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

void drawModel(Model& model) {
	drawPool[model.material.shader.ID].push_back(&model);
}

void drawCube(f32 x, f32 y, f32 z, f32 width, f32 height, f32 depth) {
	//create cubeMesh
	//drawPool.emplace(std::pair<Mesh*, Model*>(&cubeMesh, &cubeModel));
}

void drawSphere(f32 x, f32 y, f32 z, f32 radius) {
	//create sphereMesh
	//drawPool.emplace(std::pair<Mesh*, Model*>(&sphereMesh, &sphereModel));
}

void drawBillboard(f32 x, f32 y, f32 z, f32 width, f32 height, Texture tex) {
	//boardModel.texture = tex;
	//drawPool.emplace(std::pair<Mesh*, Model*>(&boardMesh, &boardModel));
}

void drawPolygon() {

}

void end3D() {
	//if(cam != NULL) 

	//TODO: Could be optimized.
	for (auto current : drawPool) {
		std::vector<Model*>* modelList = &current.second;
		Shader* currShader = &modelList->at(0)->material.shader;
		startShader(currShader);
		//if (cam != NULL) loadMat4(currShader, "vw_matrix", create_view_matrix(*cam));
		loadMat4(currShader, "projection_matrix", perspective);

		for (u16 i = 0; i < modelList->size(); ++i) {
			Model* currentModel = modelList->at(i);
			bindMesh(currentModel->mesh);
			bindTexture(currentModel->texture, 0);

			loadMat4(
				currShader,
				"transformation_matrix",
				create_transformation_matrix(currentModel->pos, currentModel->rotate, currentModel->scale)
			);

			unbindTexture(0);
			unbindMesh();
		}

		stopShader();
	}
}

void set3DRenderViewport(u32 width, u32 height) {
	glViewport(0, 0, getWindowWidth(), getWindowHeight());
	aspectRatio = getWindowWidth() / getWindowHeight();
	perspective = perspective_projection(FOV, aspectRatio, 0.1f, 999.0f);

}

void setFOV(f32 fieldOfView) {
	FOV = fieldOfView;
	aspectRatio = getWindowWidth() / getWindowHeight();
	perspective = perspective_projection(FOV, aspectRatio, 0.1f, 999.0f);
}

f32 getFOV() {
	return FOV;
}