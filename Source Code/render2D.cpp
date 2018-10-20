///////////////////////////////////////////////////////////////////////////
// FILE:                      render2D.cpp                               //
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

#include "render2D.h"
#include "window.h"

#if defined(BMT_USE_NAMESPACE) 
namespace bmt {
#endif

INTERNAL GLuint vao;
INTERNAL GLuint vbo;
INTERNAL GLuint ebo;
INTERNAL u16 indexcount;
INTERNAL u16 texcount;
INTERNAL GLuint  textures[BATCH_MAX_TEXTURES];
INTERNAL GLchar* locations[BATCH_MAX_TEXTURES];
INTERNAL VertexData* buffer;
INTERNAL Shader shader;

const GLchar* ORTHO_SHADER_FRAG_SHADER = R"FOO(
#version 130
out vec4 outColor;

in vec4 pass_color;
in vec2 pass_uv;
in float pass_texid;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform sampler2D tex7;
uniform sampler2D tex8;
uniform sampler2D tex9;
uniform sampler2D tex10;
uniform sampler2D tex11;
uniform sampler2D tex12;
uniform sampler2D tex13;
uniform sampler2D tex14;
uniform sampler2D tex15;
uniform sampler2D tex16;
void main() {
	vec4 texColor = vec4(1.0);
	if(pass_texid > 0.0){
		if(pass_texid == 1.0) texColor = texture(tex1, pass_uv);
		if(pass_texid == 2.0) texColor = texture(tex2, pass_uv);
		if(pass_texid == 3.0) texColor = texture(tex3, pass_uv);
		if(pass_texid == 4.0) texColor = texture(tex4, pass_uv);
		if(pass_texid == 5.0) texColor = texture(tex5, pass_uv);
		if(pass_texid == 6.0) texColor = texture(tex6, pass_uv);
		if(pass_texid == 7.0) texColor = texture(tex7, pass_uv);
		if(pass_texid == 8.0) texColor = texture(tex8, pass_uv);
		if(pass_texid == 9.0) texColor = texture(tex9, pass_uv);
		if(pass_texid == 10.0) texColor = texture(tex10, pass_uv);
		if(pass_texid == 11.0) texColor = texture(tex11, pass_uv);
		if(pass_texid == 12.0) texColor = texture(tex12, pass_uv);
		if(pass_texid == 13.0) texColor = texture(tex13, pass_uv);
		if(pass_texid == 14.0) texColor = texture(tex14, pass_uv);
		if(pass_texid == 15.0) texColor = texture(tex15, pass_uv);
		if(pass_texid == 16.0) texColor = texture(tex16, pass_uv);
	}
	outColor = pass_color * texColor;
}

)FOO";

const GLchar* ORTHO_SHADER_VERT_SHADER = R"FOO(
#version 130
in vec2 position;
in vec4 color;
in vec2 uv;
in float texid;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);

out vec4 pass_color;
out vec2 pass_uv;
out float pass_texid;

void main() {
	pass_color = color;
	pass_uv = uv;
	pass_texid = texid;
	
	gl_Position = projection * view * vec4(position, 1.0, 1.0);
}

)FOO";

INTERNAL
GLfloat DEFAULT_UVS[8] = {
	0, 0, 0, 1,
	1, 1, 1, 0
};
INTERNAL
GLfloat FLIP_HORIZONTAL_UVS[8] = {
	1, 0, 1, 1,
	0, 1, 0, 0
};
INTERNAL
GLfloat FLIP_VERTICAL_UVS[8] = {
	0, 1, 0, 0,
	1, 0, 1, 1
};
INTERNAL
GLfloat FLIP_BOTH_UVS[8] = {
	1, 1, 1, 0,
	0, 0, 0, 1
};

INTERNAL
int submit_tex(Texture tex) {
	int texSlot = 0;
	bool found = false;
	for (u32 i = 0; i < texcount; ++i) {
		if (textures[i] == tex.ID) {
			texSlot = (i + 1);
			found = true;
			break;
		}
	}
	if (!found) {
		if (texcount >= BATCH_MAX_TEXTURES) {
			end2D();
			begin2D(shader);
		}
		textures[texcount++] = tex.ID;
		texSlot = texcount;
	}
	return texSlot;
}

