///////////////////////////////////////////////////////////////////////////
// FILE:                      render2D.cpp                               //
///////////////////////////////////////////////////////////////////////////
//                      BAHAMUT GRAPHICS LIBRARY                         //
//                        Author: Corbin Stark                           //
///////////////////////////////////////////////////////////////////////////
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

//I'm using globals because, let's be honest, singletons are just globals for people too afraid to use globals
GLuint BMT_vao;
GLuint BMT_vbo;
GLuint BMT_ebo;
unsigned short BMT_indexcount;
unsigned short BMT_texcount;
GLuint BMT_textures[BATCH_MAX_TEXTURES];
GLchar* BMT_locations[BATCH_MAX_TEXTURES];
VertexData* BMT_buffer;

BMTStretchMode BMT_stretch_mode;
BMTAspectMode BMT_aspect_mode;

Shader BMT_shader;
mat4f BMT_ortho_projection;

GLfloat BMT_DEFAULT_UVS[8] = {
	0, 0, 0, 1,
	1, 1, 1, 0
};
GLfloat BMT_FLIP_HORIZONTAL_UVS[8] = {
	1, 0, 1, 1,
	0, 1, 0, 0
};
GLfloat BMT_FLIP_VERTICAL_UVS[8] = {
	0, 1, 0, 0,
	1, 0, 1, 1
};
GLfloat BMT_FLIP_BOTH_UVS[8] = {
	1, 1, 1, 0,
	0, 0, 0, 1
};

int submitTex(Texture& tex) {
	int texSlot = 0;
	bool found = false;
	for (int i = 0; i < BMT_texcount; ++i) {
		if (BMT_textures[i] == tex.ID) {
			texSlot = (i + 1);
			found = true;
			break;
		}
	}
	if (!found) {
		if (BMT_texcount >= BATCH_MAX_TEXTURES) {
			end2D();
			begin2D();
		}
		BMT_textures[BMT_texcount++] = tex.ID;
		texSlot = BMT_texcount;
	}
	return texSlot;
}

void init2D(int x, int y, int width, int height) {
	BMT_stretch_mode = STRETCH_NONE;
	BMT_aspect_mode = ASPECT_NONE;
	set2DRenderViewport(x, y, width, height, getVirtualWidth(), getVirtualHeight());
	glViewport(x, y, width, height);
	BMT_ortho_projection = mat4f::orthographic(x, y, width, height, -10.0f, 10.0f);
	BMT_texcount = BMT_indexcount = 0;

	//LOAD SHADER - CUSTOM LOAD BECAUSE ATTRIB LOCATIONS MUST BE BOUND MANUALLY TO SUPPORT EARLIER VERSIONS
	BMT_shader.vertexshaderID = loadShaderString(ORTHO_SHADER_VERT_SHADER, GL_VERTEX_SHADER);
	BMT_shader.fragshaderID = loadShaderString(ORTHO_SHADER_FRAG_SHADER, GL_FRAGMENT_SHADER);
	BMT_shader.programID = glCreateProgram();
	glAttachShader(BMT_shader.programID, BMT_shader.vertexshaderID);
	glAttachShader(BMT_shader.programID, BMT_shader.fragshaderID);
	glBindFragDataLocation(BMT_shader.programID, 0, "outColor");
	glBindAttribLocation(BMT_shader.programID, 0, "position");
	glBindAttribLocation(BMT_shader.programID, 1, "color");
	glBindAttribLocation(BMT_shader.programID, 2, "uv");
	glBindAttribLocation(BMT_shader.programID, 3, "texid");
	glLinkProgram(BMT_shader.programID);
	glValidateProgram(BMT_shader.programID);
	glUseProgram(0);
	//LOAD SHADER - CUSTOM LOAD BECAUSE ATTRIB LOCATIONS MUST BE BOUND MANUALLY TO SUPPORT EARLIER VERSIONS
	for (int i = 0; i < BATCH_MAX_TEXTURES; ++i)
		BMT_textures[i] = 0;

	BMT_locations[0] = "tex1";
	BMT_locations[1] = "tex2";
	BMT_locations[2] = "tex3";
	BMT_locations[3] = "tex4";
	BMT_locations[4] = "tex5";
	BMT_locations[5] = "tex6";
	BMT_locations[6] = "tex7";
	BMT_locations[7] = "tex8";
	BMT_locations[8] = "tex9";
	BMT_locations[9] = "tex10";
	BMT_locations[10] = "tex11";
	BMT_locations[11] = "tex12";
	BMT_locations[12] = "tex13";
	BMT_locations[13] = "tex14";
	BMT_locations[14] = "tex15";
	BMT_locations[15] = "tex16";

	glGenVertexArrays(1, &BMT_vao);
	glBindVertexArray(BMT_vao);

	glGenBuffers(1, &BMT_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, BMT_vbo);
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

	glGenBuffers(1, &BMT_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BMT_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, BATCH_INDICE_SIZE * sizeof(GLushort), indices, GL_STATIC_DRAW);

	//the vao must be unbound before the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void begin2D() {
	glBindBuffer(GL_ARRAY_BUFFER, BMT_vbo);
	BMT_buffer = (VertexData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, BATCH_BUFFER_SIZE,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
	);
}

