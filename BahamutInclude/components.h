#pragma once
#include "component.h"

struct testComponent : public Component {
	testComponent(int test) {
		this->test = test;
	}
	int test = 2;
};