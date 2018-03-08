///////////////////////////////////////////////////////////////////////////
// FILE:                       render2D.h                                //
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

#ifndef RENDER2D_H
#define RENDER2D_H

#include "image.h"
#include "shader.h"
#include "font.h"
#include "rectangle.h"
#include "renderTexture.h"
#include "defines.h"

#define BATCH_MAX_SPRITES	    20000
#define BATCH_VERTEX_SIZE	    sizeof(VertexData)
#define BATCH_SPRITE_SIZE	    BATCH_VERTEX_SIZE * 4
#define BATCH_BUFFER_SIZE	    BATCH_SPRITE_SIZE * BATCH_MAX_SPRITES
#define BATCH_INDICE_SIZE	    BATCH_MAX_SPRITES * 6
#define BATCH_MAX_TEXTURES		16

enum StretchMode {
	STRETCH_NONE,
	STRETCH_PROJECTION,
	STRETCH_VIEWPORT
};

enum AspectMode {
	ASPECT_NONE,
	ASPECT_KEEP,
	ASPECT_KEEP_WIDTH,
	ASPECT_KEEP_HEIGHT
};

//==========================================================================================
//Description: Initializes the 2D renderer with all the data it needs
//
//Parameters: 
//		-A rectangle (x, y, width, height) for the projection matrix (the viewport)
//			of the render batch.
//==========================================================================================
void init2D(int x, int y, int width, int height);
//==========================================================================================
//Description: Begins the renderer. You must do all draw calls in between
//	begin2D and end2D.
//==========================================================================================
void begin2D();
void begin2D(mat4f projection);
void begin2D(Shader shader);
void begin2D(Shader shader, mat4f projection);
//==========================================================================================
//Description: Draws a texture onto the bound framebuffer (by default the window)
//
//Parameters: 
//		-A texture to render
//		-An x and y position to render to
//
//Comments: If the texture has it's flip_flag set to FLIP_HORIZONTAL or 
//		FLIP_VERTICAL or both, it will be flipped accordingly when drawn.
//==========================================================================================
void drawTexture(Texture& tex, int xPos, int yPos);
//==========================================================================================
//Description: Draws a texture onto the bound framebuffer (by default the window)
//
//Parameters: 
//		-A texture to render
//		-An x and y position to render to
//		-a color (RGBA) to multiply the texture with. If the texture is white all white 
//			parts will be turned to that color. nonwhite parts will combine with that color
//
//Comments: If the texture has it's flip_flag set to FLIP_HORIZONTAL or 
//		FLIP_VERTICAL or both, it will be flipped accordingly when drawn.
//==========================================================================================
void drawTexture(Texture& tex, int xPos, int yPos, float r, float g, float b, float a);
void drawTextureRot(Texture& tex, int xPos, int yPos, float rotateDegree);
void drawTextureRot(Texture& tex, int xPos, int yPos, vec2f origin, float rotation);
void drawTextureEX(Texture& tex, Rect source, Rect dest);
void drawTextureEX(Texture& tex, Rect source, Rect dest, vec2f origin);
void drawRenderTexture(RenderTexture& tex, int xPos, int yPos);
void drawRectangle(int x, int y, int width, int height, float r, float g, float b, float a);
void drawRectangle(int x, int y, int width, int height, vec4f& color);
void drawText(Font& font, std::string str, int x, int y);
void drawText(Font& font, std::string str, int xPos, int yPos, float r, float g, float b);
//===============================================================================
//Description: Ends and flushes the renderer. You must do all draw calls in between
//	begin2D and end2D. You can optionally attach a shader before calling begin2D.
//===============================================================================
void end2D();

int submitTex(Texture& tex);

void setStretchMode(StretchMode mode);
void setAspectMode(AspectMode mode);

Rect getViewportRect();

void set2DRenderViewport(int x, int y, int width, int height, int virtual_width, int virtual_height);
void attachShader2D(Shader shader);

void dispose2D();

GLuint inline rgbaToUint(int r, int g, int b, int a) {
	return a << 24 | b << 16 | g << 8 | r;
}

#endif