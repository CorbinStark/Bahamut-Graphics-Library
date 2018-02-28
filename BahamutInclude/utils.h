#pragma once
#include "vec2f.h"

void inline swap(int& a, int& b) {
	int c = b;
	b = a;
	a = c;
}

void quickSort(int arr[], int size, bool(*compareFunc)());

double getDistanceM(vec2f first, vec2f second);
double getDistanceE(vec2f first, vec2f second);

template <class T>
void clamp(T& input, T min, T max) {
	if (input > max) input = max;
	if (input < min) input = min;
}

template <class T>
T roundUp(T numToRound, T multiple) {
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (numToRound < 0)
		return -(abs(numToRound) - remainder);
	else
		return numToRound + multiple - remainder;
}