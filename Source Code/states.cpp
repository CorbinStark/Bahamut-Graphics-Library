///////////////////////////////////////////////////////////////////////////
// FILE:                        states.h                                 //
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

#include "state.h"

#define MAX_STATES 10

State states[MAX_STATES];
int numStates = 0;
int currState = 0;

void setState(std::string name) {
	bool found = false;
	for (int i = 0; i < numStates; ++i) {
		if (states[i].name == name) {
			currState = i;
			found = true;
		}
	}
	if (!found)
		printf("ERROR: STATES: State '%s' doesn't exist!\n", name.c_str());
}

void addState(State state) {
	if (numStates >= MAX_STATES)
		printf("ERROR: STATES: TOO MANY STATES!\n");
	else {
		states[numStates] = state;
		numStates++;
		if(state.init != NULL)
			state.init();
		else
			printf("ERROR: STATES: INIT FUNCTION FOR STATE '%s' IS NULL!\n", state.name.c_str());
	}
}

void removeState(std::string name) {
	bool found = false;
	for (int i = 0; i < numStates; ++i) {
		if (states[i].name == name) {
			if(states[i].dispose != NULL)
				states[i].dispose();
			else
				printf("ERROR: STATES: DISPOSE FUNCTION FOR STATE '%s' IS NULL!\n", states[i].name.c_str());
			//move everything down 
			//i is the item to remove
			for (int j = i + 1; j < numStates; ++j) {
				states[j - 1].init = states[j].init;
				states[j - 1].dispose = states[j].dispose;
				states[j - 1].draw = states[j].draw;
				states[j - 1].update = states[j].update;
				states[j - 1].name = states[j].name;
			}
			numStates--;
			found = true;
		}
	}
	if (!found)
		printf("ERROR: STATES: State '%s' doesn't exist!\n", name.c_str());
}


void drawCurrentState() {
	if(states[currState].draw != NULL)
		states[currState].draw();
	else
		printf("ERROR: STATES: DRAW FUNCTION FOR STATE '%s' IS NULL!\n", states[currState].name.c_str());
}

void updateCurrentState() {
	if (states[currState].update != NULL)
		states[currState].update();
	else
		printf("ERROR: STATES: UPDATE FUNCTION FOR STATE '%s' IS NULL!\n", states[currState].name.c_str());
}