///////////////////////////////////////////////////////////////////////////
// FILE:                         maths.h                                 //
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

#ifndef MATHS_H
#define MATHS_H

#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "mat4f.h"
#include "defines.h"

INTERNAL const f32 PI = (float)3.141592653589793238;

INTERNAL
f32 degToRad(f32 deg) {
	return deg * PI / 180;
}

union vec2 {
	f32 e[2];
	struct {
		f32 x;
		f32 y;
	};
};

union vec3 {
	f32 e[3];
	struct {
		f32 x;
		f32 y;
		f32 z;
	};
	struct {
		vec2 xy;
		f32 _pad0;
	};
	struct {
		f32 _pad1;
		vec2 yz;
	};
};

union vec4 {
	f32 e[4];
	struct {
		f32 x;
		f32 y;
		f32 z;
		f32 w;
	};
	struct {
		vec3 xyz;
		f32 _pad0;
	};
	struct {
		vec2 xy;
		f32 _pad1;
		f32 _pad2;
	};
	struct {
		f32 _pad3;
		vec2 yz;
		f32 _pad4;
	};
	struct {
		f32 _pad5;
		f32 _pad6;
		vec2 zw;
	};
};

//================================================
//Description: 4x4 matrix for graphics calculations. 
//
//Comments: IMPORTANT! Matrices in OpenGL are
//	column-major. Accessing elements of this
//	matrix are done in the following way:
//------------------------------------------------
//		elements[column + row * 4]
//================================================
union mat4 {
	f32 e[4 * 4];
	vec4 columns[4];
};

INTERNAL
inline mat4 identity() {
	mat4 mat =
	{ 
	    1, 0, 0, 0,
	    0, 1, 0, 0,
	    0, 0, 1, 0,
	    0, 0, 0, 1 
	};
	return mat;
}

INTERNAL
inline mat4 operator*(const mat4& a, const mat4& b) {
	mat4 c = {};
	f32 data[4 * 4];
	for (u8 y = 0; y < 4; ++y) {
		for (u8 x = 0; x < 4; ++x) {
			f32 sum = 0.0f;
			for (u8 i = 0; i < 4; ++i) {
				sum += a.e[x + i * 4] * b.e[i + y * 4];
			}
			data[x + y * 4] = sum;
		}
	}
	memcpy(c.e, data, 16 * sizeof(f32));
	return c;
}

INTERNAL
inline void operator*=(mat4& a, mat4& b) {
	f32 data[4 * 4];
	for (u8 y = 0; y < 4; ++y) {
		for (u8 x = 0; x < 4; ++x) {
			f32 sum = 0.0f;
			for (u8 i = 0; i < 4; ++i) {
				sum += a.e[x + i * 4] * b.e[i + y * 4];
			}
			data[x + y * 4] = sum;
		}
	}
	memcpy(a.e, data, 16 * sizeof(f32));
}

INTERNAL
inline mat4 translation(const vec3& translation) {
	mat4 mat = identity();
	mat.e[0 + 3 * 4] = translation.x;
	mat.e[1 + 3 * 4] = translation.y;
	mat.e[2 + 3 * 4] = translation.z;
	return mat;
}

INTERNAL
inline mat4 translation(const f32 x, const f32 y, const f32 z) {
	mat4 mat = identity();
	mat.e[0 + 3 * 4] = x;
	mat.e[1 + 3 * 4] = y;
	mat.e[2 + 3 * 4] = z;
	return mat;
}

INTERNAL
inline mat4 scale(const vec3& scale) {
	mat4 mat = { 0 };
	mat.e[0 + 0 * 4] = scale.x;
	mat.e[1 + 1 * 4] = scale.y;
	mat.e[2 + 2 * 4] = scale.z;
	mat.e[3 + 3 * 4] = 1.0f;
	return mat;
}

INTERNAL
inline mat4 scale(const f32 x, const f32 y, const f32 z) {
	mat4 mat = { 0 };
	mat.e[0 + 0 * 4] = x;
	mat.e[1 + 1 * 4] = y;
	mat.e[2 + 2 * 4] = z;
	mat.e[3 + 3 * 4] = 1.0f;
	return mat;
}

INTERNAL
inline mat4 rotateX(const vec3& rotationX) {

}

#endif