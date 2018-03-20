///////////////////////////////////////////////////////////////////////////
// FILE:                     orthoshader.h                               //
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

#ifndef ORTHOSHADER_H
#define ORTHOSHADER_H

#include "defines.h"

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

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);

out vec4 pass_color;
out vec2 pass_uv;
out float pass_texid;

void main() {
	pass_color = color;
	pass_uv = uv;
	pass_texid = texid;
	
	gl_Position = pr_matrix * vw_matrix * vec4(position, 1.0, 1.0);
}

)FOO";

#endif