Shader load_default_shader_2D() {
	return load_shader_2D_from_strings(ORTHO_SHADER_VERT_SHADER, ORTHO_SHADER_FRAG_SHADER);
}

void init2D(i32 x, i32 y, u32 width, u32 height) {
	texcount = indexcount = 0;

	locations[0] = "tex1";
	locations[1] = "tex2";
	locations[2] = "tex3";
	locations[3] = "tex4";
	locations[4] = "tex5";
	locations[5] = "tex6";
	locations[6] = "tex7";
	locations[7] = "tex8";
	locations[8] = "tex9";
	locations[9] = "tex10";
	locations[10] = "tex11";
	locations[11] = "tex12";
	locations[12] = "tex13";
	locations[13] = "tex14";
	locations[14] = "tex15";
	locations[15] = "tex16";

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, BATCH_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	//the last argument to glVertexAttribPointer is the offset from the start of the vertex to the
	//data you want to look at - so each new attrib adds up all the ones before it.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, BATCH_VERTEX_SIZE, (const GLvoid*)0);                     //vertices
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, BATCH_VERTEX_SIZE, (const GLvoid*)(2 * sizeof(GLfloat))); //color
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, BATCH_VERTEX_SIZE, (const GLvoid*)(6 * sizeof(GLfloat))); //tex coords
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, BATCH_VERTEX_SIZE, (const GLvoid*)(8 * sizeof(GLfloat))); //texture id

	GLushort indices[BATCH_INDICE_SIZE];

	int offset = 0;
	for (u32 i = 0; i < BATCH_INDICE_SIZE; i += 6) {
		indices[i] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;
		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, BATCH_INDICE_SIZE * sizeof(GLushort), indices, GL_STATIC_DRAW);

	//the vao must be unbound before the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void begin2D(Shader shader_in, bool blending, bool depthTest) {
	shader = shader_in;
	start_shader(shader_in);

	if (blending)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	if (depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	buffer = (VertexData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, BATCH_BUFFER_SIZE,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
	);
}

void draw_texture(Texture tex, i32 xPos, i32 yPos) {
	if (tex.ID == 0)
		return;
	int texSlot = submit_tex(tex);
	GLfloat* uvs;

	uvs = DEFAULT_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_BOTH_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL)
		uvs = FLIP_HORIZONTAL_UVS;
	if (tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_VERTICAL_UVS;

	buffer->pos.x = xPos;
	buffer->pos.y = yPos;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos;
	buffer->pos.y = yPos + tex.height;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + tex.width;
	buffer->pos.y = yPos + tex.height;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + tex.width;
	buffer->pos.y = yPos;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[6];
	buffer->uv.y = uvs[7];
	buffer->texid = texSlot;
	buffer++;

	indexcount += 6;
}

void draw_texture(Texture tex, i32 xPos, i32 yPos, i32 width, i32 height) {
	if (tex.ID == 0)
		return;
	int texSlot = submit_tex(tex);
	GLfloat* uvs;

	uvs = DEFAULT_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_BOTH_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL)
		uvs = FLIP_HORIZONTAL_UVS;
	if (tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_VERTICAL_UVS;

	buffer->pos.x = xPos;
	buffer->pos.y = yPos;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos;
	buffer->pos.y = yPos + height;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + width;
	buffer->pos.y = yPos + height;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + width;
	buffer->pos.y = yPos;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[6];
	buffer->uv.y = uvs[7];
	buffer->texid = texSlot;
	buffer++;

	indexcount += 6;
}

void draw_texture(Texture tex, i32 xPos, i32 yPos, vec4 color) {
	draw_texture(tex, xPos, yPos, color.x, color.y, color.z, color.w);
}

