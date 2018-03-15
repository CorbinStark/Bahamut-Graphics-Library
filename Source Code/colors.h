///////////////////////////////////////////////////////////////////////////
// FILE:                       colors.h                                  //
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

#ifndef COLORS_H
#define COLORS_H

#include "defines.h"
#include "vec4f.h"

INTERNAL vec4f LIGHTGRAY = vec4f(200, 200, 200, 255);
INTERNAL vec4f GRAY = vec4f(130, 130, 130, 255);
INTERNAL vec4f DARKGRAY = vec4f(80, 80, 80, 255);
INTERNAL vec4f YELLOW = vec4f(253, 249, 0, 255);
INTERNAL vec4f GOLD = vec4f(255, 203, 0, 255);
INTERNAL vec4f ORANGE = vec4f(255, 161, 0, 255);
INTERNAL vec4f PINK = vec4f(255, 109, 194, 255);
INTERNAL vec4f RED = vec4f(230, 41, 55, 255);
INTERNAL vec4f MAROON = vec4f(190, 33, 55, 255);
INTERNAL vec4f GREEN = vec4f(0, 228, 48, 255);
INTERNAL vec4f LIME = vec4f(0, 158, 47, 255);
INTERNAL vec4f DARKGREEN = vec4f(0, 117, 44, 255);
INTERNAL vec4f SKYBLUE = vec4f(102, 191, 255, 255);
INTERNAL vec4f BLUE = vec4f(0, 121, 241, 255);
INTERNAL vec4f DARKBLUE = vec4f(0, 82, 172, 255);
INTERNAL vec4f PURPLE = vec4f(200, 122, 255, 255);
INTERNAL vec4f VIOLET = vec4f(135, 60, 190, 255);
INTERNAL vec4f DARKPURPLE = vec4f(112, 31, 126, 255);
INTERNAL vec4f BEIGE = vec4f(211, 176, 131, 255);
INTERNAL vec4f BROWN = vec4f(127, 106, 79, 255);
INTERNAL vec4f DARKBROWN = vec4f(76, 63, 47, 255);

INTERNAL vec4f WHITE = vec4f(255, 255, 255, 255);
INTERNAL vec4f BLACK = vec4f(0, 0, 0, 255);
INTERNAL vec4f BLANK = vec4f(0, 0, 0, 0);
INTERNAL vec4f MAGENTA = vec4f(255, 0, 255, 255);

#endif