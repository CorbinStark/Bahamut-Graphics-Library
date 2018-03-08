///////////////////////////////////////////////////////////////////////////
// FILE:                        vec2f.h                                  //
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

#ifndef VEC2F_H
#define VEC2F_H

#include <ostream>
#include "defines.h"

struct vec2f {
#ifdef USE_SSE
	__m128 SimdReals;
#else
	float x, y;
#endif

	vec2f();
#ifdef USE_SSE
	vec2f(__m128 vec);
#else
	vec2f(float x, float y);
#endif

	void normalize();
	//magnitude
	float length() const;
	float dot(const vec2f& other);

	vec2f& add(const vec2f& other);
	vec2f& subtract(const vec2f& other);
	vec2f& multiply(const vec2f& other);
	vec2f& divide(const vec2f& other);

	friend vec2f operator+(vec2f left, const vec2f& right);
	friend vec2f operator-(vec2f left, const vec2f& right);
	friend vec2f operator*(vec2f left, const vec2f& right);
	friend vec2f operator/(vec2f left, const vec2f& right);

	//float operations
	vec2f& operator+=(const float other);
	vec2f& operator-=(const float other);
	vec2f& operator*=(const float other);
	vec2f& operator/=(const float other);

	//vec2f operations
	vec2f& operator+=(const vec2f& other);
	vec2f& operator-=(const vec2f& other);
	vec2f& operator*=(const vec2f& other);
	vec2f& operator/=(const vec2f& other);
	bool operator==(const vec2f& other);
	bool operator!=(const vec2f& other);
	bool operator<(const vec2f& other);
	bool operator>(const vec2f& other);

	friend std::ostream& operator<<(std::ostream& stream, const vec2f& vector);
};

#endif