void draw_texture(Texture tex, i32 xPos, i32 yPos, f32 r, f32 g, f32 b, f32 a) {
	if (tex.ID == 0)
		return;

	r /= 255;
	g /= 255;
	b /= 255;
	a /= 255;

	int texSlot = submit_tex(tex);
	GLfloat* uvs;

	uvs = DEFAULT_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_BOTH_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL)
		uvs = FLIP_HORIZONTAL_UVS;
	if (tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_VERTICAL_UVS;

	buffer->pos.x = xPos;
	buffer->pos.y = yPos;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos;
	buffer->pos.y = yPos + tex.height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + tex.width;
	buffer->pos.y = yPos + tex.height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + tex.width;
	buffer->pos.y = yPos;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[6];
	buffer->uv.y = uvs[7];
	buffer->texid = texSlot;
	buffer++;

	indexcount += 6;
}

void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, f32 rotateDegree) {
	float originX = xPos + (tex.width / 2.0f);
	float originY = yPos + (tex.height / 2.0f);
	draw_texture_rotated(tex, xPos, yPos, V2(originX, originY), rotateDegree);
}

void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, vec2 origin, f32 rotation) {
	if (tex.ID == 0)
		return;
	int texSlot = submit_tex(tex);
	GLfloat* uvs;
	
	uvs = DEFAULT_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_BOTH_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL)
		uvs = FLIP_HORIZONTAL_UVS;
	if (tex.flip_flag & FLIP_VERTICAL)
		uvs = FLIP_VERTICAL_UVS;
	
	float originX = origin.x;
	float originY = origin.y;
	
	//degToRad is expensive don't do it if there is no rotation
	if (rotation != 0)
		rotation = deg_to_rad(rotation);
	
	float newX;
	float newY;
	
	float cosineDegree = 1;
	float sineDegree = 0;
	//sin and cos are expensive don't do it if there is no rotation
	if (rotation != 0) {
		cosineDegree = cos(rotation);
		sineDegree = sin(rotation);
	}
	
	newX = xPos;
	newY = yPos;
	buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;
	
	newX = xPos;
	newY = yPos + tex.height;
	buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;
	
	newX = xPos + tex.width;
	newY = yPos + tex.height;
	buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;
	
	newX = xPos + tex.width;
	newY = yPos;
	buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[6];
	buffer->uv.y = uvs[7];
	buffer->texid = texSlot;
	buffer++;
	
	indexcount += 6;

	//if (tex.ID == 0)
	//	return;
	//
	//f32 r = 1;
	//f32 g = 1;
	//f32 b = 1;
	//f32 a = 1;
	//
	//int texSlot = submit_tex(tex);
	//GLfloat* uvs;
	//
	//f32 cosTheta = cos(deg_to_rad(rotation));
	//f32 sinTheta = sin(deg_to_rad(rotation));
	//
	//uvs = DEFAULT_UVS;
	//if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
	//	uvs = FLIP_BOTH_UVS;
	//if (tex.flip_flag & FLIP_HORIZONTAL)
	//	uvs = FLIP_HORIZONTAL_UVS;
	//if (tex.flip_flag & FLIP_VERTICAL)
	//	uvs = FLIP_VERTICAL_UVS;
	//
	//buffer->pos.x = ( (xPos - origin.x) * cosTheta - (yPos - origin.y) * sinTheta ) + origin.x;
	//buffer->pos.y = ( (yPos - origin.y) * cosTheta + (xPos - origin.x) * sinTheta ) + origin.y;
	//buffer->color.x = r;
	//buffer->color.y = g;
	//buffer->color.z = b;
	//buffer->color.w = a;
	//buffer->uv.x = uvs[0];
	//buffer->uv.y = uvs[1];
	//buffer->texid = texSlot;
	//buffer++;
	//
	//buffer->pos.x = ( (xPos - origin.x) * cosTheta - (yPos - origin.y) * sinTheta ) + origin.x;
	//buffer->pos.y = ( ((yPos + tex.height) - origin.y) * cosTheta + (xPos - origin.x) * sinTheta ) + origin.y;
	//buffer->color.x = r;
	//buffer->color.y = g;
	//buffer->color.z = b;
	//buffer->color.w = a;
	//buffer->uv.x = uvs[2];
	//buffer->uv.y = uvs[3];
	//buffer->texid = texSlot;
	//buffer++;
	//
	//buffer->pos.x = ( ((xPos + tex.width) - origin.x) * cosTheta - (yPos - origin.y) * sinTheta ) + origin.x;
	//buffer->pos.y = ( ((yPos + tex.height) - origin.y) * cosTheta + (xPos - origin.x) * sinTheta ) + origin.y;
	//buffer->color.x = r;
	//buffer->color.y = g;
	//buffer->color.z = b;
	//buffer->color.w = a;
	//buffer->uv.x = uvs[4];
	//buffer->uv.y = uvs[5];
	//buffer->texid = texSlot;
	//buffer++;
	//
	//buffer->pos.x = ( ((xPos + tex.width) - origin.x) * cosTheta - (yPos - origin.y) * sinTheta ) + origin.x;
	//buffer->pos.y = ( (yPos - origin.y) * cosTheta + (xPos - origin.x) * sinTheta ) + origin.y;
	//buffer->color.x = r;
	//buffer->color.y = g;
	//buffer->color.z = b;
	//buffer->color.w = a;
	//buffer->uv.x = uvs[6];
	//buffer->uv.y = uvs[7];
	//buffer->texid = texSlot;
	//buffer++;
	//
	//indexcount += 6;
}

