///////////////////////////////////////////////////////////////////////////
// FILE:                       utils.cpp                                 //
///////////////////////////////////////////////////////////////////////////
//                      BAHAMUT GRAPHICS LIBRARY                         //
//                        Author: Corbin Stark                           //
///////////////////////////////////////////////////////////////////////////
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