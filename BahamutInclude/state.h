#pragma once
#include <string>

struct State {
	void(*init)() = NULL;
	void(*draw)() = NULL;
	void(*update)() = NULL;
	void(*dispose)() = NULL;
	std::string name;
};

void setState(std::string name);
void addState(State state);
void removeState(std::string name);

void drawCurrentState();
void updateCurrentState();