void drawTexture(Texture& tex, int xPos, int yPos) {
	if (tex.ID == 0)
		return;
	int texSlot = submitTex(tex);
	GLfloat* uvs;

	uvs = BMT_DEFAULT_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
		uvs = BMT_FLIP_BOTH_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL)
		uvs = BMT_FLIP_HORIZONTAL_UVS;
	if (tex.flip_flag & FLIP_VERTICAL)
		uvs = BMT_FLIP_VERTICAL_UVS;

	BMT_buffer->pos.x = xPos;
	BMT_buffer->pos.y = yPos;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[0];
	BMT_buffer->uv.y = uvs[1];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = xPos;
	BMT_buffer->pos.y = yPos + tex.height;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[2];
	BMT_buffer->uv.y = uvs[3];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = xPos + tex.width;
	BMT_buffer->pos.y = yPos + tex.height;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[4];
	BMT_buffer->uv.y = uvs[5];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = xPos + tex.width;
	BMT_buffer->pos.y = yPos;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[6];
	BMT_buffer->uv.y = uvs[7];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_indexcount += 6;
}
void drawTexture(Texture& tex, int xPos, int yPos, float r, float g, float b, float a) {
	if (tex.ID == 0)
		return;

	r /= 255;
	g /= 255;
	b /= 255;
	a /= 255;

	int texSlot = submitTex(tex);
	GLfloat* uvs;

	uvs = BMT_DEFAULT_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
		uvs = BMT_FLIP_BOTH_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL)
		uvs = BMT_FLIP_HORIZONTAL_UVS;
	if (tex.flip_flag & FLIP_VERTICAL)
		uvs = BMT_FLIP_VERTICAL_UVS;

	BMT_buffer->pos.x = xPos;
	BMT_buffer->pos.y = yPos;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = uvs[0];
	BMT_buffer->uv.y = uvs[1];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = xPos;
	BMT_buffer->pos.y = yPos + tex.height;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = uvs[2];
	BMT_buffer->uv.y = uvs[3];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = xPos + tex.width;
	BMT_buffer->pos.y = yPos + tex.height;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = uvs[4];
	BMT_buffer->uv.y = uvs[5];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = xPos + tex.width;
	BMT_buffer->pos.y = yPos;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = uvs[6];
	BMT_buffer->uv.y = uvs[7];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_indexcount += 6;
}
void drawTextureRot(Texture& tex, int xPos, int yPos, float rotateDegree) {
	float originX = xPos + (tex.width / 2);
	float originY = yPos + (tex.height / 2);
	drawTextureRot(tex, xPos, yPos, vec2f(originX, originY), rotateDegree);
}
void drawTextureRot(Texture& tex, int xPos, int yPos, vec2f origin, float rotation) {
	if (tex.ID == 0)
		return;
	int texSlot = submitTex(tex);
	GLfloat* uvs;

	uvs = BMT_DEFAULT_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL && tex.flip_flag & FLIP_VERTICAL)
		uvs = BMT_FLIP_BOTH_UVS;
	if (tex.flip_flag & FLIP_HORIZONTAL)
		uvs = BMT_FLIP_HORIZONTAL_UVS;
	if (tex.flip_flag & FLIP_VERTICAL)
		uvs = BMT_FLIP_VERTICAL_UVS;

	float originX = origin.x;
	float originY = origin.y;

	//degToRad is expensive don't do it if there is no rotation
	if (rotation != 0)
		rotation = Math::degToRad(rotation);

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
	BMT_buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	BMT_buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[0];
	BMT_buffer->uv.x = uvs[1];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	newX = xPos;
	newY = yPos + tex.height;
	BMT_buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	BMT_buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[2];
	BMT_buffer->uv.x = uvs[3];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	newX = xPos + tex.width;
	newY = yPos + tex.height;
	BMT_buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	BMT_buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[4];
	BMT_buffer->uv.x = uvs[5];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	newX = xPos + tex.width;
	newY = yPos;
	BMT_buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	BMT_buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	BMT_buffer->color.x = 1; 
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[6];
	BMT_buffer->uv.x = uvs[7];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_indexcount += 6;
}
void drawTextureEX(Texture& tex, Rect source, Rect dest) {
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

	int texSlot = submitTex(tex);

	BMT_buffer->pos.x = dest.x;
	BMT_buffer->pos.y = dest.y;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[0];
	BMT_buffer->uv.y = uvs[1];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = dest.x;
	BMT_buffer->pos.y = dest.y + dest.height;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[2];
	BMT_buffer->uv.y = uvs[3];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = dest.x + dest.width;
	BMT_buffer->pos.y = dest.y + dest.height;
	BMT_buffer->color.x = 1;
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[4];
	BMT_buffer->uv.y = uvs[5];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_buffer->pos.x = dest.x + dest.width;
	BMT_buffer->pos.y = dest.y;
	BMT_buffer->color.x = 1; 
	BMT_buffer->color.y = 1;
	BMT_buffer->color.z = 1;
	BMT_buffer->color.w = 1;
	BMT_buffer->uv.x = uvs[6];
	BMT_buffer->uv.y = uvs[7];
	BMT_buffer->texid = texSlot;
	BMT_buffer++;

	BMT_indexcount += 6;
}
void drawRectangle(int x, int y, int width, int height, float r, float g, float b, float a) {
	r /= 255.0f;
	g /= 255.0f;
	b /= 255.0f;
	a /= 255.0f;

	BMT_buffer->pos.x = x;
	BMT_buffer->pos.y = y;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 0.0f;
	BMT_buffer->uv.y = 0.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_buffer->pos.x = x;
	BMT_buffer->pos.y = y + height;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 0.0f;
	BMT_buffer->uv.y = 1.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_buffer->pos.x = x + width;
	BMT_buffer->pos.y = y + height;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 1.0f;
	BMT_buffer->uv.y = 1.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_buffer->pos.x = x + width;
	BMT_buffer->pos.y = y;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 1.0f;
	BMT_buffer->uv.y = 0.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_indexcount += 6;
}
void drawRectangle(int x, int y, int width, int height, vec4f& color) {
	float r = color.x / 255.0f;
	float g = color.y / 255.0f;
	float b = color.z / 255.0f;
	float a = color.w / 255.0f;

	BMT_buffer->pos.x = x;
	BMT_buffer->pos.y = y;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 0.0f;
	BMT_buffer->uv.y = 0.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_buffer->pos.x = x;
	BMT_buffer->pos.y = y + height;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 0.0f;
	BMT_buffer->uv.y = 1.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_buffer->pos.x = x + width;
	BMT_buffer->pos.y = y + height;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 1.0f;
	BMT_buffer->uv.y = 1.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_buffer->pos.x = x + width;
	BMT_buffer->pos.y = y;
	BMT_buffer->color.x = r;
	BMT_buffer->color.y = g;
	BMT_buffer->color.z = b;
	BMT_buffer->color.w = a;
	BMT_buffer->uv.x = 1.0f;
	BMT_buffer->uv.y = 0.0f;
	BMT_buffer->texid = 0;
	BMT_buffer++;

	BMT_indexcount += 6;
}
void drawText(Font& font, std::string str, int xPos, int yPos) {
	for (int i = 0; i < str.size(); ++i) {
		Character* c = font.characters[str[i]];
		int yOffset = (font.characters['T']->bearing.y - c->bearing.y) + 1;
		if (yOffset < 0) yOffset = 0;

		int x = xPos + c->bearing.x;
		int y = yPos + yOffset;

		drawTexture(font.characters[str[i]]->texture, x, y);
		xPos += (font.characters[str[i]]->advance >> 6);
	}
}
void drawText(Font& font, std::string str, int xPos, int yPos, float r, float g, float b) {
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
		int texSlot = submitTex(tex);
		GLfloat* uvs;
		uvs = BMT_DEFAULT_UVS;

		BMT_buffer->pos.x = x;
		BMT_buffer->pos.y = y;
		BMT_buffer->color.x = r;
		BMT_buffer->color.y = g;
		BMT_buffer->color.z = b;
		BMT_buffer->color.w = 1;
		BMT_buffer->uv.x = uvs[0];
		BMT_buffer->uv.y = uvs[1];
		BMT_buffer->texid = texSlot;
		BMT_buffer++;

		BMT_buffer->pos.x = x;
		BMT_buffer->pos.y = y + tex.height;
		BMT_buffer->color.x = r;
		BMT_buffer->color.y = g;
		BMT_buffer->color.z = b;
		BMT_buffer->color.w = 1;
		BMT_buffer->uv.x = uvs[2];
		BMT_buffer->uv.y = uvs[3];
		BMT_buffer->texid = texSlot;
		BMT_buffer++;

		BMT_buffer->pos.x = x + tex.width;
		BMT_buffer->pos.y = y + tex.height;
		BMT_buffer->color.x = r;
		BMT_buffer->color.y = g;
		BMT_buffer->color.z = b;
		BMT_buffer->color.w = 1;
		BMT_buffer->uv.x = uvs[4];
		BMT_buffer->uv.y = uvs[5];
		BMT_buffer->texid = texSlot;
		BMT_buffer++;

		BMT_buffer->pos.x = x + tex.width;
		BMT_buffer->pos.y = y;
		BMT_buffer->color.x = r; 
		BMT_buffer->color.y = g;
		BMT_buffer->color.z = b;
		BMT_buffer->color.w = 1;
		BMT_buffer->uv.x = uvs[6];
		BMT_buffer->uv.y = uvs[7];
		BMT_buffer->texid = texSlot;
		BMT_buffer++;

		BMT_indexcount += 6;
		xPos += (font.characters[str[i]]->advance >> 6);
	}
}
void end2D() {
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	startShader(&BMT_shader);
	loadMat4f(&BMT_shader, "pr_matrix", BMT_ortho_projection);

	for (int i = 0; i < BMT_texcount; ++i) {
		//if (textures[i] != 0) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, BMT_textures[i]);
		loadInt(&BMT_shader, BMT_locations[i], i);
		//}
	}

	glBindVertexArray(BMT_vao);
	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //color
	glEnableVertexAttribArray(2); //texture coordinates
	glEnableVertexAttribArray(3); //texture ID

	glDrawElements(GL_TRIANGLES, BMT_indexcount, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0); //position
	glDisableVertexAttribArray(1); //color
	glDisableVertexAttribArray(2); //texture coordinates
	glDisableVertexAttribArray(3); //textureID
	glBindVertexArray(0);

	for (int i = 0; i < BMT_texcount; ++i)
		unbindTexture(BMT_textures[i]);

	BMT_indexcount = 0;
	BMT_texcount = 0;

	stopShader();
}

