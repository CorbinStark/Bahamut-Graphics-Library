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

#if defined(BMT_USE_NAMESPACE) 
namespace bmt {
#endif

struct Shader {
	GLuint ID;
	GLuint vertexshaderID;
	GLuint fragshaderID;
};

//==========================================================================================
//Description: Loads and compiles a shader from a vertex file and fragment file
//
//Parameters: 
//		-A path to a vertex shader file
//		-A path to a fragment shader file
//==========================================================================================
Shader load_shader_2D(const GLchar* vertexfile, const GLchar* fragmentfile);
Shader load_shader_3D(const GLchar* vertexfile, const GLchar* fragmentfile);
Shader load_shader_2D_from_strings(const GLchar* vertexstring, const GLchar* fragmentstring);
Shader load_shader_3D_from_strings(const GLchar* vertexstring, const GLchar* fragmentstring);
GLuint load_shader_file(const GLchar* path, GLuint type);
GLuint load_shader_string(const GLchar* string, GLuint type);
GLint get_uniform_location(Shader shader, const GLchar* name);
//=============================================
//
//      UNIFORM VARIABLE UPLOADING
//
//=============================================
void upload_float(Shader shader, const GLchar* name, GLfloat value);
void upload_float_array(Shader shader, const GLchar* name, GLfloat arr[], int count);
void upload_int(Shader shader, const GLchar* name, GLint value);
void upload_int_array(Shader shader, const GLchar* name, GLint arr[], int count);
void upload_vec2(Shader shader, const GLchar* name, vec2 vec);
void upload_vec3(Shader shader, const GLchar* name, vec3 vec);
void upload_vec4(Shader shader, const GLchar* name, vec4 vec);
void upload_bool(Shader shader, const GLchar* name, bool value);
void upload_mat4(Shader shader, const GLchar* name, mat4 mat);
//=============================================
//
//      UNIFORM VARIABLE UPLOADING
//
//=============================================
void start_shader(Shader shader);
void stop_shader();

void dispose_shader(Shader shader);

#if defined(BMT_USE_NAMESPACE) 
}
#endif

#endif