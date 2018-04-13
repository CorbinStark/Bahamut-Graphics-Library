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
#include <string>
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
//this maps a texture ID with a list of models
INTERNAL std::unordered_map<GLuint, std::vector<Model*>> drawPool;

INTERNAL Shader shader;

INTERNAL inline
void bind_mesh(Mesh mesh) {
	glBindVertexArray(mesh.vao);
	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //uvs
	glEnableVertexAttribArray(2); //normals
}

INTERNAL inline
void unbind_mesh() {
	glDisableVertexAttribArray(2);	//normals
	glDisableVertexAttribArray(1);	//uvs
	glDisableVertexAttribArray(0);	//position
	glBindVertexArray(0);
}

INTERNAL inline
std::vector<std::string> strsplit(const GLchar *str, GLchar c) {
	std::vector<std::string> result;
	do {
		const GLchar *begin = str;
		while (*str != c && *str)
			str++;

		result.push_back(std::string(begin, str));
	} while (0 != *str++);
	return result;
}

INTERNAL inline 
void process_vertex(std::vector<std::string>& vertexData, std::vector<GLushort>& indices, std::vector<vec2>& textures, std::vector<vec3>& normals, GLfloat textureArray[], GLfloat normalsArray[]) {
	int currVertIndex = std::stoi(vertexData[0]) - 1;
	indices.push_back(currVertIndex);

	try {
		vec2 currentTex = textures.at(std::stoi(vertexData[1]) - 1);
		textureArray[currVertIndex * 2] = currentTex.x;
		textureArray[currVertIndex * 2 + 1] = 1 - currentTex.y;
	}
	catch (std::exception e) {
		vec2 currentTex = V2(0, 0);
		textureArray[currVertIndex * 2] = currentTex.x;
		textureArray[currVertIndex * 2 + 1] = 1 - currentTex.y;
	}

	vec3 currentNorm = normals.at(std::stoi(vertexData[2]) - 1);
	normalsArray[currVertIndex * 3] = currentNorm.x;
	normalsArray[currVertIndex * 3 + 1] = currentNorm.y;
	normalsArray[currVertIndex * 3 + 2] = currentNorm.z;
}

INTERNAL
Mesh loadOBJ(const char* path) {
	Mesh mesh = { 0 };

	std::ifstream file(path);

	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	std::vector<GLushort> indices;

	GLfloat* verticesArray = NULL;
	GLfloat* normalsArray = NULL;
	GLfloat* textureArray = NULL;
	GLushort* indicesArray = NULL;

	u32 texcount = 0;
	u32 normalscount = 0;
	u32 indexcount = 0;
	u32 vertexcount = 0;

	if (file) {
		std::string line;
		while (true) {
			getline(file, line);

			if (!line.empty()) {
				std::vector<std::string> tokens;
				tokens = strsplit(line.c_str(), ' ');

				if (tokens[0] == "o") {
					BMT_LOG(INFO, "[%s] Loading mesh from .OBJ. (%s)", path, tokens[1].c_str());
				}

				else if (tokens[0] == "v") {
					vec3 v = V3(std::stof(tokens[1], 0), std::stof(tokens[2], 0), std::stof(tokens[3], 0));
					vertices.push_back(v);
				}

				else if (tokens[0] == "vt") {
					vec2 uv = V2(std::stof(tokens[1], 0), std::stof(tokens[2], 0));
					uvs.push_back(uv);
				}

				else if (tokens[0] == "vn") {
					vec3 norm = V3(std::stof(tokens[1], 0), std::stof(tokens[2], 0), std::stof(tokens[3], 0));
					normals.push_back(norm);
				}

				else if (tokens[0] == "f") {
					texcount = vertices.size() * 2;
					normalscount = vertices.size() * 3;
					textureArray = new GLfloat[texcount]; //need 2 for every vertex
					normalsArray = new GLfloat[normalscount]; //need 3 for every vertex

					break;
				}
			}
		}
		while (!line.empty()) {
			std::vector<std::string> tokens;
			tokens = strsplit(line.c_str(), ' ');
			if (tokens[0] != "f") {
				getline(file, line);
				continue;
			}

			std::vector<std::string> vertex1 = strsplit(tokens[1].c_str(), '/');
			std::vector<std::string> vertex2 = strsplit(tokens[2].c_str(), '/');
			std::vector<std::string> vertex3 = strsplit(tokens[3].c_str(), '/');

			process_vertex(vertex1, indices, uvs, normals, textureArray, normalsArray);
			process_vertex(vertex2, indices, uvs, normals, textureArray, normalsArray);
			process_vertex(vertex3, indices, uvs, normals, textureArray, normalsArray);
			getline(file, line);
		}
	}

	vertexcount = vertices.size() * 3;
	indexcount = indices.size();
	verticesArray = new GLfloat[vertexcount];
	indicesArray = new GLushort[indexcount];

	int vertexIndex = 0;
	for (vec3 vertex : vertices) {
		verticesArray[vertexIndex++] = vertex.x;
		verticesArray[vertexIndex++] = vertex.y;
		verticesArray[vertexIndex++] = vertex.z;
	}

	for (auto i = 0; i < indices.size(); ++i) {
		indicesArray[i] = indices.at(i);
	}

	//VAO
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	//POSITION
	glGenBuffers(1, &mesh.posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.posVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(GLfloat), verticesArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//TEXTURE COORDS
	glGenBuffers(1, &mesh.uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.uvVBO);
	glBufferData(GL_ARRAY_BUFFER, texcount * sizeof(GLfloat), textureArray, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//NORMALS
	glGenBuffers(1, &mesh.normVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.normVBO);
	glBufferData(GL_ARRAY_BUFFER, normalscount * sizeof(GLfloat), normalsArray, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//INDEX
	glGenBuffers(1, &mesh.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indexcount, indicesArray, GL_STATIC_DRAW);

	mesh.indexcount = indexcount;
	mesh.vertexcount = vertexcount;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] verticesArray;
	delete[] normalsArray;
	delete[] textureArray;
	delete[] indicesArray;

	BMT_LOG(INFO, "[%s] .obj file loaded!", path);

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

	model.mesh = mesh;
	model.scale = V3(1, 1, 1);

	return model;
}

void dispose_model(Model& model) {

}

//TODO: Create giant arrays of data containing vertex information for cubes and spheres so that they can be drawn.

void init3D() {
	drawPool.clear();
}

void begin3D(Shader shader_in, bool blending, bool depthTest) {
	shader = shader_in;
	start_shader(shader);

	if (blending)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	if (depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	drawPool.clear();
}

void draw_model(Model& model) {
	drawPool[model.texture.ID].push_back(&model);
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
		//texture gets bound, then all models with that texture get drawn
		std::vector<Model*>* modelList = &current.second;
		bind_texture(modelList->at(0)->texture, 0);

		for (u16 i = 0; i < modelList->size(); ++i) {
			Model* currentModel = modelList->at(i);
			upload_mat4(shader, "transformation", create_transformation_matrix(currentModel->pos, currentModel->rotate, currentModel->scale));
			bind_mesh(currentModel->mesh);
			glDrawElements(GL_TRIANGLES, currentModel->mesh.indexcount, GL_UNSIGNED_SHORT, 0);
			unbind_mesh();
		}
		unbind_texture(0);
	}
	stop_shader();
}