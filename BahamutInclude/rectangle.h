#pragma once
#include "vec2f.h"

struct Rectangle {
	Rectangle();
	Rectangle(float x, float y, float width, float height);
	float x;
	float y;
	float width;
	float height;
};

bool colliding(Rectangle& first, Rectangle& second);
bool colliding(Rectangle& rect, float x, float y, float width, float height);
bool colliding(Rectangle& rect, vec2f& point);