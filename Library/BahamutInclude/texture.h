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

#include <GL/glew.h>
#include <vector>
#include "rectangle.h"
#include "defines.h"

const int FLIP_HORIZONTAL = 1;
const int FLIP_VERTICAL = 2;

struct Texture {
	GLuint ID;
	unsigned long flip_flag;
	int width;
	int height;
};

Texture createBlankTexture(u32 width = 0, u32 height = 0);
Texture loadTexture(unsigned char* pixels, u32 width, u32 height, u16 param);
Texture loadTexture(const char* filepath, u16 param);
void disposeTexture(Texture& texture);

void blitTexture(Texture& src, Texture& dest, Rect drawFrom, Rect drawTo);

void setTexturePixels(Texture& texture, unsigned char* pixels, u32 width, u32 height);
void setTexturePixelsFromFile(Texture& texture, const char* filepath);

void bindTexture(Texture& texture, u32 slot);
void unbindTexture(u32 slot);

#endif