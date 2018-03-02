///////////////////////////////////////////////////////////////////////////
// FILE:			           vec4f.cpp                                 //
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

#include "vec4f.h"

vec4f::vec4f() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

vec4f::vec4f(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float vec4f::length() {
	return sqrt(x * x + y * y + z * z + w * w);
}

void vec4f::normalize() {
	float length = this->length();

	if (length != 0) {
		x = x / length;
		y = y / length;
		z = z / length;
		w = w / length;
	}
}

float vec4f::dot(const vec4f& other) {
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

vec4f& vec4f::add(const vec4f& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	return *this;
}

vec4f& vec4f::subtract(const vec4f& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;

	return *this;
}

vec4f& vec4f::multiply(const vec4f& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;

	return *this;
}

vec4f& vec4f::divide(const vec4f& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;

	return *this;
}

vec4f operator+(vec4f left, const vec4f& right) {
	left.add(right);
	return left;
}

vec4f operator-(vec4f left, const vec4f& right) {
	left.subtract(right);
	return left;
}

vec4f operator*(vec4f left, const vec4f& right) {
	left.multiply(right);
	return left;
}

vec4f operator/(vec4f left, const vec4f& right) {
	left.divide(right);
	return left;
}

//float operations
vec4f& vec4f::operator+=(const float other) {
	x += other;
	y += other;
	z += other;
	w += other;

	return vec4f(x, y, z, w);
}

vec4f& vec4f::operator-=(const float other) {
	x -= other;
	y -= other;
	z -= other;
	w -= other;

	return vec4f(x, y, z, w);
}

vec4f& vec4f::operator*=(const float other) {
	x *= other;
	y *= other;
	z *= other;
	w *= other;

	return vec4f(x, y, z, w);
}

vec4f& vec4f::operator/=(const float other) {
	x /= other;
	y /= other;
	z /= other;
	w /= other;

	return vec4f(x, y, z, w);
}

int vec4f::operator[](const int index) {
	return *(&x + index);
}

//vec3f operations
vec4f& vec4f::operator+=(const vec4f& other) {
	return add(other);
}

vec4f& vec4f::operator-=(const vec4f& other) {
	return subtract(other);
}

vec4f& vec4f::operator*=(const vec4f& other) {
	return multiply(other);
}

vec4f& vec4f::operator/=(const vec4f& other) {
	return divide(other);
}

bool vec4f::operator==(const vec4f& other) {
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool vec4f::operator!=(const vec4f& other) {
	return !(*this == other);
}


std::ostream& operator<<(std::ostream& stream, const vec4f& vector) {
	stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return stream;
}