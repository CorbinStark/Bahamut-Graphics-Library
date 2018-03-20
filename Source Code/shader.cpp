///////////////////////////////////////////////////////////////////////////
// FILE:                        shader.cpp                               //
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

#include "shader.h"
#include "IO.h"
#include <iostream>
#include <vector>

GLint get_uniform_location(Shader* shader, const GLchar* name) {
	//GLint location = 0;
	//try {
	//	location = _locations.get(name);
	//}
	//catch (std::out_of_range& const e) {
	//	std::cout << "Caching uniform location of the name: " << name << std::endl;
	//	_locations.insert(name, glGetUniformLocation(_progID, name)); //so it doesnt display error message again
	//}
	return glGetUniformLocation(shader->ID, name);
}

Shader load_shader(const GLchar* vertexfile, const GLchar* fragmentfile) {
	Shader shader;
	shader.vertexshaderID = load_shader_file(vertexfile, GL_VERTEX_SHADER);
	shader.fragshaderID = load_shader_file(fragmentfile, GL_FRAGMENT_SHADER);

	shader.ID = glCreateProgram();
	glAttachShader(shader.ID, shader.vertexshaderID);
	glAttachShader(shader.ID, shader.fragshaderID);
	glBindFragDataLocation(shader.ID, 0, "outColor");
	glLinkProgram(shader.ID);
	glValidateProgram(shader.ID);
	glUseProgram(shader.ID);
	return shader;
}

Shader load_shader_from_strings(const GLchar* vertexstring, const GLchar* fragmentstring) {
	Shader shader;
	shader.vertexshaderID = load_shader_string(vertexstring, GL_VERTEX_SHADER);
	shader.fragshaderID = load_shader_string(fragmentstring, GL_FRAGMENT_SHADER);

	shader.ID = glCreateProgram();
	glAttachShader(shader.ID, shader.vertexshaderID);
	glAttachShader(shader.ID, shader.fragshaderID);
	glBindFragDataLocation(shader.ID, 0, "outColor");
	glLinkProgram(shader.ID);
	glValidateProgram(shader.ID);
	glUseProgram(shader.ID);
	return shader;
}

void load_float(Shader* shader, const GLchar* name, GLfloat value) {
	GLint location = get_uniform_location(shader, name);
	glUniform1f(location, value);
}

void load_float_array(Shader* shader, const GLchar* name, GLfloat arr[], int count) {
	GLint location = get_uniform_location(shader, name);
	glUniform1fv(location, count, arr);
}

void load_int(Shader* shader, const GLchar* name, GLint value) {
	GLint location = get_uniform_location(shader, name);
	glUniform1i(location, value);
}

void load_int_array(Shader* shader, const GLchar* name, GLint arr[], int count) {
	GLint location = get_uniform_location(shader, name);
	glUniform1iv(location, count, arr);
}

void load_vec2(Shader* shader, const GLchar* name, vec2 vec) {
	GLint location = get_uniform_location(shader, name);
	glUniform2f(location, vec.x, vec.y);
}

void load_vec3(Shader* shader, const GLchar* name, vec3 vec) {
	GLint location = get_uniform_location(shader, name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void load_vec4(Shader* shader, const GLchar* name, vec4 vec) {
	GLint location = get_uniform_location(shader, name);
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void load_bool(Shader* shader, const GLchar* name, bool value) {
	int boolean = 0;
	if (value)
		boolean = 1;

	GLint location = get_uniform_location(shader, name);
	glUniform1f((GLfloat)location, boolean);
}

void load_mat4(Shader* shader, const GLchar* name, mat4 mat) {
	GLint location = get_uniform_location(shader, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, mat.elements);
}

GLuint load_shader_file(const GLchar* filename, GLuint type) {
	int shaderID = glCreateShader(type);

	const GLchar* shaderSource = read_file(filename);

	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);

	GLint result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint len;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> error(len);
		glGetShaderInfoLog(shaderID, len, &len, &error[0]);
		glDeleteShader(shaderID);
		std::cout << "\nCOULD NOT COMPILE  " << filename << " SHADER!" << "(ID: " << shaderID << ")\n" << std::endl;
		std::cout << &error[0] << std::endl;
		return 0;
	}
	else {
		//std::cout << "Compiled " << filename << " shader successfully! " << "(ID: " << shaderID << ")" << std::endl;
		//std::cout << shaderSource << std::endl;
	}

	delete[] shaderSource;
	return shaderID;
}

GLuint load_shader_string(const GLchar* shaderSource, GLuint type) {
	int shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);

	GLint result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint len;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> error(len);
		glGetShaderInfoLog(shaderID, len, &len, &error[0]);
		glDeleteShader(shaderID);
		std::cout << "\nCOULD NOT COMPILE  " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER!" << "(ID: " << shaderID << ")\n" << std::endl;
		std::cout << &error[0] << std::endl;
		return 0;
	}
	else {
		//std::cout << "Compiled " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader successfully! " << "(ID: " << shaderID << ")" << std::endl;
	}

	return shaderID;
}

void start_shader(Shader* shader) {
	glUseProgram(shader->ID);
}

void stop_shader() {
	glUseProgram(0);
}

void dispose_shader(Shader shader) {
	glDeleteShader(shader.fragshaderID);
	glDeleteShader(shader.vertexshaderID);
	glDeleteProgram(shader.ID);
}