void setStretchMode(BMTStretchMode mode) {
	BMT_stretch_mode = mode;
}

void setAspectMode(BMTAspectMode mode) {
	BMT_aspect_mode = mode;
}

void limitViewportToAspectRatio(float aspect, float screen_width, float screen_height) {
	int new_width = screen_width;
	int new_height = (int)(screen_width / aspect);
	if (new_height > screen_height) {
		new_height = screen_height;
		new_width = (int)(screen_height * aspect);
	}
	glViewport((screen_width - new_width) / 2, (screen_height - new_height) / 2, new_width, new_height);
}

void set2DRenderViewport(int x, int y, int width, int height, int virtual_width, int virtual_height) {
	if (virtual_height == 0) virtual_height = 1;

	if (BMT_stretch_mode == STRETCH_NONE) {
		BMT_ortho_projection = mat4f::orthographic(x, y, width, height, -10.0f, 10.0f);
		glViewport(x, y, width, height);
		//if there is no stretching, ignore aspect ratio.
	}

	//TODO: Fix projection stretch
	if (BMT_stretch_mode == STRETCH_PROJECTION) {
		if (BMT_aspect_mode == ASPECT_NONE) {
			BMT_ortho_projection = mat4f::orthographic(x, y, width, height, -10.0f, 10.0f);
		}
		if (BMT_aspect_mode == ASPECT_KEEP) {
			float aspect = (float)virtual_width / (float)virtual_height;

			int new_width = width;
			int new_height = (int)(width / aspect);
			if (new_height > height) {
				new_height = height;
				new_width = (int)(height * aspect);
			}
			BMT_ortho_projection = mat4f::orthographic((width - new_width) / 2, (height - new_height) / 2, new_width, new_height, -10.0f, 10.0f);
		}
	}

	if (BMT_stretch_mode == STRETCH_VIEWPORT) {
		if (BMT_aspect_mode == ASPECT_NONE) {
			glViewport(x, y, width, height);
		}
		if (BMT_aspect_mode == ASPECT_KEEP) {
			float aspect = (float)virtual_width / (float)virtual_height;
			limitViewportToAspectRatio(aspect, width, height);
		}
	}
}

void attachShader2D(Shader shader_in) {
	BMT_shader = shader_in;
}

void dispose2D() {
	glDeleteVertexArrays(1, &BMT_vao);
	glDeleteBuffers(1, &BMT_vbo);
	glDeleteBuffers(1, &BMT_ebo);
	disposeShader(BMT_shader);
}