void draw_texture_EX(Texture tex, Rect source, Rect dest, f32 r, f32 g, f32 b, f32 a) {
	if (tex.ID == 0)
		return;

	r /= 255.0f;
	g /= 255.0f;
	b /= 255.0f;
	a /= 255.0f;

	static GLfloat EX_UVS[8];

	if (tex.flip_flag == 0) {
		EX_UVS[0] = source.x / tex.width;
		EX_UVS[1] = source.y / tex.height;
		EX_UVS[2] = source.x / tex.width;
		EX_UVS[3] = (source.y + source.height) / tex.height;
		EX_UVS[4] = (source.x + source.width) / tex.width;
		EX_UVS[5] = (source.y + source.height) / tex.height;
		EX_UVS[6] = (source.x + source.width) / tex.width;
		EX_UVS[7] = source.y / tex.height;
	}
	else if (tex.flip_flag & FLIP_HORIZONTAL) {
		EX_UVS[0] = (source.x + source.width) / tex.width;
		EX_UVS[1] = source.y / tex.height;
		EX_UVS[2] = (source.x + source.width) / tex.width;
		EX_UVS[3] = (source.y + source.height) / tex.height;
		EX_UVS[4] = source.x / tex.width;
		EX_UVS[5] = (source.y + source.height) / tex.height;
		EX_UVS[6] = source.x / tex.width;
		EX_UVS[7] = source.y / tex.height;
	}
	else if (tex.flip_flag & FLIP_VERTICAL) {
		EX_UVS[0] = source.x / tex.width;
		EX_UVS[1] = (source.y + source.height) / tex.height;
		EX_UVS[2] = source.x / tex.width;
		EX_UVS[3] = source.y / tex.height;
		EX_UVS[4] = (source.x + source.width) / tex.width;
		EX_UVS[5] = source.y / tex.height;
		EX_UVS[6] = (source.x + source.width) / tex.width;
		EX_UVS[7] = (source.y + source.height) / tex.height;
	}
	else if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL) {
		EX_UVS[0] = (source.x + source.width) / tex.width;
		EX_UVS[1] = (source.y + source.height) / tex.height;
		EX_UVS[2] = (source.x + source.width) / tex.width;
		EX_UVS[3] = source.y / tex.height;
		EX_UVS[4] = source.x / tex.width;
		EX_UVS[5] = source.y / tex.height;
		EX_UVS[6] = source.x / tex.width;
		EX_UVS[7] = (source.y + source.height) / tex.height;
	}

	GLfloat* uvs = EX_UVS;

	int texSlot = submit_tex(tex);

	buffer->pos.x = dest.x;
	buffer->pos.y = dest.y;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x;
	buffer->pos.y = dest.y + dest.height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x + dest.width;
	buffer->pos.y = dest.y + dest.height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x + dest.width;
	buffer->pos.y = dest.y;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = uvs[6];
	buffer->uv.y = uvs[7];
	buffer->texid = texSlot;
	buffer++;

	indexcount += 6;
}

