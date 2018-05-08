///////////////////////////////////////////////////////////////////////////
// FILE:                       texture.h                                 //
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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include "defines.h"

#if defined(BMT_USE_NAMESPACE) 
namespace bmt {
#endif

const int FLIP_HORIZONTAL = 1;
const int FLIP_VERTICAL = 2;

struct Texture {
	GLuint ID;
	u64 flip_flag;
	i32 width;
	i32 height;
};

Texture create_blank_texture(u32 width = 0, u32 height = 0);
Texture load_texture(unsigned char* pixels, u32 width, u32 height, u16 param);
Texture load_texture(const char* filepath, u16 param);
void dispose_texture(Texture& texture);

void set_texture_pixels(Texture texture, unsigned char* pixels, u32 width, u32 height);
void set_texture_pixels_from_file(Texture texture, const char* filepath);

void bind_texture(Texture texture, u32 slot);
void unbind_texture(u32 slot);

//==========================================================================================
//Description: Sets the wrap_x of the texture (horizontal wrapping)
//
//Parameters: 
//		-A texture to set parameter of
//		-The type of wrapping to do
//
//Comments: Unbinds whatever is in slot 0
//==========================================================================================
INTERNAL inline
void set_texture_wrap_x(Texture texture, u32 type) {
	bind_texture(texture, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type);
	unbind_texture(0);
}

//==========================================================================================
//Description: Sets the wrap_y of the texture (vertical wrapping)
//
//Parameters: 
//		-A texture to set parameter of
//		-The type of wrapping to do
//
//Comments: Unbinds whatever is in slot 0
//==========================================================================================
INTERNAL inline
void set_texture_wrap_y(Texture texture, u32 type) {
	bind_texture(texture, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type);
	unbind_texture(0);
}

struct Framebuffer {
	GLuint ID;
	Texture texture;
};

#define DEPTHBUFFER 0
#define COLORBUFFER 1

INTERNAL inline
Framebuffer create_framebuffer(u32 width, u32 height, u16 param, u8 buffertype) {
	assert(buffertype < 2);

	Framebuffer buffer;
	buffer.texture.width = width;
	buffer.texture.height = height;
	buffer.texture.flip_flag = 0;

	glGenTextures(1, &buffer.texture.ID);
	glBindTexture(GL_TEXTURE_2D, buffer.texture.ID);
	if (buffertype == COLORBUFFER) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}
	else if(buffertype == DEPTHBUFFER) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &buffer.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.ID);

	if(buffertype == COLORBUFFER)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texture.ID, 0);
	else if(buffertype == DEPTHBUFFER)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, buffer.texture.ID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		BMT_LOG(INFO, "Framebuffer #%d successfully created", buffer.ID);
	}
	else {
		BMT_LOG(WARNING, "Framebuffer #%d not complete!", buffer.ID);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return buffer;
}

INTERNAL inline
Framebuffer create_colorbuffer(u32 width, u32 height, u16 param) {
	create_framebuffer(width, height, param, COLORBUFFER);
}

INTERNAL inline
Framebuffer create_depthbuffer(u32 width, u32 height, u16 param) {
	create_framebuffer(width, height, param, DEPTHBUFFER);
}

INTERNAL inline
void dispose_framebuffer(Framebuffer buffer) {
	dispose_texture(buffer.texture);
	glDeleteFramebuffers(1, &buffer.ID);
}

INTERNAL inline
void bind_framebuffer(Framebuffer buffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.ID);
}

INTERNAL inline
void unbind_framebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//==========================================================================================
//Description: Clears the bound framebuffer (by default the window)
//
//Comments: The window is cleared by end_drawing() so no need to do it yourself.
//			If you want to clear the screen mid-frame for some reason, this will do that.
//==========================================================================================
INTERNAL inline
void clear_bound_framebuffer() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#if defined(BMT_USE_NAMESPACE) 
}
#endif

#endif