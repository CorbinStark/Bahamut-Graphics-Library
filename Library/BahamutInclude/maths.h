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

#include "defines.h"

INTERNAL const f32 PI = (float)3.141592653589793238;

INTERNAL
inline f32 deg_to_rad(f32 deg) {
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

INTERNAL
inline vec2 V2(f32 x, f32 y) {
	vec2 vec;
	vec.x = x;
	vec.y = y;
	return vec;
}

INTERNAL
inline vec3 V3(f32 x, f32 y, f32 z) {
	vec3 vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	return vec;
}

INTERNAL
inline vec4 V4(f32 x, f32 y, f32 z, f32 w) {
	vec4 vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	vec.w = w;
	return vec;
}

INTERNAL
inline f32 length(vec2 vec) {
#ifdef USE_SEE
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(SimdReals, SimdReals, 0x71)));
#else
	return sqrt(vec.x * vec.x + vec.y * vec.y);
#endif
}

INTERNAL
inline void normalize(vec2& vec) {
	f32 len = length(vec);
	if (len != 0) {
		vec.x /= len;
		vec.y /= len;
	}
}

INTERNAL 
inline f32 dot(vec2 a, vec2 b) {
	return (a.x * b.x) + (a.y * b.y);
}

INTERNAL
inline f32 length(vec3 vec) {
#ifdef USE_SEE
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(SimdReals, SimdReals, 0x71)));
#else
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
#endif
}

INTERNAL
inline void normalize(vec3& vec) {
	f32 len = length(vec);
	if (len != 0) {
		vec.x /= len;
		vec.y /= len;
		vec.z /= len;
	}
}

INTERNAL
inline f32 dot(vec3 a, vec3 b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

INTERNAL
inline vec3 cross(vec3 a, vec3 b) {
	return V3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

INTERNAL
inline f32 length(vec4 vec) {
#ifdef USE_SEE
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(SimdReals, SimdReals, 0x71)));
#else
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
#endif
}

INTERNAL
inline void normalize(vec4& vec) {
	f32 len = length(vec);
	if (len != 0) {
		vec.x /= len;
		vec.y /= len;
		vec.z /= len;
		vec.w /= len;
	}
}

