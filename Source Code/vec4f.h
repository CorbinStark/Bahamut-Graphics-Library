///////////////////////////////////////////////////////////////////////////
// FILE:                         vec4f.h                                 //
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

#ifndef VEC4F_H
#define VEC4F_H

#include <ostream>
#include "defines.h"

//================================================
//Description: 4-dimensional vector.
//================================================
struct vec4f {
	float x, y, z, w;

	vec4f();
	vec4f(float x, float y, float z, float w);

	void normalize();
	//magnitude
	float length();
	float dot(const vec4f& other);

	vec4f& add(const vec4f& other);
	vec4f& subtract(const vec4f& other);
	vec4f& multiply(const vec4f& other);
	vec4f& divide(const vec4f& other);

	friend vec4f operator+(vec4f left, const vec4f& right);
	friend vec4f operator-(vec4f left, const vec4f& right);
	friend vec4f operator*(vec4f left, const vec4f& right);
	friend vec4f operator/(vec4f left, const vec4f& right);

	//float operations
	vec4f& operator+=(const float other);
	vec4f& operator-=(const float other);
	vec4f& operator*=(const float other);
	vec4f& operator/=(const float other);

	int operator[](const int index);

	//vec3f operations
	vec4f& operator+=(const vec4f& other);
	vec4f& operator-=(const vec4f& other);
	vec4f& operator*=(const vec4f& other);
	vec4f& operator/=(const vec4f& other);
	bool operator==(const vec4f& other);
	bool operator!=(const vec4f& other);

	friend std::ostream& operator<<(std::ostream& stream, const vec4f& vector);
};

#endif