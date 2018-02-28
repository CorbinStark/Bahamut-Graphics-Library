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