INTERNAL
inline f32 dot(vec4 a, vec4 b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

//OPERATOR OVERLOADS
//TODO: add overloads for vec3 and vec4 too

INTERNAL
inline vec2 operator+(vec2 a, vec2 b) {
	vec2 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

INTERNAL 
inline vec2 operator-(vec2 a, vec2 b) {
	vec2 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}

INTERNAL
inline vec2 operator*(vec2 a, vec2 b) {
	vec2 c;
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	return c;
}

INTERNAL
inline vec2 operator/(vec2 a, vec2 b) {
	vec2 c;
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	return c;
}

INTERNAL
inline bool operator==(vec2 a, vec2 b) {
	return (a.x == b.x && a.y == b.y);
}

INTERNAL
inline bool operator==(vec3 a, vec3 b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

INTERNAL
inline bool operator==(vec4 a, vec4 b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

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
	f32 elements[4 * 4];
	vec4 columns[4];
	struct {
		f32 m00, m01, m02, m03;
		f32 m10, m11, m12, m13;
		f32 m20, m21, m22, m23;
		f32 m30, m31, m32, m33;
	};
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
inline mat4 operator*(const mat4 a, const mat4 b) {
	mat4 c = {};
	f32 data[4 * 4];
	for (u8 y = 0; y < 4; ++y) {
		for (u8 x = 0; x < 4; ++x) {
			f32 sum = 0.0f;
			for (u8 i = 0; i < 4; ++i) {
				sum += a.elements[x + i * 4] * b.elements[i + y * 4];
			}
			data[x + y * 4] = sum;
		}
	}
	memcpy(c.elements, data, 16 * sizeof(f32));
	return c;
}

INTERNAL
inline void operator*=(mat4 a, mat4 b) {
	f32 data[4 * 4];
	for (u8 y = 0; y < 4; ++y) {
		for (u8 x = 0; x < 4; ++x) {
			f32 sum = 0.0f;
			for (u8 i = 0; i < 4; ++i) {
				sum += a.elements[x + i * 4] * b.elements[i + y * 4];
			}
			data[x + y * 4] = sum;
		}
	}
	memcpy(a.elements, data, 16 * sizeof(f32));
}

//TODO: replace everything like "mat.elements[0 + 3 * 4]" with "mat.m03"

INTERNAL
inline mat4 translation(const f32 x, const f32 y, const f32 z) {
	mat4 mat = identity();
	mat.elements[0 + 3 * 4] = x;
	mat.elements[1 + 3 * 4] = y;
	mat.elements[2 + 3 * 4] = z;
	return mat;
}

INTERNAL
inline mat4 translation(const vec3 translation_vec) {
	return translation(translation_vec.x, translation_vec.y, translation_vec.z);
}

INTERNAL
inline mat4 scale(const f32 x, const f32 y, const f32 z) {
	mat4 mat = { 0 };
	mat.elements[0 + 0 * 4] = x;
	mat.elements[1 + 1 * 4] = y;
	mat.elements[2 + 2 * 4] = z;
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}


INTERNAL
inline mat4 scale(const vec3& scale_vec) {
	return scale(scale_vec.x, scale_vec.y, scale_vec.z);
}

INTERNAL
inline mat4 rotateX(f32 angle) {
	mat4 mat = identity();
	f32 theta = deg_to_rad(angle);
	f32 s = sin(theta);
	f32 c = cos(theta);

	mat.elements[1 + 1 * 4] = c;
	mat.elements[2 + 2 * 4] = c;
	mat.elements[2 + 1 * 4] = s;
	mat.elements[1 + 2 * 4] = -s;
	return mat;
}

INTERNAL
inline mat4 rotateY(f32 angle) {
	mat4 mat = identity();
	f32 theta = deg_to_rad(angle);
	f32 s = sin(theta);
	f32 c = cos(theta);

	mat.elements[0 + 0 * 4] = c;
	mat.elements[2 + 0 * 4] = -s;
	mat.elements[0 + 2 * 4] = s;
	mat.elements[2 + 2 * 4] = c;
	return mat;
}

INTERNAL
inline mat4 rotateZ(f32 angle) {
	mat4 mat = identity();
	f32 theta = deg_to_rad(angle);
	f32 s = sin(theta);
	f32 c = cos(theta);

	mat.elements[0 + 0 * 4] = c;
	mat.elements[1 + 0 * 4] = s;
	mat.elements[0 + 1 * 4] = -s;
	mat.elements[1 + 1 * 4] = c;
	return mat;
}

INTERNAL
inline mat4 rotation(f32 angle, f32 x, f32 y, f32 z) {
	mat4 mat = { 0 };
	f32 theta = deg_to_rad(angle);
	f32 s = sin(theta);
	f32 c = cos(theta);
	f32 cm1 = (1.0f - c);

	mat.elements[0 + 0 * 4] = x * x * cm1 + c;
	mat.elements[1 + 0 * 4] = y * x * cm1 + z * s;
	mat.elements[2 + 0 * 4] = x * z * cm1 - y * s;
	mat.elements[0 + 1 * 4] = x * y * cm1 - z * s;
	mat.elements[1 + 1 * 4] = y * y * cm1 + c;
	mat.elements[2 + 1 * 4] = y * z * cm1 + x * s;
	mat.elements[0 + 2 * 4] = x * z * cm1 + y * s;
	mat.elements[1 + 2 * 4] = y * z * cm1 - x * s;
	mat.elements[2 + 2 * 4] = z * z * cm1 + c;
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}

INTERNAL
inline mat4 rotation(f32 angle, const vec3 axis) {
	return rotation(angle, axis.x, axis.y, axis.z);
}

//2D Projection
INTERNAL
inline mat4 orthographic_projection(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane) {
	mat4 mat = { 0 };
	mat.elements[0 + 0 * 4] = 2.0f / (width - x);
	mat.elements[1 + 1 * 4] = 2.0f / (y - height);
	mat.elements[2 + 2 * 4] = -2.0f / (far_plane - near_plane);
	mat.elements[0 + 3 * 4] = -((width + x) / (width - x));
	mat.elements[1 + 3 * 4] = -((y + height) / (y - height));
	mat.elements[2 + 3 * 4] = -((far_plane + near_plane) / (far_plane - near_plane));
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}

//3D Projection
INTERNAL
inline mat4 perspective_projection(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
	mat4 mat = { 0 };
	float radFov = deg_to_rad(fov);
	mat.elements[0 + 0 * 4] = (1 / tan(radFov * 0.5f)) / aspect_ratio;
	mat.elements[1 + 1 * 4] = 1 / tan(radFov * 0.5f);
	mat.elements[2 + 2 * 4] = -((far_plane + near_plane) / (far_plane - near_plane));
	mat.elements[2 + 3 * 4] = -((2.0f * near_plane * far_plane) / (far_plane - near_plane));
	mat.elements[3 + 2 * 4] = -1.0f;
	return mat;
}

INTERNAL
inline mat4 create_transformation_matrix(f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ) {
	mat4 mat = identity();
	mat *= translation(x, y, z);
	mat *= rotateX(rotX);
	mat *= rotateY(rotY);
	mat *= rotateZ(rotZ);
	mat *= scale(scaleX, scaleY, scaleZ);
	return mat;
}

INTERNAL
inline mat4 create_transformation_matrix(const vec3 translation, const vec3 rotation, const vec3 scale_vec) {
	return create_transformation_matrix(
		translation.x, translation.y, translation.z,
		rotation.x, rotation.y, rotation.z,
		scale_vec.x, scale_vec.y, scale_vec.z
	);
}

#endif