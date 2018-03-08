///////////////////////////////////////////////////////////////////////////
// FILE:                       audio.h                                   //
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

#ifndef AUDIO_H
#define AUDIO_H

#include <GL/glew.h>
#include "defines.h"

struct Sound {
	GLuint source;
	GLuint buffer;
	GLint format;
};

struct AudioStream {
	GLuint sampleRate;
	GLuint sampleSize;
	GLuint channels;

	GLint format;
	GLuint source;
	GLuint buffers[2];
};

void setMasterVolume(float volume);
float getMasterVolume();

Sound loadSound(const char* filename);

void setSoundData(Sound& sound, const void* data, GLint samplesCount);

void playSound(Sound sound);
void pauseSound(Sound sound);
void resumeSound(Sound sound);
void stopSound(Sound sound);
bool isSoundPlaying(Sound sound);
void setSoundVolume(Sound sound, float volume);
void setSoundPitch(Sound sound, float pitch);

void disposeSound(Sound& sound);

#endif