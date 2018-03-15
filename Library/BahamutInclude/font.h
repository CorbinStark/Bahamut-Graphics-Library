///////////////////////////////////////////////////////////////////////////
// FILE:                        font.h                                   //
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

#ifndef FONT_H
#define FONT_H

#include "texture.h"
#include "vec2f.h"
#include "defines.h"
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H 

struct Character {
	Texture	texture;
	vec2f	size;
	vec2f	bearing;
	GLuint	advance;
};

struct Font {
	Character* characters[128];
	FT_Face face;
	FT_Library ft;
	int size;
};

Texture createTextureFromString(Font& font, const std::string str, GLubyte r = 0, GLubyte g = 0, GLubyte b = 0);
Font loadFont(const GLchar* filepath, unsigned int size);
void disposeFont(Font& font);

inline const u32 getStringWidth(Font& font, const char* str) {
	u32 len = strlen(str);
	u32 width = 0;
	for (u32 i = 0; i < len; ++i) {
		if (str[i] == '\n') return width;
		width += font.characters[str[i]]->texture.width;
	}
	width += font.characters['c']->texture.width;
	return width;
}

//text must be less than 128 chars long.
inline const char* formatText(const char* text, ...) {
	static char buffer[MAX_FORMAT_TEXT_SIZE];

	va_list args;
	va_start(args, text);
	vsprintf(buffer, text, args);
	va_end(args);

	return buffer;
}

#endif