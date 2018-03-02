///////////////////////////////////////////////////////////////////////////
// FILE:			          texture.h                                  //
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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <vector>
#include "rectangle.h"

const int FLIP_HORIZONTAL = 1;
const int FLIP_VERTICAL = 2;

struct Texture {
	GLuint ID = 0;
	unsigned long flip_flag;
	int width;
	int height;
};

Texture createBlankTexture(int width = 0, int height = 0);
Texture loadTexture(unsigned char* pixels, int width, int height, int param);
Texture loadTexture(std::string filepath, int param);
void disposeTexture(Texture& texture);

void blitTexture(Texture& src, Texture& dest, Rectangle drawFrom, Rectangle drawTo);

void setTexturePixels(Texture& texture, unsigned char* pixels, int width, int height);
void setTexturePixelsFromFile(Texture& texture, std::string filepath);

void bindTexture(Texture& texture, unsigned int slot);
void unbindTexture(unsigned int slot);

#endif