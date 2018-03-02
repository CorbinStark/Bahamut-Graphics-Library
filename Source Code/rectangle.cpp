///////////////////////////////////////////////////////////////////////////
// FILE:			         rectangle.cpp                               //
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
