#pragma once
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