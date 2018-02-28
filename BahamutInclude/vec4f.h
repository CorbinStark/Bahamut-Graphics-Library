#pragma once
#include <ostream>

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