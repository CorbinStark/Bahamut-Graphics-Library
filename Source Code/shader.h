///////////////////////////////////////////////////////////////////////////
// FILE:                        shader.h                                 //
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

#ifndef SHADER_H
#define SHADER_H

#include "defines.h"
#include "maths.h"

struct Shader {
	GLuint ID;
	GLuint vertexshaderID;
	GLuint fragshaderID;

	GLuint vertexLoc;
	GLuint texcoordLoc;
	GLuint normalLoc;
	GLuint colorLoc;

	GLuint modelMatrixLoc;
	GLuint diffuseColorLoc;
	GLuint specularColorLoc;
	GLuint ambientColorLoc;

	GLuint mapTextureLoc0;
};

Shader loadShader(const GLchar* vertexfile, const GLchar* fragmentfile);
Shader loadShaderFromStrings(const GLchar* vertexstring, const GLchar* fragmentstring);
GLuint loadShaderFile(const GLchar* path, GLuint type);
GLuint loadShaderString(const GLchar* string, GLuint type);
GLint getUniformLocation(Shader* shader, const GLchar* name);
//============================================||
// ---------UNIFORM VARIABLE LOADING--------- ||
//============================================||
void loadFloat(Shader* shader, const GLchar* name, GLfloat value);
void loadFloatArray(Shader* shader, const GLchar* name, GLfloat arr[], int count);
void loadInt(Shader* shader, const GLchar* name, GLint value);
void loadIntArray(Shader* shader, const GLchar* name, GLint arr[], int count);
void loadVec2(Shader* shader, const GLchar* name, vec2 vec);
void loadVec3(Shader* shader, const GLchar* name, vec3 vec);
void loadVec4(Shader* shader, const GLchar* name, vec4 vec);
void loadBool(Shader* shader, const GLchar* name, bool value);
void loadMat4(Shader* shader, const GLchar* name, mat4 mat);
//============================================||
// ---------UNIFORM VARIABLE LOADING--------- ||
//============================================||
void startShader(Shader* shader);
void stopShader();

void disposeShader(Shader shader);

#endif