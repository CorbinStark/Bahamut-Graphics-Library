#pragma once
#include <ostream>

struct vec3f {
	float x, y, z;

	vec3f();
	vec3f(float x, float y, float z);

	void normalize();
	//magnitude
	float length() const;
	float dot(const vec3f& other) const;
	vec3f cross(const vec3f& other) const;

	vec3f& add(const vec3f& other);
	vec3f& subtract(const vec3f& other);
	vec3f& multiply(const vec3f& other);
	vec3f& divide(const vec3f& other);

	friend vec3f operator+(vec3f left, const vec3f& right);
	friend vec3f operator-(vec3f left, const vec3f& right);
	friend vec3f operator*(vec3f left, const vec3f& right);
	friend vec3f operator/(vec3f left, const vec3f& right);

	//float operations
	vec3f& operator+=(const float other);
	vec3f& operator-=(const float other);
	vec3f& operator*=(const float other);
	vec3f& operator/=(const float other);

	vec3f& operator/(const float other);
	vec3f& operator*(const float other);

	//vec3f operations
	vec3f& operator+=(const vec3f& other);
	vec3f& operator-=(const vec3f& other);
	vec3f& operator*=(const vec3f& other);
	vec3f& operator/=(const vec3f& other);
	bool operator==(const vec3f& other);
	bool operator!=(const vec3f& other);

	friend std::ostream& operator<<(std::ostream& stream, const vec3f& vector);
};