void draw_texture_EX(Texture tex, Rect source, Rect dest, vec4 color) {
	draw_texture_EX(tex, source, dest, color.x, color.y, color.z, color.w);
}

void draw_texture_EX(Texture tex, Rect source, Rect dest) {
	draw_texture_EX(tex, source, dest, 255.0f, 255.0f, 255.0f, 255.0f);
}

void draw_framebuffer(Framebuffer buffer, i32 xPos, i32 yPos) {
	draw_texture(buffer.texture, xPos, yPos);
}

void draw_rectangle(i32 x, i32 y, i32 width, i32 height, f32 r, f32 g, f32 b, f32 a) {
	r /= 255.0f;
	g /= 255.0f;
	b /= 255.0f;
	a /= 255.0f;

	buffer->pos.x = x;
	buffer->pos.y = y;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 0.0f;
	buffer->uv.y = 0.0f;
	buffer->texid = 0;
	buffer++;

	buffer->pos.x = x;
	buffer->pos.y = y + height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 0.0f;
	buffer->uv.y = 1.0f;
	buffer->texid = 0;
	buffer++;

	buffer->pos.x = x + width;
	buffer->pos.y = y + height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 1.0f;
	buffer->uv.y = 1.0f;
	buffer->texid = 0;
	buffer++;

	buffer->pos.x = x + width;
	buffer->pos.y = y;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 1.0f;
	buffer->uv.y = 0.0f;
	buffer->texid = 0;
	buffer++;

	indexcount += 6;
}

void draw_rectangle(i32 x, i32 y, i32 width, i32 height, vec4 color) {
	f32 r = color.x / 255.0f;
	f32 g = color.y / 255.0f;
	f32 b = color.z / 255.0f;
	f32 a = color.w / 255.0f;

	buffer->pos.x = x;
	buffer->pos.y = y;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 0.0f;
	buffer->uv.y = 0.0f;
	buffer->texid = 0;
	buffer++;

	buffer->pos.x = x;
	buffer->pos.y = y + height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 0.0f;
	buffer->uv.y = 1.0f;
	buffer->texid = 0;
	buffer++;

	buffer->pos.x = x + width;
	buffer->pos.y = y + height;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 1.0f;
	buffer->uv.y = 1.0f;
	buffer->texid = 0;
	buffer++;

	buffer->pos.x = x + width;
	buffer->pos.y = y;
	buffer->color.x = r;
	buffer->color.y = g;
	buffer->color.z = b;
	buffer->color.w = a;
	buffer->uv.x = 1.0f;
	buffer->uv.y = 0.0f;
	buffer->texid = 0;
	buffer++;

	indexcount += 6;
}

void draw_text(Font& font, const char* str, i32 xPos, i32 yPos, f32 r, f32 g, f32 b) {
	r /= 255;
	g /= 255;
	b /= 255;

	u32 len = strlen(str);
	for (u32 i = 0; i < len; ++i) {
		Character* c = font.characters[str[i]];
		int yOffset = (font.characters['T']->bearing.y - c->bearing.y) + 1;
		if (yOffset < 0) yOffset = 0;

		int x = xPos + c->bearing.x;
		int y = yPos + yOffset;

		Texture tex = font.characters[str[i]]->texture;
		int texSlot = submit_tex(tex);
		GLfloat* uvs;
		uvs = DEFAULT_UVS;

		buffer->pos.x = x;
		buffer->pos.y = y;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[0];
		buffer->uv.y = uvs[1];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x;
		buffer->pos.y = y + tex.height;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[2];
		buffer->uv.y = uvs[3];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x + tex.width;
		buffer->pos.y = y + tex.height;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[4];
		buffer->uv.y = uvs[5];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x + tex.width;
		buffer->pos.y = y;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[6];
		buffer->uv.y = uvs[7];
		buffer->texid = texSlot;
		buffer++;

		indexcount += 6;
		xPos += (font.characters[str[i]]->advance >> 6);
	}
}

