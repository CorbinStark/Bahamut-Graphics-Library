///////////////////////////////////////////////////////////////////////////
// FILE:                      texture.cpp                                //
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

#include "texture.h"
#include <SOIL.h>

#if defined(BMT_USE_NAMESPACE) 
namespace bmt {
#endif

#define MAX_LOADED_TEXTURES 0xFFFF
#define _PREVENT_MULTIPLE_TEXTURES

struct TexData {
	char* identifier;
	Texture texture;
};

INTERNAL
char* createUniqueName() {
	STORAGE u16 counter;
	char* name = (char*)malloc(5);
	sprintf(name, "%d", counter++);
	return name;
}

//INTERNAL VARIABLES
INTERNAL u16 loaded_textures_size = 1;
INTERNAL u16 num_loaded_textures = 0;
INTERNAL TexData* loaded_textures = (TexData*)malloc(loaded_textures_size * sizeof(TexData));

////////////////////////////////////////////////////////////////////////////////////////////////////

Texture create_blank_texture(u32 width, u32 height) {
	Texture texture;
	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	texture.width = width;
	texture.height = height;
	texture.flip_flag = 0;

	return texture;
}

Texture load_texture(unsigned char* pixels, u32 width, u32 height, u16 param) {
	Texture texture;
	texture.width = width;
	texture.height = height;

	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glBindTexture(GL_TEXTURE_2D, 0);
	texture.flip_flag = 0;

	return texture;
}

Texture load_texture(const char* filepath, u16 param) {
#if defined(_PREVENT_MULTIPLE_TEXTURES)
	for (u16 i = 0; i < num_loaded_textures; ++i) {
		if (strcmp(loaded_textures[i].identifier, filepath) == 0) {
			BMT_LOG(INFO, "[%s] This texture has already been loaded into VRAM. \
			\nReturning a copy of the already loaded texture.", filepath);
			return loaded_textures[i].texture;
		}
	}
#endif

	Texture texture;
	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	unsigned char* image = SOIL_load_image(filepath, &texture.width, &texture.height, 0, SOIL_LOAD_RGBA);
	if (image != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

#if defined(_PREVENT_MULTIPLE_TEXTURES)
		loaded_textures[num_loaded_textures].identifier = duplicate_string(filepath);
		loaded_textures[num_loaded_textures].texture.ID = texture.ID;
		loaded_textures[num_loaded_textures].texture.height = texture.height;
		loaded_textures[num_loaded_textures].texture.width = texture.width;
		loaded_textures[num_loaded_textures].texture.flip_flag = 0;
		num_loaded_textures++;

		if (num_loaded_textures == loaded_textures_size) {
			if (loaded_textures_size * 2 >= MAX_LOADED_TEXTURES)
				loaded_textures_size = MAX_LOADED_TEXTURES;
			else
				loaded_textures_size *= 2;
			loaded_textures = (TexData*)realloc(loaded_textures, loaded_textures_size * sizeof(TexData));
		}
#endif

	}
	else {
		BMT_LOG(WARNING, "[%s] Texture could not be loaded! Disposing and returning blank texture.", filepath);
		dispose_texture(texture);
	}
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glBindTexture(GL_TEXTURE_2D, 0);
	texture.flip_flag = 0;

	return texture;
}

void dispose_texture(Texture& texture) {
#if defined(_PREVENT_MULTIPLE_TEXTURES)
	for (u16 i = 0; i < num_loaded_textures; ++i) {
		if (texture.ID == loaded_textures[i].texture.ID) {
			loaded_textures_size--;
			for (u16 j = 0; j < loaded_textures_size; ++j)
				loaded_textures[j] = loaded_textures[j + 1];
			break;
		}
	}
#endif
	glDeleteTextures(1, &texture.ID);
	texture.ID = 0;
}

void set_texture_pixels(Texture texture, unsigned char* pixels, u32 width, u32 height) {
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void set_texture_pixels_from_file(Texture texture, const char* filepath) {
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	unsigned char* image = SOIL_load_image(filepath, &texture.width, &texture.height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void bind_texture(Texture texture, u32 slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
}

void unbind_texture(u32 slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

#if defined(BMT_USE_NAMESPACE) 
}
#endif