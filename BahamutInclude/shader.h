#pragma once
#include "image.h"

struct Shader {
	GLuint programID;
	GLuint vertexshaderID;
	GLuint fragshaderID;
};

struct Shader2D : public Shader {
	void(*prepareRenderCallback)(Texture& image);
};

struct Shader3D : public Shader {
	//void(*prepareRenderCallback)(Model& image);
};

Shader loadShader(const GLchar* vertexfile, const GLchar* fragmentfile);
Shader loadShaderFromStrings(const GLchar* vertexstring, const GLchar* fragmentstring);
GLuint loadShaderFile(const GLchar* path, GLuint type);
GLuint loadShaderString(const GLchar* string, GLuint type);
GLint getUniformLocation(Shader* shader, const GLchar* name);
//============================================||
//			    	---------UNIFORM VARIABLE LOADING-----------				 ||
//============================================||
void loadFloat(Shader* shader, const GLchar* name, GLfloat value);
void loadFloatArray(Shader* shader, const GLchar* name, GLfloat arr[], int count);
void loadInt(Shader* shader, const GLchar* name, GLint value);
void loadIntArray(Shader* shader, const GLchar* name, GLint arr[], int count);
void loadVec2f(Shader* shader, const GLchar* name, vec2f vec);
void loadVec3f(Shader* shader, const GLchar* name, vec3f vec);
void loadVec4f(Shader* shader, const GLchar* name, vec4f vec);
void loadBool(Shader* shader, const GLchar* name, bool value);
void loadMat4f(Shader* shader, const GLchar* name, mat4f mat);
//============================================||
//				---------UNIFORM VARIABLE LOADING-----------					 ||
//============================================||
void startShader(Shader* shader);
void stopShader();

void disposeShader(Shader shader);