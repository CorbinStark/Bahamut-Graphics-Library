#pragma once

#include "maths.h"
#include "camera.h"
#include <iomanip>

enum rotationflag {
	ROTATE_X,
	ROTATE_Y,
	ROTATE_Z
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
struct mat4f {

	union {
		float elements[4 * 4];
		vec4f columns[4];
	};

	mat4f();
	mat4f(float diagonal);

	//================================================
	//Description: Creates an identity matrix and 
	//	returns it.
	//================================================
	static mat4f identity();

	//================================================
	//Description: Creates an orthographic matrix and 
	//	returns it.
	//
	//
	//	2D-VIEW Projection Matrix                                              
	//	x = left, y = top, width = right, height = bottom, 
	//		near = z1, far = z2	
	//================================================
	static mat4f orthographic(float x, float y, float width, float height, float near, float far);

	//================================================
	//Description: Creates a perspective matrix and 
	//	returns it.
	//================================================
	static mat4f perspective(float fov, float aspectRatio, float near, float far);

	//================================================
	//Description: Creates a transformation matrix and 
	//	returns it.
	//================================================
	static mat4f createTransformationMatrix(const vec3f& translation, const vec3f& rotation, const vec3f& scale);
	static mat4f createTransformationMatrix(const float x, const float y, const float z, 
		const float rotX, const float rotY, const float rotZ, 
		const float scaleX, const float scaleY, const float scaleZ
	);
	static mat4f createViewMatrix(Camera& cam);

	static mat4f translation(const vec3f& translation);
	static mat4f translation(const float x, const float y, const float z);
	static mat4f rotation(float angle, const vec3f& axis);
	static mat4f rotation(float angle, const float axisX, const float axisY, const float axisZ);
	static mat4f rotationX(float angle);
	static mat4f rotationY(float angle);
	static mat4f rotationZ(float angle);
	static mat4f scaleMatrix(const vec3f& scale);
	static mat4f scaleMatrix(const float scaleX, const float scaleY, const float scaleZ);

	//================================================
	//Description: Multiplies the current mat4f by 
	//	a translation matrix created with the given
	//	vector3f
	//================================================
	void translate(const vec3f& translation);
	void translate(const float x, const float y, const float z);

	//================================================
	//Description: Multiplies the current mat4f by 
	//	a rotation matrix created with the given
	//	vector3f and angle
	//================================================
	void rotate(const float angle, const vec3f& axis);
	void rotate(const float angle, const float axisX, const float axisY, const float axisZ);

	//================================================
	//Description: Multiplies the current mat4f by 
	//	a scale matrix created with the given
	//	vector3f
	//================================================
	void scale(const vec3f& scale);
	void scale(const float scaleX, const float scaleY, const float scaleZ);

	mat4f& multiply(const mat4f& other);

	friend mat4f operator*(mat4f left, const mat4f& right);

	mat4f& operator*=(const mat4f& other);
	friend std::ostream& operator<<(std::ostream& stream, const mat4f& mat);

};