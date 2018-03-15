///////////////////////////////////////////////////////////////////////////
// FILE:                        window.h                                 //
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

#ifndef WINDOW_H
#define WINDOW_H

#if defined(_WIN32)
#include <windows.h>
#endif
#if defined(__LINUX__)
#include <unistd.h>
#endif
#if defined(__APPLE__)
#include <unistd.h>
#endif

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdlib.h>
#include <vector>
#include "maths.h"
#include "defines.h"

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

void initWindow(int width, int height, const char* title, bool fullscreen, bool resizable, bool primary_monitor);

void setWindowPos(int x, int y);
void setWindowSize(int width, int height);
void beginDrawing();
void endDrawing();
bool isWindowClosed();

void setClearColor(float r, float g, float b, float a);
void setClearColor(vec4f& color);
void setKeyCallback(void(*keyCallback)(int key, int action));
void setMouseCallback(void(*mouseCallback)(double mousex, double mousey, int button, int action));
void setWindowResizeCallback(void(*resizecallback)(int width, int height));

bool isKeyPressed(unsigned int keycode);
bool isKeyReleased(unsigned int keycode);
bool isButtonPressed(unsigned int button);
bool isButtonReleased(unsigned int button);
bool isKeyDown(unsigned int keycode);
bool isButtonDown(unsigned int button);
bool isKeyUp(unsigned int keycode);
bool isButtonUp(unsigned int button);

double getElapsedTime();

void getMousePos(double* mousex, double* mousey);
vec2f getMousePos();
void disposeWindow();

void setFPSCap(double FPS);

void setMouseLocked(bool locked);
void setMouseHidden(bool hidden);
void setVSync(bool vsync);

void setVirtualSize(int v_width, int v_height);
int getVirtualWidth();
int getVirtualHeight();
vec2f getVirtualSize();
int getWindowWidth();
int getWindowHeight();

#endif