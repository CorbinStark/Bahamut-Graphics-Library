///////////////////////////////////////////////////////////////////////////
// FILE:                        states.h                                 //
///////////////////////////////////////////////////////////////////////////
//                      BAHAMUT GRAPHICS LIBRARY                         //
//                        Author: Corbin Stark                           //
///////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Corbin Stark                                       //
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

#ifndef MAX_STATES
#define MAX_STATES 15
#endif

INTERNAL State* states[MAX_STATES];
INTERNAL int curr_state;
INTERNAL int num_states;

void State::init() {}
void State::ready() {}
void State::dispose() {}

void add_state(State* state, const char* name) {
	if (num_states >= MAX_STATES)
		printf("ERROR: STATES: TOO MANY STATES!\n");
	else {
		state->name = (char*)malloc(strlen(name) + 1);
		strcpy(state->name, name);
		states[num_states] = state;
		num_states++;
		state->init();
	}
}

void remove_state(const char* name) {
	bool found = false;
	for (int i = 0; i < num_states; ++i) {
		if (strcmp(states[i]->name, name) == 0) {
			free(states[i]->name);
			states[i]->dispose();
			//move everything down 
			//i is the item to remove
			for (int j = i + 1; j < num_states; ++j) {
				states[j - 1] = states[j];
			}
			num_states--;
			found = true;
		}
	}
	if (!found)
		printf("ERROR: STATES: State '%s' doesn't exist!\n", name);
}

void set_state(const char* name) {
	bool found = false;
	for (int i = 0; i < num_states; ++i) {
		if (states[i] != NULL && strcmp(states[i]->name, name) == 0) {
			states[i]->ready();
			curr_state = i;
			found = true;
		}
	}
	if (!found)
		printf("ERROR: STATES: State '%s' doesn't exist!\n", name);
}

void update_curr_state() {
	states[curr_state]->update();
}

void draw_curr_state() {
	states[curr_state]->draw();
}