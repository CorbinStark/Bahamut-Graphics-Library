#pragma once
#include "texture.h"
#include "vec2f.h"
#include <ft2build.h>
#include FT_FREETYPE_H 

#define MAX_FORMAT_TEXT_SIZE 128

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
inline const char* formatText(const char* text, ...) {
	static char buffer[MAX_FORMAT_TEXT_SIZE];

	va_list args;
	va_start(args, text);
	vsprintf(buffer, text, args);
	va_end(args);

	return buffer;
}