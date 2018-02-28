#pragma once
#include "maths.h"
#include "texture.h"
#include <GL/glew.h>

//================================================
//Description: All the data that goes into a shader for every vertex (2D)
//================================================
struct VertexData {
	vec2f pos;
	vec4f color; //32 bit color (8 for R, 8 for G, 8 for B, 8 for A)
	vec2f uv;
	float texid;
};