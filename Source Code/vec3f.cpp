///////////////////////////////////////////////////////////////////////////
// FILE:                        vec3f.cpp                                //
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

#include "vec3f.h"

vec3f::vec3f() {
	x = 0;
	y = 0;
	z = 0;
}

vec3f::vec3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float vec3f::length() const {
	return sqrt(x * x + y * y + z * z);
}

void vec3f::normalize() {
	float length = this->length();

	if (length != 0) {
		x = x / length;
		y = y / length;
		z = z / length;
	}
}

float vec3f::dot(const vec3f& other) const {
	return (x * other.x) + (y * other.y) + (z * other.z);
}

vec3f vec3f::cross(const vec3f& other) const {
	return vec3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y *other.x);
}

vec3f& vec3f::add(const vec3f& other) {
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

vec3f& vec3f::subtract(const vec3f& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

vec3f& vec3f::multiply(const vec3f& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

vec3f& vec3f::divide(const vec3f& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;

	return *this;
}

vec3f operator+(vec3f left, const vec3f& right) {
	left.add(right);
	return left;
}

vec3f operator-(vec3f left, const vec3f& right) {
	left.subtract(right);
	return left;
}

vec3f operator*(vec3f left, const vec3f& right) {
	left.multiply(right);
	return left;
}

vec3f operator/(vec3f left, const vec3f& right) {
	left.divide(right);
	return left;
}

//float operations
vec3f& vec3f::operator+=(const float other) {
	x += other;
	y += other;
	z += other;

	return vec3f(x, y, z);
}

vec3f& vec3f::operator-=(const float other) {
	x -= other;
	y -= other;
	z -= other;

	return vec3f(x, y, z);
}

vec3f& vec3f::operator*=(const float other) {
	x *= other;
	y *= other;
	z *= other;

	return vec3f(x, y, z);
}

vec3f& vec3f::operator/=(const float other) {
	x /= other;
	y /= other;
	z /= other;

	return vec3f(x, y, z);
}

vec3f& vec3f::operator/(const float other) {
	return vec3f(
		x / other,
		y / other,
		z / other
	);
}

vec3f& vec3f::operator*(const float other) {
	return vec3f(
		x * other,
		y * other,
		z * other
	);
}

//vec3f operations
vec3f& vec3f::operator+=(const vec3f& other) {
	return add(other);
}

vec3f& vec3f::operator-=(const vec3f& other) {
	return subtract(other);
}

vec3f& vec3f::operator*=(const vec3f& other) {
	return multiply(other);
}

vec3f& vec3f::operator/=(const vec3f& other) {
	return divide(other);
}

bool vec3f::operator==(const vec3f& other) {
	return x == other.x && y == other.y && z == other.z;
}

bool vec3f::operator!=(const vec3f& other) {
	return !(*this == other);
}


std::ostream& operator<<(std::ostream& stream, const vec3f& vector) {
	stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return stream;
}