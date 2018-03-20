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

#ifndef RENDER_3D_H
#define RENDER_3D_H

#include <vector>
#include "defines.h"
#include "camera.h"
#include "maths.h"
#include "shader.h"
#include "texture.h"

#ifndef MAX_MODELS
#define MAX_MODELS 10000
#endif

struct Mesh {
	u16 vertexCount;
	GLuint vao;
	GLuint vbo;
};

struct Material {
	Shader shader;
	Texture diffuse;
	Texture normals;
	Texture specular;
	vec4 diffuseColor;
	vec4 ambientColor;
	vec4 specularColor;
	f32 gloss;
};

struct Model {
	Mesh mesh;
	Material material;
	Texture texture;
	vec3 pos;
	vec3 rotate;
	vec3 scale;
};

Model load_model(const char* path);
void dispose_model(Model& model);

void init3D();

void begin3D();
void begin3D(Camera* camera);
void begin3D(mat4 projection);
void begin3D(Camera* camera, mat4 projection);

void draw_model(Model& model);
void draw_cube(f32 x, f32 y, f32 z, f32 width, f32 height, f32 depth);
void draw_sphere(f32 x, f32 y, f32 z, f32 radius);
void draw_billboard(f32 x, f32 y, f32 z, f32 width, f32 height, Texture tex);
void draw_polygon();

void end3D();

void set_3D_render_viewport(u32 width, u32 height);
void set_FOV(f32 fieldOfView);
f32 get_FOV();

#endif