void draw_text(Font& font, std::string str, i32 xPos, i32 yPos, f32 r, f32 g, f32 b) {
	r /= 255;
	g /= 255;
	b /= 255;

	for (u16 i = 0; i < str.size(); ++i) {
		Character* c = font.characters[str[i]];
		int yOffset = (font.characters['T']->bearing.y - c->bearing.y) + 1;
		if (yOffset < 0) yOffset = 0;

		int x = xPos + c->bearing.x;
		int y = yPos + yOffset;

		Texture tex = font.characters[str[i]]->texture;
		int texSlot = submit_tex(tex);
		GLfloat* uvs;
		uvs = DEFAULT_UVS;

		buffer->pos.x = x;
		buffer->pos.y = y;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[0];
		buffer->uv.y = uvs[1];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x;
		buffer->pos.y = y + tex.height;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[2];
		buffer->uv.y = uvs[3];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x + tex.width;
		buffer->pos.y = y + tex.height;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[4];
		buffer->uv.y = uvs[5];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x + tex.width;
		buffer->pos.y = y;
		buffer->color.x = r;
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[6];
		buffer->uv.y = uvs[7];
		buffer->texid = texSlot;
		buffer++;

		indexcount += 6;
		xPos += (font.characters[str[i]]->advance >> 6);
	}
}

void end2D() {
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (u16 i = 0; i < texcount; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		upload_int(shader, locations[i], i);
	}

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //color
	glEnableVertexAttribArray(2); //texture coordinates
	glEnableVertexAttribArray(3); //texture ID

	glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0); //position
	glDisableVertexAttribArray(1); //color
	glDisableVertexAttribArray(2); //texture coordinates
	glDisableVertexAttribArray(3); //textureID
	glBindVertexArray(0);

	for (u16 i = 0; i < texcount; ++i)
		unbind_texture(textures[i]);

	indexcount = 0;
	texcount = 0;

	stop_shader();
}

f32 get_blackbar_width(f32 aspect) {
	if (aspect == 0) aspect = 1;
	f32 screen_width = get_window_width();
	f32 screen_height = get_window_height();

	i32 new_width = screen_width;
	i32 new_height = (i32)(screen_width / aspect);
	if (new_height > screen_height) {
		new_height = screen_height;
		new_width = (i32)(screen_height * aspect);
	}
	return (screen_width - new_width) / 2;
}

f32 get_blackbar_height(f32 aspect) {
	if (aspect == 0) aspect = 1;
	f32 screen_width = get_window_width();
	f32 screen_height = get_window_height();

	i32 new_width = screen_width;
	i32 new_height = (i32)(screen_width / aspect);
	if (new_height > screen_height) {
		new_height = screen_height;
		new_width = (i32)(screen_height * aspect);
	}
	return (screen_height - new_height) / 2;
}

Rect fit_aspect_ratio(f32 aspect) {
	if (aspect == 0) aspect = 1;
	f32 screen_width = get_window_width();
	f32 screen_height = get_window_height();

	i32 new_width = screen_width;
	i32 new_height = (i32)(screen_width / aspect);
	if (new_height > screen_height) {
		new_height = screen_height;
		new_width = (i32)(screen_height * aspect);
	}
	return rect((screen_width - new_width) / 2, (screen_height - new_height) / 2, new_width, new_height);
}

void dispose2D() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	dispose_shader(shader);
}

#if defined(BMT_USE_NAMESPACE) 
}
#endif