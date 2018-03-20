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
#include "orthoshader.h"
#include "window.h"

INTERNAL GLuint vao;
INTERNAL GLuint vbo;
INTERNAL GLuint ebo;
INTERNAL u16 indexcount;
INTERNAL u16 texcount;
INTERNAL GLuint  textures[BATCH_MAX_TEXTURES];
INTERNAL GLchar* locations[BATCH_MAX_TEXTURES];
INTERNAL VertexData* buffer;

INTERNAL StretchMode stretchMode;
INTERNAL AspectMode aspectMode;

INTERNAL Shader shader;
INTERNAL mat4 orthoProjection;
INTERNAL mat4 customProjection;
INTERNAL bool usingCustomProjection;

INTERNAL Rect viewportRect;

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
	for (int i = 0; i < texcount; ++i) {
		if (textures[i] == tex.ID) {
			texSlot = (i + 1);
			found = true;
			break;
		}
	}
	if (!found) {
		if (texcount >= BATCH_MAX_TEXTURES) {
			end2D();
			begin2D();
		}
		textures[texcount++] = tex.ID;
		texSlot = texcount;
	}
	return texSlot;
}

void init2D(i32 x, i32 y, u32 width, u32 height) {
	stretchMode = STRETCH_NONE;
	aspectMode = ASPECT_NONE;
	set_2D_render_viewport(x, y, width, height, get_virtual_width(), get_virtual_height());
	texcount = indexcount = 0;

	shader.vertexshaderID = load_shader_string(ORTHO_SHADER_VERT_SHADER, GL_VERTEX_SHADER);
	shader.fragshaderID = load_shader_string(ORTHO_SHADER_FRAG_SHADER, GL_FRAGMENT_SHADER);
	shader.ID = glCreateProgram();
	glAttachShader(shader.ID, shader.vertexshaderID);
	glAttachShader(shader.ID, shader.fragshaderID);
	glBindFragDataLocation(shader.ID, 0, "outColor");
	glBindAttribLocation(shader.ID,   0, "position");
	glBindAttribLocation(shader.ID,   1, "color");
	glBindAttribLocation(shader.ID,   2, "uv");
	glBindAttribLocation(shader.ID,   3, "texid");
	glLinkProgram(shader.ID);
	glValidateProgram(shader.ID);
	glUseProgram(0);
	for (int i = 0; i < BATCH_MAX_TEXTURES; ++i)
		textures[i] = 0;

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
	for (int i = 0; i < BATCH_INDICE_SIZE; i += 6) {
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

void begin2D() {
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glViewport(viewportRect.x, viewportRect.y, viewportRect.width, viewportRect.height);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	buffer = (VertexData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, BATCH_BUFFER_SIZE,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
	);
}

void begin2D(mat4 projection) {
	customProjection = projection;
	usingCustomProjection = true;
	begin2D();
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
	buffer->uv.x = uvs[1];
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
	buffer->uv.x = uvs[3];
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
	buffer->uv.x = uvs[5];
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
	buffer->uv.x = uvs[7];
	buffer->texid = texSlot;
	buffer++;
	
	indexcount += 6;
}

void draw_texture_EX(Texture tex, Rect source, Rect dest) {
	if (tex.ID == 0)
		return;

	static GLfloat EX_UVS[8];
	EX_UVS[0] = source.x / tex.width;
	EX_UVS[1] = source.y / tex.height;
	EX_UVS[2] = source.x / tex.width;
	EX_UVS[3] = (source.y + source.height) / tex.height;
	EX_UVS[4] = (source.x + source.width) / tex.width;
	EX_UVS[5] = (source.y + source.height) / tex.height;
	EX_UVS[6] = (source.x + source.width) / tex.width;
	EX_UVS[7] = source.y / tex.height;
	GLfloat* uvs = EX_UVS;

	int texSlot = submit_tex(tex);

	buffer->pos.x = dest.x;
	buffer->pos.y = dest.y;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x;
	buffer->pos.y = dest.y + dest.height;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x + dest.width;
	buffer->pos.y = dest.y + dest.height;
	buffer->color.x = 1;
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x + dest.width;
	buffer->pos.y = dest.y;
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

void draw_text(Font& font, std::string str, i32 xPos, i32 yPos) {
	for (int i = 0; i < str.size(); ++i) {
		Character* c = font.characters[str[i]];
		int yOffset = (font.characters['T']->bearing.y - c->bearing.y) + 1;
		if (yOffset < 0) yOffset = 0;

		int x = xPos + c->bearing.x;
		int y = yPos + yOffset;

		draw_texture(font.characters[str[i]]->texture, x, y);
		xPos += (font.characters[str[i]]->advance >> 6);
	}
}

void draw_text(Font& font, std::string str, i32 xPos, i32 yPos, f32 r, f32 g, f32 b) {
	r /= 255;
	g /= 255;
	b /= 255;

	for (int i = 0; i < str.size(); ++i) {
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

	start_shader(&shader);
	if (usingCustomProjection)
		load_mat4(&shader, "pr_matrix", customProjection);
	else
		load_mat4(&shader, "pr_matrix", orthoProjection);
	usingCustomProjection = false;

	for (int i = 0; i < texcount; ++i) {
		//if (textures[i] != 0) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		load_int(&shader, locations[i], i);
		//}
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

	for (int i = 0; i < texcount; ++i)
		unbind_texture(textures[i]);

	indexcount = 0;
	texcount = 0;

	stop_shader();
}

void set_stretch_mode(StretchMode mode) {
	stretchMode = mode;
}

void set_aspect_mode(AspectMode mode) {
	aspectMode = mode;
}

Rect get_viewport_rect() {
	return viewportRect;
}

INTERNAL 
void limit_viewport_to_aspect_ratio(f32 aspect, f32 screen_width, f32 screen_height) {
	if (aspect == 0) aspect = 1;

	i32 new_width = screen_width;
	i32 new_height = (i32)(screen_width / aspect);
	if (new_height > screen_height) {
		new_height = screen_height;
		new_width = (i32)(screen_height * aspect);
	}
	glViewport((screen_width - new_width) / 2, (screen_height - new_height) / 2, new_width, new_height);
	viewportRect = rect((screen_width - new_width) / 2, (screen_height - new_height) / 2, new_width, new_height);
}

void set_2D_render_viewport(i32 x, i32 y, u32 width, u32 height, u32 virtual_width, u32 virtual_height) {
	if (virtual_height == 0) virtual_height = 1;

	if (stretchMode == STRETCH_NONE) {
		orthoProjection = orthographic_projection(x, y, width, height, -10.0f, 10.0f);
		set_virtual_size(width, height);
		viewportRect = rect(x, y, width, height);
		if (aspectMode == ASPECT_NONE) {
			glViewport(x, y, width, height);
		}
		if (aspectMode == ASPECT_KEEP) {
			f32 aspect = (f32)virtual_width / (f32)virtual_height;
			limit_viewport_to_aspect_ratio(aspect, width, height);
		}
		if (aspectMode == ASPECT_KEEP_WIDTH) {

		}
		if (aspectMode == ASPECT_KEEP_HEIGHT) {

		}
	}

	//TODO: Fix projection stretch
	if (stretchMode == STRETCH_PROJECTION) {
		if (aspectMode == ASPECT_NONE) {
			orthoProjection = orthographic_projection(x, y, width, height, -10.0f, 10.0f);
		}
		if (aspectMode == ASPECT_KEEP) {
			f32 aspect = (f32)virtual_width / (f32)virtual_height;

			i32 new_width = width;
			i32 new_height = (i32)(width / aspect);
			if (new_height > height) {
				new_height = height;
				new_width = (i32)(height * aspect);
			}
			orthoProjection = orthographic_projection((width - new_width) / 2, (height - new_height) / 2, new_width, new_height, -10.0f, 10.0f);
		}
	}

	if (stretchMode == STRETCH_VIEWPORT) {
		if (aspectMode == ASPECT_NONE) {
			glViewport(x, y, width, height);
		}
		if (aspectMode == ASPECT_KEEP) {
			f32 aspect = (f32)virtual_width / (f32)virtual_height;
			limit_viewport_to_aspect_ratio(aspect, width, height);
		}
		if (aspectMode == ASPECT_KEEP_WIDTH) {
			f32 aspect = (f32)virtual_width / (f32)virtual_height;
			limit_viewport_to_aspect_ratio(aspect, width, height);
		}
		if (aspectMode == ASPECT_KEEP_HEIGHT) {
			f32 aspect = (f32)virtual_width / (f32)virtual_height;
			limit_viewport_to_aspect_ratio(aspect, width, height);
		}
	}
}

void dispose2D() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	dispose_shader(shader);
}