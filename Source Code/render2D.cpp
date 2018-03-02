///////////////////////////////////////////////////////////////////////////
// FILE:			          render2D.cpp                               //
///////////////////////////////////////////////////////////////////////////
//                      BAHAMUT GRAPHICS LIBRARY                         //
//						  Author: Corbin Stark							 //
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

GLuint vao;
GLuint vbo;
GLuint ebo;
unsigned short indexcount;
unsigned short texcount;
GLuint textures[BATCH_MAX_TEXTURES];
GLchar* locations[BATCH_MAX_TEXTURES];
VertexData* buffer;

Shader shader;
mat4f ortho_projection;

GLfloat DEFAULT_UVS[8] = {
	0, 0, 0, 1,
	1, 1, 1, 0
};
GLfloat FLIP_HORIZONTAL_UVS[8] = {
	1, 0, 1, 1,
	0, 1, 0, 0
};
GLfloat FLIP_VERTICAL_UVS[8] = {
	0, 1, 0, 0,
	1, 0, 1, 1
};
GLfloat FLIP_BOTH_UVS[8] = {
	1, 1, 1, 0,
	0, 0, 0, 1
};

int submitTex(Texture& tex) {
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

void init2D(int x, int y, int width, int height) {
	set2DRenderViewport(x, y, width, height);
	texcount = indexcount = 0;

	//LOAD SHADER - CUSTOM LOAD BECAUSE ATTRIB LOCATIONS MUST BE BOUND MANUALLY TO SUPPORT EARLIER VERSIONS
	shader.vertexshaderID = loadShaderString(ORTHO_SHADER_VERT_SHADER, GL_VERTEX_SHADER);
	shader.fragshaderID = loadShaderString(ORTHO_SHADER_FRAG_SHADER, GL_FRAGMENT_SHADER);
	shader.programID = glCreateProgram();
	glAttachShader(shader.programID, shader.vertexshaderID);
	glAttachShader(shader.programID, shader.fragshaderID);
	glBindFragDataLocation(shader.programID, 0, "outColor");
	glBindAttribLocation(shader.programID, 0, "position");
	glBindAttribLocation(shader.programID, 1, "color");
	glBindAttribLocation(shader.programID, 2, "uv");
	glBindAttribLocation(shader.programID, 3, "texid");
	glLinkProgram(shader.programID);
	glValidateProgram(shader.programID);
	glUseProgram(0);
	//LOAD SHADER - CUSTOM LOAD BECAUSE ATTRIB LOCATIONS MUST BE BOUND MANUALLY TO SUPPORT EARLIER VERSIONS
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, BATCH_VERTEX_SIZE, (const GLvoid*)0);					       //vertices
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

	//the vao must be unbound before the buffers..... don't ask me why
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void begin2D() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	buffer = (VertexData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, BATCH_BUFFER_SIZE, 
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
	);
}

void drawTexture(Texture& tex, int xPos, int yPos) {
	if (tex.ID == 0)
		return;
	int texSlot = submitTex(tex);
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
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos;
	buffer->pos.y = yPos + tex.height;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + tex.width;
	buffer->pos.y = yPos + tex.height;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = xPos + tex.width;
	buffer->pos.y = yPos;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[6];
	buffer->uv.y = uvs[7];
	buffer->texid = texSlot;
	buffer++;

	indexcount += 6;
}
void drawTexture(Texture& tex, int xPos, int yPos, float r, float g, float b, float a) {
	if (tex.ID == 0)
		return;

	r /= 255;
	g /= 255;
	b /= 255;
	a /= 255;

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
	buffer->pos.x = cosineDegree * (newX - originX) - sineDegree * (newY - originY) + originX;
	buffer->pos.y = sineDegree * (newX - originX) + cosineDegree * (newY - originY) + originY;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
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
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
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
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
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
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[6];
	buffer->uv.x = uvs[7];
	buffer->texid = texSlot;
	buffer++;

	indexcount += 6;
}
void drawTextureEX(Texture& tex, Rectangle source, Rectangle dest) {
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

	//FORCIBLY INLINED submitTex FOR PERFORMANCE
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
	//FORCIBLY INLINED submitTex FOR PERFORMANCE

	buffer->pos.x = dest.x;
	buffer->pos.y = dest.y;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[0];
	buffer->uv.y = uvs[1];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x;
	buffer->pos.y = dest.y + dest.height;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[2];
	buffer->uv.y = uvs[3];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x + dest.width;
	buffer->pos.y = dest.y + dest.height;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[4];
	buffer->uv.y = uvs[5];
	buffer->texid = texSlot;
	buffer++;

	buffer->pos.x = dest.x + dest.width;
	buffer->pos.y = dest.y;
	buffer->color.x = 1; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
	buffer->color.y = 1;
	buffer->color.z = 1;
	buffer->color.w = 1;
	buffer->uv.x = uvs[6];
	buffer->uv.y = uvs[7];
	buffer->texid = texSlot;
	buffer++;

	indexcount += 6;
}
void drawRectangle(int x, int y, int width, int height, float r, float g, float b, float a) {
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
void drawRectangle(int x, int y, int width, int height, vec4f& color) {
	float r = color.x / 255.0f;
	float g = color.y / 255.0f;
	float b = color.z / 255.0f;
	float a = color.w / 255.0f;

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
		uvs = DEFAULT_UVS;

		buffer->pos.x = x;
		buffer->pos.y = y;
		buffer->color.x = r; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[0];
		buffer->uv.y = uvs[1];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x;
		buffer->pos.y = y + tex.height;
		buffer->color.x = r; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[2];
		buffer->uv.y = uvs[3];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x + tex.width;
		buffer->pos.y = y + tex.height;
		buffer->color.x = r; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
		buffer->color.y = g;
		buffer->color.z = b;
		buffer->color.w = 1;
		buffer->uv.x = uvs[4];
		buffer->uv.y = uvs[5];
		buffer->texid = texSlot;
		buffer++;

		buffer->pos.x = x + tex.width;
		buffer->pos.y = y;
		buffer->color.x = r; //images don't need color since they are textured. Although if the texture is not found it will be a black square.
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

	startShader(&shader);
	loadMat4f(&shader, "pr_matrix", ortho_projection);

	for (int i = 0; i < texcount; ++i) {
		//if (textures[i] != 0) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			loadInt(&shader, locations[i], i);
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
		unbindTexture(textures[i]);

	indexcount = 0;
	texcount = 0;

	stopShader();
}

void set2DRenderViewport(int x, int y, int width, int height) {
	x = x;
	y = y;
	width = width;
	height = height;
	ortho_projection = mat4f::orthographic(x, y, width, height, -10.0f, 10.0f);
	glViewport(x, y, width, height);
}
void attachShader2D(Shader shader_in) {
	shader = shader_in;
}

void dispose2D() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	disposeShader(shader);
}