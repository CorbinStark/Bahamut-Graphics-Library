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

#if defined(BMT_USE_NAMESPACE) 
namespace bmt {
#endif

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
//
//Parameters: 
//		-A shader to process the draw commands. Use load_default_shader_2D() to get the
//			default shader.
//		-(OPTIONAL) Whether or not to blend alpha (default = true)
//		-(OPTIONAL) Whether or not to test depth (default = false)
//==========================================================================================
void begin2D(Shader shader, bool blending = true, bool depthTest = false);
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
//		-A width and height to stretch the texture to (this is offered just in case you don't feel
//			like changing the texture's width and height, then changing it back.)
//
//Comments: If the texture has it's flip_flag set to FLIP_HORIZONTAL or 
//		FLIP_VERTICAL or both, it will be flipped accordingly when drawn.
//==========================================================================================
void draw_texture(Texture tex, i32 xPos, i32 yPos, i32 width, i32 height);
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
void draw_texture(Texture tex, i32 xPos, i32 yPos, vec4 color);
//==========================================================================================
//Description: Draws a rotated texture onto the bound framebuffer (by default the window)
//
//Parameters: 
//		-A texture to render
//		-An x and y position to render to
//		-a degree to rotate by
//
//Comments: If the texture has it's flip_flag set to FLIP_HORIZONTAL or 
//		FLIP_VERTICAL or both, it will be flipped accordingly when drawn.
//==========================================================================================
void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, f32 rotateDegree);
//==========================================================================================
//Description: Draws a rotated texture onto the bound framebuffer (by default the window)
//
//Parameters: 
//		-A texture to render
//		-An x and y position to render to
//		-an origin to rotate about
//		-a degree to rotate by
//
//Comments: If the texture has it's flip_flag set to FLIP_HORIZONTAL or 
//		FLIP_VERTICAL or both, it will be flipped accordingly when drawn.
//==========================================================================================
void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, vec2 origin, f32 rotation);
//==========================================================================================
//Description: Draws a portion of a texture onto the portion of the bound framebuffer 
//			   (by default the window)
//
//Parameters: 
//		-A texture to render
//		-A square area to render from the texture
//		-A square area to render onto the bound framebuffer
//		-OPTIONAL - A color(RGBA) to multiply with
//
//Comments: If the texture has it's flip_flag set to FLIP_HORIZONTAL or 
//		FLIP_VERTICAL or both, it will be flipped accordingly when drawn.
//==========================================================================================
void draw_texture_EX(Texture tex, Rect source, Rect dest);
void draw_texture_EX(Texture tex, Rect source, Rect dest, f32 r, f32 g, f32 b, f32 a);
void draw_texture_EX(Texture tex, Rect source, Rect dest, vec4 color);
void draw_texture_EX(Texture tex, Rect source, Rect dest, vec2 origin);
//==========================================================================================
//Description: Draws a framebuffer onto another bound framebuffer (by default the window)
//
//Parameters: 
//		-A framebuffer to render
//		-An x and y position
//
//Comments: If the texture has it's flip_flag set to FLIP_HORIZONTAL or 
//		FLIP_VERTICAL or both, it will be flipped accordingly when drawn.
//		Calling a draw_texture function with the framebuffer's texture will also work.
//==========================================================================================
void draw_framebuffer(Framebuffer buffer, i32 xPos, i32 yPos);
//==========================================================================================
//Description: Draws a rectangle onto the bound framebuffer (by default the window)
//
//Parameters: 
//		-An x, y, width, and height
//		-A color(RGBA)
//==========================================================================================
void draw_rectangle(i32 x, i32 y, i32 width, i32 height, f32 r, f32 g, f32 b, f32 a);
void draw_rectangle(i32 x, i32 y, i32 width, i32 height, vec4 color);
//==========================================================================================
//Description: Draws a string onto the bound framebuffer (by default the window)
//
//Parameters: 
//		-A font to take character textures from
//		-A string to draw
//		-An x and y position to draw at
//		-A color(RGBA)
//==========================================================================================
void draw_text(Font& font, const char* str, i32 xPos, i32 yPos, f32 r = 255.0f, f32 g = 255.0f, f32 b = 255.0f);
void draw_text(Font& font, std::string str, i32 xPos, i32 yPos, f32 r = 255.0f, f32 g = 255.0f, f32 b = 255.0f);
//===============================================================================
//Description: Ends and flushes the renderer. You must do all draw calls in between
//	begin2D and end2D.
//===============================================================================
void end2D();

f32 get_blackbar_width(f32 aspect);
f32 get_blackbar_height(f32 aspect);
Rect limit_to_aspect_ratio(f32 aspect);
Shader load_default_shader_2D();

void dispose2D();

u32 inline rgba_to_u32(i32 r, i32 g, i32 b, i32 a) {
	return a << 24 | b << 16 | g << 8 | r;
}

#if defined(BMT_USE_NAMESPACE) 
}
#endif

#endif