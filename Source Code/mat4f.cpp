///////////////////////////////////////////////////////////////////////////
// FILE:                       mat4f.cpp                                 //
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

#include "mat4f.h"

mat4f::mat4f() {
	memset(elements, 0, 4 * 4 * sizeof(float));
}

mat4f::mat4f(float diagonal) {
	memset(elements, 0, 4 * 4 * sizeof(float));
	elements[0 + 0 * 4] = diagonal;
	elements[1 + 1 * 4] = diagonal;
	elements[2 + 2 * 4] = diagonal;
	elements[3 + 3 * 4] = diagonal;
}

void mat4f::translate(const vec3f& translation) {
	multiply(mat4f::translation(translation));
}
void mat4f::translate(const float x, const float y, const float z) {
	multiply(mat4f::translation(x, y, z));
}

void mat4f::rotate(const float angle, const vec3f& axis) {
	multiply(mat4f::rotation(angle, axis));
}
void mat4f::rotate(const float angle, const float axisX, const float axisY, const float axisZ) {
	multiply(mat4f::rotation(angle, axisX, axisY, axisZ));
}

void mat4f::scale(const vec3f& scale) {
	multiply(mat4f::scaleMatrix(scale));
}
void mat4f::scale(const float scaleX, const float scaleY, const float scaleZ) {
	multiply(mat4f::scaleMatrix(scaleX, scaleY, scaleZ));
}

mat4f& mat4f::multiply(const mat4f& other) {
	float data[16];
	for (auto y = 0; y < 4; ++y) {
		for (auto x = 0; x < 4; ++x) {
			float sum = 0.0f;
			for (auto i = 0; i < 4; ++i) {
				sum += elements[x + i * 4] * other.elements[i + y * 4];
			}
			data[x + y * 4] = sum;
		}
	}
	memcpy(elements, data, 16 * sizeof(float));
	return *this;
}

mat4f operator*(mat4f left, const mat4f& right) {
	return left.multiply(right);
}

mat4f& mat4f::operator*=(const mat4f& other) {
	return multiply(other);
}

mat4f mat4f::translation(const vec3f& translation) {
	mat4f mat(1.0f);
	mat.elements[0 + 3 * 4] = translation.x;
	mat.elements[1 + 3 * 4] = translation.y;
	mat.elements[2 + 3 * 4] = translation.z;
	return mat;
}

mat4f mat4f::translation(const float x, const float y, const float z) {
	mat4f mat(1.0f);
	mat.elements[0 + 3 * 4] = x;
	mat.elements[1 + 3 * 4] = y;
	mat.elements[2 + 3 * 4] = z;
	return mat;
}

mat4f mat4f::scaleMatrix(const vec3f& scale) {
	mat4f mat;
	mat.elements[0 + 0 * 4] = scale.x;
	mat.elements[1 + 1 * 4] = scale.y;
	mat.elements[2 + 2 * 4] = scale.z;
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}

mat4f mat4f::scaleMatrix(const float scaleX, const float scaleY, const float scaleZ) {
	mat4f mat;
	mat.elements[0 + 0 * 4] = scaleX;
	mat.elements[1 + 1 * 4] = scaleY;
	mat.elements[2 + 2 * 4] = scaleZ;
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}

mat4f mat4f::rotationX(float angle) {
	mat4f mat(1.0f);
	float theta = degToRad(angle);
	float s = sin(theta);
	float c = cos(theta);

	mat.elements[1 + 1 * 4] = c;
	mat.elements[2 + 2 * 4] = c;
	mat.elements[2 + 1 * 4] = s;
	mat.elements[1 + 2 * 4] = -s;
	return mat;
}

mat4f mat4f::rotationY(float angle) {
	mat4f mat(1.0f);
	float theta = degToRad(angle);
	float s = sin(theta);
	float c = cos(theta);

	mat.elements[0 + 0 * 4] = c;
	mat.elements[2 + 0 * 4] = -s;
	mat.elements[0 + 2 * 4] = s;
	mat.elements[2 + 2 * 4] = c;
	return mat;
}

mat4f mat4f::rotationZ(float angle) {
	mat4f mat(1.0f);
	float theta = degToRad(angle);
	float s = sin(theta);
	float c = cos(theta);

	mat.elements[0 + 0 * 4] = c;
	mat.elements[1 + 0 * 4] = s;
	mat.elements[0 + 1 * 4] = -s;
	mat.elements[1 + 1 * 4] = c;
	return mat;
}


mat4f mat4f::rotation(float angle, const vec3f& axis) {
	mat4f mat;
	float theta = degToRad(angle);
	float s = sin(theta);
	float c = cos(theta);
	float cm1 = (1.0f - c);

	mat.elements[0 + 0 * 4] = axis.x * axis.x * cm1 + c;
	mat.elements[1 + 0 * 4] = axis.y * axis.x * cm1 + axis.z * s;
	mat.elements[2 + 0 * 4] = axis.x * axis.z * cm1 - axis.y * s;
	mat.elements[0 + 1 * 4] = axis.x * axis.y * cm1 - axis.z * s;
	mat.elements[1 + 1 * 4] = axis.y * axis.y * cm1 + c;
	mat.elements[2 + 1 * 4] = axis.y * axis.z * cm1 + axis.x * s;
	mat.elements[0 + 2 * 4] = axis.x * axis.z * cm1 + axis.y * s;
	mat.elements[1 + 2 * 4] = axis.y * axis.z * cm1 - axis.x * s;
	mat.elements[2 + 2 * 4] = axis.z * axis.z * cm1 + c;
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}

