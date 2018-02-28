#include "rectangle.h"

Rectangle::Rectangle() { x = y = width = height = 0; }
Rectangle::Rectangle(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

bool colliding(Rectangle& first, Rectangle& second) {
	if (first.x + first.width >= second.x && first.x <= second.x + second.width) {
		if (first.y <= second.y + second.height && first.y + first.height >= second.y) {
			return true;
		}
	}
	return false;
}

bool colliding(Rectangle& rect, float x, float y, float width, float height) {
	if (rect.x + rect.width >= x && rect.x <= x + width) {
		if (rect.y <= y + height && rect.y + rect.height >= y) {
			return true;
		}
	}
	return false;
}

bool colliding(Rectangle& rect, vec2f& point) {
	if (rect.x + rect.width >= point.x && rect.x <= point.x + 1) {
		if (rect.y <= point.y + 1 && rect.y + rect.height >= point.y) {
			return true;
		}
	}
	return false;
}
