#include "utils.h"
#include <algorithm>

void quickSort(int arr[], int min, int max) {
	if (min < max) {
		int pivot = arr[min];
		int left = min;
		for (int i = min + 1; i < max; ++i) {
			if (arr[i] < pivot) {
				swap(arr[i], arr[left]);
				left++;
			}
		}
		swap(pivot, arr[left]);
		pivot = left;

		quickSort(arr, min, pivot);
		quickSort(arr, pivot + 1, max);
	}
}

void quickSort(int arr[], int size) {
	quickSort(arr, 0, size);
}

double getDistanceM(vec2f first, vec2f second) {
	double dx = std::abs(first.x - second.x);
	double dy = std::abs(first.y - second.y);
	return std::max(dx, dy);
}

double getDistanceE(vec2f first, vec2f second) {
	double dx = first.x - second.x;
	double dy = first.y - second.y;
	return std::abs(std::sqrt(dx * dx + dy * dy));
}