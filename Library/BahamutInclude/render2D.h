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

#include "defines.h"
#include "shader.h"
#include "font.h"

struct VertexData {
	vec2 pos;
	vec4 color; //32 bit color (8 for R, 8 for G, 8 for B, 8 for A)
	vec2 uv;
	f32 texid;
};

#ifndef BATCH_MAX_SPRITES
#define BATCH_MAX_SPRITES	    20000
#endif

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
void init2D(i32 x, i32 y, u32 width, u32 height);
//==========================================================================================
//Description: Begins the renderer. You must do all draw calls in between
//	begin2D and end2D.
//==========================================================================================
void begin2D();
void begin2D(mat4 projection);
void begin2D(Shader shader);
void begin2D(Shader shader, mat4 projection);
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
void draw_texture(Texture tex, i32 xPos, i32 yPos);
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
void draw_texture(Texture tex, i32 xPos, i32 yPos, f32 r, f32 g, f32 b, f32 a);
void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, f32 rotateDegree);
void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, vec2 origin, f32 rotation);
void draw_texture_EX(Texture tex, Rect source, Rect dest);
void draw_texture_EX(Texture tex, Rect source, Rect dest, vec2 origin);
void draw_render_texture(RenderTexture tex, i32 xPos, i32 yPos);
void draw_rectangle(i32 x, i32 y, i32 width, i32 height, f32 r, f32 g, f32 b, f32 a);
void draw_rectangle(i32 x, i32 y, i32 width, i32 height, vec4 color);
void draw_text(Font& font, std::string str, i32 x, i32 y);
void draw_text(Font& font, std::string str, i32 xPos, i32 yPos, f32 r, f32 g, f32 b);
//===============================================================================
//Description: Ends and flushes the renderer. You must do all draw calls in between
//	begin2D and end2D.
//===============================================================================
void end2D();

//StretchMode only applies to 2D
void set_stretch_mode(StretchMode mode);
//AspectMode only applies to 2D
void set_aspect_mode(AspectMode mode);

Rect get_viewport_rect();
void set_2D_render_viewport(i32 x, i32 y, u32 width, u32 height, u32 virtual_width, u32 virtual_height);

void dispose2D();

u32 inline rgba_to_u32(i32 r, i32 g, i32 b, i32 a) {
	return a << 24 | b << 16 | g << 8 | r;
}

#endif