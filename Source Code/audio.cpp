///////////////////////////////////////////////////////////////////////////
// FILE:                       audio.cpp                                 //
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

#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AL/alc.h>
#include <AL/al.h>

#if defined(_WIN32) || defined(_WIN64)
#define strtok_r strtok_s
#endif

//INTERNAL VARIABLES
INTERNAL u8 master_volume;

INTERNAL
struct SoundData {
	GLuint sampleCount;
	GLuint sampleRate;
	GLuint sampleSize;
	GLuint channels;
	void* data;
};

INTERNAL
SoundData loadWAV(const char* filename) {
	SoundData data;

	return data;
}
INTERNAL
SoundData loadOGG(const char* filename) {
	SoundData data;
	return data;
}
INTERNAL
SoundData loadFLAC(const char* filename) {
	SoundData data;
	return data;
}

INTERNAL
char *duplicateString(const char *s) {
	char *d = (char*)malloc(strlen(s) + 1);
	if (d == NULL) return NULL;
	strcpy(d, s);
	return d;
}

INTERNAL
char** splitString(const char* string, const char* seperator, u32* numTokens) {
	char** tokens;
	char* s = duplicateString(string);

	u32 tokens_allocated = 1;
	u32 tokens_used = 0;
	tokens = (char**)calloc(tokens_allocated, sizeof(char*));
	char* token;
	char* strtok_ctx;
	for (token = strtok_r(s, seperator, &strtok_ctx); token != NULL; token = strtok_r(NULL, seperator, &strtok_ctx)) {
		if (tokens_used == tokens_allocated) {
			tokens_allocated *= 2;
			tokens = (char**)realloc(tokens, tokens_allocated * sizeof(char*));
		}
		tokens[tokens_used++] = duplicateString(token);
	}
	if (tokens_used == 0) {
		free(tokens);
		tokens = NULL;
	}
	else {
		tokens = (char**)realloc(tokens, tokens_used * sizeof(char*));
	}
	*numTokens = tokens_used;
	free(s);
	return tokens;
}

INTERNAL
bool hasExtension(const char* filename, const char* extension) {
	u32 numTokens = 0;
	char** tokens;
	tokens = splitString(filename, ".", &numTokens);
	bool answer = strcmp(tokens[numTokens - 1], extension) == 0;

	for (u8 i = 0; i < numTokens; ++i)
		if (tokens[i] != NULL)
			free(tokens[i]);
	if(tokens != NULL)
		free(tokens);

	return answer;
}

void initAudio() {
	ALCdevice *device = alcOpenDevice(NULL);

	if (!device) 
		BMT_LOG(FATAL_ERROR, "Could not open audio device!");
	else {
		ALCcontext *context = alcCreateContext(device, NULL);

		if ((context == NULL) || (alcMakeContextCurrent(context) == ALC_FALSE)) {
			if (context != NULL) alcDestroyContext(context);
			alcCloseDevice(device);
			BMT_LOG(FATAL_ERROR, "Could not initialize audio context!");
		}
		else {
			BMT_LOG(INFO, "Audio device and context initialized successfully: %s", alcGetString(device, ALC_DEVICE_SPECIFIER));

			alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
			alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
			alListener3f(AL_ORIENTATION, 0.0f, 0.0f, -1.0f);
			alListenerf(AL_GAIN, 1.0f);
		}
	}
}

void disposeAudio() {
	ALCdevice *device;
	ALCcontext *context = alcGetCurrentContext();

	if (context == NULL) 
		BMT_LOG(WARNING, "Could not get current audio context for closing.");

	device = alcGetContextsDevice(context);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	BMT_LOG(INFO, "Audio device closed.");
}

void setMasterVolume(u8 volume) {
	master_volume = volume;
	alListenerf(AL_GAIN, (float)volume / 255.0f);
}

u8 getMasterVolume() {
	return master_volume;
}

Sound loadSound(const char* filename) {
	Sound sound;
	SoundData data;

	if (hasExtension(filename, "wav"))       data = loadWAV(filename);
	else if (hasExtension(filename, "ogg"))  data = loadOGG(filename);
	else if (hasExtension(filename, "flac")) data = loadFLAC(filename);
	else {
		BMT_LOG(FATAL_ERROR, "[%s] Extension not supported!", filename);
	}

	sound.format = 0;
	if (data.channels == 1) {
		if (data.sampleSize == 8)       sound.format = AL_FORMAT_MONO8;
		else if (data.sampleSize == 16) sound.format = AL_FORMAT_MONO16;
		else BMT_LOG(WARNING, "Sample size not supported: %i", data.sampleSize);
	}
	else if (data.channels == 2) {
		if (data.sampleSize == 8)       sound.format = AL_FORMAT_STEREO8;
		else if (data.sampleSize == 16) sound.format = AL_FORMAT_STEREO16;
		else BMT_LOG(WARNING, "Sample size not supported: %i", data.sampleSize);
	}
	else
		BMT_LOG(WARNING, "Only MONO and STEREO channels are supported.");

	alGenSources(1, &sound.src);
	alSourcef(sound.src, AL_PITCH, 1.0f);
	alSourcef(sound.src, AL_GAIN, 1.0f);
	alSource3f(sound.src, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(sound.src, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(sound.src, AL_LOOPING, AL_FALSE);

	alGenBuffers(1, &sound.buffer);
	u32 buffer_size = data.channels * data.sampleCount * data.sampleSize / 8;
	alBufferData(sound.buffer, sound.format, data.data, buffer_size, data.sampleRate);
	alSourcei(sound.src, AL_BUFFER, sound.buffer);

	return sound;
}