#include "bahamut.h"
#include <iostream>

void setDepthTesting(bool testing) {
	if (testing)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}