mat4f mat4f::rotation(float angle, const float axisX, const float axisY, const float axisZ) {
	mat4f mat;
	float theta = degToRad(angle);
	float s = sin(theta);
	float c = cos(theta);
	float cm1 = (1.0f - c);

	mat.elements[0 + 0 * 4] = axisX * axisX * cm1 + c;
	mat.elements[1 + 0 * 4] = axisY * axisX * cm1 + axisZ * s;
	mat.elements[2 + 0 * 4] = axisX * axisZ * cm1 - axisY * s;
	mat.elements[0 + 1 * 4] = axisX * axisY * cm1 - axisZ * s;
	mat.elements[1 + 1 * 4] = axisY * axisY * cm1 + c;
	mat.elements[2 + 1 * 4] = axisY * axisZ * cm1 + axisX * s;
	mat.elements[0 + 2 * 4] = axisX * axisZ * cm1 + axisY * s;
	mat.elements[1 + 2 * 4] = axisY * axisZ * cm1 - axisX * s;
	mat.elements[2 + 2 * 4] = axisZ * axisZ * cm1 + c;
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}

mat4f mat4f::identity() {
	return mat4f(1.0f);
}

//=========================================================================
//	2D-VIEW Projection Matrix                                              
//	x = left, y = top, width = right, height = bottom, near = z1, far = z2	   
//=========================================================================
mat4f mat4f::orthographic(float x, float y, float width, float height, float near, float far) {
	mat4f mat;
	mat.elements[0 + 0 * 4] = 2.0f / (width - x);
	mat.elements[1 + 1 * 4] = 2.0f / (y - height);
	mat.elements[2 + 2 * 4] = -2.0f / (far - near);
	mat.elements[0 + 3 * 4] = -((width + x) / (width - x));
	mat.elements[1 + 3 * 4] = -((y + height) / (y - height));
	mat.elements[2 + 3 * 4] = -((far + near) / (far - near));
	mat.elements[3 + 3 * 4] = 1.0f;
	return mat;
}

//=========================================================================
// 3D-VIEW Projection Matrix                                               
//=========================================================================
mat4f mat4f::perspective(float fov, float aspectRatio, float near, float far) {
	mat4f mat;
	float radFov = degToRad(fov);
	mat.elements[0 + 0 * 4] = (1 / tan(radFov * 0.5f)) / aspectRatio;
	mat.elements[1 + 1 * 4] = 1 / tan(radFov * 0.5f);
	mat.elements[2 + 2 * 4] = -((far + near) / (far - near));
	mat.elements[2 + 3 * 4] = -((2.0f * near * far) / (far - near));
	mat.elements[3 + 2 * 4] = -1.0f;
	return mat;
}

mat4f mat4f::createTransformationMatrix(const vec3f& translation, const vec3f& rotation, const vec3f& scale) {
	mat4f mat(1.0f);
	mat.translate(translation);
	mat.rotate(rotation.x, 1, 0, 0);
	mat.rotate(rotation.y, 0, 1, 0);
	mat.rotate(rotation.z, 0, 0, 1);
	mat.scale(scale);
	return mat;
}

mat4f mat4f::createTransformationMatrix(const float x, const float y, const float z,
	const float rotX, const float rotY, const float rotZ,
	const float scaleX, const float scaleY, const float scaleZ) 
{
	mat4f mat(1.0f);
	mat.translate(x, y, z);
	mat.rotate(rotX, 1, 0, 0);
	mat.rotate(rotY, 0, 1, 0);
	mat.rotate(rotZ, 0, 0, 1);
	mat.scale(scaleX, scaleY, scaleZ);
	return mat;
}

mat4f mat4f::createViewMatrix(Camera& cam) {
	mat4f mat(1.0f);
	mat.rotate((float)degToRad(cam.pitch), vec3f(1, 0, 0));
	mat.rotate((float)degToRad(cam.yaw), vec3f(0, 1, 0));
	mat.rotate((float)degToRad(cam.roll), vec3f(0, 0, 1));
	vec3f negativePos(-cam.x, -cam.y, -cam.z);
	mat.translate(negativePos);
	return mat;
}

std::ostream& operator<<(std::ostream& stream, const mat4f& mat) {
	stream << std::endl;
	stream << "[";
	for (int y = 0; y < 4; ++y) {
		if (y != 0)
			stream << " ";
		for (int x = 0; x < 4; ++x) {
			if (x == 3 && y == 3) {
				stream << mat.elements[y + x * 4] << "]";
			}
			else {
				stream << mat.elements[y + x * 4] << ", ";
			}
		}
		stream << std::endl;
	}
	return stream;
}