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

#include <stdlib.h>
#include <vector>
#include "defines.h"
#include "maths.h"

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

void init_window(int width, int height, const char* title, bool fullscreen, bool resizable, bool primary_monitor);

void set_window_pos(int x, int y);
void set_window_size(int width, int height);
void begin_drawing();
void end_drawing();
bool is_window_closed();

void set_clear_color(float r, float g, float b, float a);
void set_clear_color(vec4 color);
void set_key_callback(void(*keyCallback)(int key, int action));
void set_mouse_callback(void(*mouseCallback)(double mousex, double mousey, int button, int action));
void set_window_resize_callback(void(*resizecallback)(int width, int height));

int get_key_pressed();
int get_button_pressed();
int get_key_released();
int get_button_released();
bool is_key_pressed(unsigned int keycode);
bool is_key_released(unsigned int keycode);
bool is_button_pressed(unsigned int button);
bool is_button_released(unsigned int button);
bool is_key_down(unsigned int keycode);
bool is_button_down(unsigned int button);
bool is_key_up(unsigned int keycode);
bool is_button_up(unsigned int button);

double get_elapsed_time();

void get_mouse_pos(double* mousex, double* mousey);
vec2 get_mouse_pos();
void set_window_should_close(bool shouldClose);
void dispose_window();

void set_FPS_cap(double FPS);

void set_mouse_locked(bool locked);
void set_mouse_hidden(bool hidden);
void set_vsync(bool vsync);

void set_virtual_size(int v_width, int v_height);
int get_virtual_width();
int get_virtual_height();
vec2 get_virtual_size();
int get_window_width();
int get_window_height();

#endif