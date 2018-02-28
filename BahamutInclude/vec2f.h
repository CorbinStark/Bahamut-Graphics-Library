#pragma once
#include <ostream>

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