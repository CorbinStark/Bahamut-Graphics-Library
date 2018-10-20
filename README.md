# Bahamut Game Library
2D and 3D compatible game library created in C++ using OpenGL, OpenAL, and GLFW. Bahamut aims to streamline and simplify the games programming experience. Easy to learn, easy to master.

## Documentation

This game library aims to simplify the games programming experience. As such there is only one window,
so all functions related to input and drawing are global and can be called from anywhere.

To initialize the window

```cpp
init_window(width, height, title, fullscreen, resizable, primary_monitor);

init_window(800, 600, "Window Title!", false, true, 0);
```

After initialization, the features of the library become available. The available functions are listed here,

### General

#### window.h

```cpp
void set_window_pos(int x, int y);
void set_window_size(int width, int height);
void begin_drawing();
void end_drawing();
bool window_closed();
bool window_open();

void set_clear_color(float r, float g, float b, float a);
void set_clear_color(vec4 color);
void set_key_callback(void(*keyCallback)(int key, int action));
void set_mouse_callback(void(*mouseCallback)(double mousex, double mousey, int button, int action));
void set_window_resize_callback(void(*resizecallback)(int width, int height));

double get_elapsed_time();

void set_window_should_close(bool shouldClose);
void dispose_window();

void set_FPS_cap(double FPS);

void set_mouse_locked(bool locked);
void set_mouse_hidden(bool hidden);
void set_vsync(bool vsync);

void set_viewport(int x, int y, int width, int height);
void resize_viewport(int width, int height);

void set_virtual_size(int v_width, int v_height);
int get_virtual_width();
int get_virtual_height();
vec2 get_virtual_size();
int get_window_width();
int get_window_height();
```

### Input

#### window.h

```cpp
int get_key_pressed();
int get_button_pressed();
int get_key_released();
int get_button_released();
double get_scroll_x();
double get_scroll_y();
bool is_key_pressed(unsigned int keycode);
bool is_key_released(unsigned int keycode);
bool is_button_pressed(unsigned int button);
bool is_button_released(unsigned int button);
bool is_key_down(unsigned int keycode);
bool is_button_down(unsigned int button);
bool is_key_up(unsigned int keycode);
bool is_button_up(unsigned int button);

void get_mouse_pos(double* mousexPtr, double* mouseyPtr);
vec2 get_mouse_pos();
void get_mouse_pos_adjusted(double* mousexPtr, double* mouseyPtr, Rect viewport);
vec2 get_mouse_pos_adjusted(Rect viewport);
```

### Textures

#### texture.h

```cpp
Texture create_blank_texture(unsigned int width = 0, unsigned int height = 0);
Texture load_texture(unsigned char* pixels, unsigned int width, unsigned int height, unsigned int param);
Texture load_texture(const char* filepath, unsigned int param);
void dispose_texture(Texture& texture);

void set_texture_pixels(Texture texture, unsigned char* pixels, unsigned int width, unsigned int height);
void set_texture_pixels_from_file(Texture texture, const char* filepath);

void bind_texture(Texture texture, unsigned int slot);
void unbind_texture(unsigned int slot);

void set_texture_wrap_x(Texture texture, u32 type);
void set_texture_wrap_y(Texture texture, u32 type);

Framebuffer create_framebuffer(u32 width, u32 height, u16 param, u8 buffertype);
Framebuffer create_colorbuffer(u32 width, u32 height, u16 param);
Framebuffer create_depthbuffer(u32 width, u32 height, u16 param);
void dispose_framebuffer(Framebuffer buffer);
void bind_framebuffer(Framebuffer buffer);
void unbind_framebuffer();
void clear_bound_framebuffer();
```

### Drawing

begin_drawing() prepares the window, and end_drawing() clears the window.
begin2D takes a shader and prepares a buffer that draw commands go into, end2D flushes that buffer.

#### Example

```cpp
while(true) {
	begin_drawing();
	begin2D(shader);
	
	draw_rectangle(50, 50, 100, 100);
	
	end2D();
	end_drawing();
}
```
3D also works in a very similar fashion.

```cpp
while(true) {
	begin_drawing();
	begin2D(shader);
	
	draw_rectangle(50, 50, 100, 100);
	
	end2D();
	
	begin3D(shader);
	
	draw_cube(V3(0, 0, 0), V3(2, 2, 2), V3(0, 90, 0), RED);
	
	end3D();
	end_drawing();
}
```

#### render2D.h

```cpp
void begin2D(Shader shader, bool blending = true, bool depthTest = false);

void draw_texture(Texture tex, i32 xPos, i32 yPos);
void draw_texture(Texture tex, i32 xPos, i32 yPos, i32 width, i32 height);
void draw_texture(Texture tex, i32 xPos, i32 yPos, f32 r, f32 g, f32 b, f32 a);
void draw_texture(Texture tex, i32 xPos, i32 yPos, vec4 color);

void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, f32 rotateDegree);
void draw_texture_rotated(Texture tex, i32 xPos, i32 yPos, vec2 origin, f32 rotation);

void draw_texture_EX(Texture tex, Rect source, Rect dest);
void draw_texture_EX(Texture tex, Rect source, Rect dest, f32 r, f32 g, f32 b, f32 a);
void draw_texture_EX(Texture tex, Rect source, Rect dest, vec4 color);
void draw_texture_EX(Texture tex, Rect source, Rect dest, vec2 origin);

void draw_framebuffer(Framebuffer buffer, i32 xPos, i32 yPos);

void draw_rectangle(i32 x, i32 y, i32 width, i32 height, f32 r, f32 g, f32 b, f32 a);
void draw_rectangle(i32 x, i32 y, i32 width, i32 height, vec4 color);

void draw_text(Font& font, const char* str, i32 xPos, i32 yPos, f32 r = 255.0f, f32 g = 255.0f, f32 b = 255.0f);
void draw_text(Font& font, std::string str, i32 xPos, i32 yPos, f32 r = 255.0f, f32 g = 255.0f, f32 b = 255.0f);

void end2D();

f32 get_blackbar_width(f32 aspect);
f32 get_blackbar_height(f32 aspect);
Rect fit_aspect_ratio(f32 aspect);
Shader load_default_shader_2D();
```

### Font

#### Example

```cpp
Font font = load_font("data/font.ttf", 28);
while(true) {
	begin_drawing();
	begin2D();
	
	draw_text(font, "Hello World", 50, 50);
	
	end2D();
	end_drawing();
}
```
#### font.h

```cpp
Texture create_texture_from_string(Font& font, const std::string str);
Texture create_texture_from_string(Font& font, const std::string str, unsigned char r, unsigned char g, unsigned char b);
Font load_font(const GLchar* filepath, unsigned int size);
void dispose_font(Font& font);

float get_font_height(Font& font);

const char* format_text(const char* text, ...);
const u32 get_string_width(Font& font, const char* str);
```

### Entity-Component-System

#### entity.h

```cpp
template<class T, class... TArgs>
T* add_comp(TArgs&&... mArgs);
    
template<class T>
T* get_comp();
    
template<class T>
void remove_comp();
```

### Maths

#### maths.h

```cpp
vec2 V2(f32 x, f32 y);
vec3 V3(f32 x, f32 y, f32 z);
vec3 V3(vec2 v2, f32 z);
vec4 V4(f32 x, f32 y, f32 z, f32 w);
vec4 V4(vec3 v3, f32 w);
vec4 V4(vec2 v21, vec2 v22);

f32 deg_to_rad(f32 deg);
f32 rad_to_deg(f32 rad);

f32 length(vec2 vec);
f32 length(vec3 vec);
f32 length(vec4 vec);

void normalize(vec2* vec);
void normalize(vec3* vec);
void normalize(vec4* vec);

vec2 normalize(vec2 vec);
vec2 normalize(vec3 vec);
vec2 normalize(vec4 vec);

f32 dot(vec2 a, vec2 b);
f32 dot(vec3 a, vec3 b);
f32 dot(vec4 a, vec4 b);

vec3 cross(vec3 a, vec3 b);

mat4 identity();
mat4 translation(const f32 x, const f32 y, const f32 z);
mat4 translation(const vec3 translation_vec);
mat4 scale(const f32 x, const f32 y, const f32 z);
mat4 scale(const vec3& scale_vec);
mat4 rotateX(f32 angle);
mat4 rotateY(f32 angle);
mat4 rotateZ(f32 angle);
mat4 rotation(f32 angle, f32 x, f32 y, f32 z);
mat4 rotation(f32 angle, const vec3 axis);
mat4 orthographic_projection(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane);
mat4 perspective_projection(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane);
mat4 create_transformation_matrix(f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ);
mat4 create_transformation_matrix(const vec3 translation, const vec3 rotation, const vec3 scale_vec);
mat4 inverse(const mat4 mat);
bool point_inside_triangle(vec3 point, vec3 tri1, vec3 tri2, vec3 tri3);
mat4 look_at(const vec3 camera, const vec3 center, const vec3 up = V3(0, 1, 0));

mat4 create_view_matrix(Camera cam);
void move_cam_forward(Camera cam, f32 units);
void move_cam_backwards(Camera cam, f32 units);
void move_cam_right(Camera cam, f32 units);
void move_cam_left(Camera cam, f32 units);
```

### Collision Detection

##### Example

```cpp
Rect first = rect(60, 60, 5, 5);
Rect second = rect(50, 50, 10, 10);
if(colliding(first, second)) {
	std::cout << "Collision!" << std::endl;
}
```

#### rectangle.h

```cpp
Rect rect(float x, float y, float width, float height);

bool colliding(Rect first, Rect second);
bool colliding(Rect rect, float x, float y, float width, float height);
bool colliding(Rect rect, float x, float y);

```

### Sample program

```cpp
int main() {
	//initialization
	init_window(800, 600, "Minigames", false, true, 1);

	//set window variables
	set_FPS_cap(60);
	set_vsync(false);
	set_clear_color(0, 0, 0, 255);

	//load basic 2D shader
	Shader basic = load_default_shader_2D();

	//main loop
	while (window_open()) {
		vec2 mousePos = get_mouse_pos();
		
		begin_drawing();
		set_viewport(0, 0, get_window_width(), get_window_height());

		//draw rectangle
		begin2D(basic);
			upload_mat4(basic, "projection", orthographic_projection(0, 0, get_window_width(), get_window_height(), -1, 1));
			draw_rectangle(100, 100, 150, 150, RED);
			draw_rectangle(mousePos.x, mousePos.y, 25, 25, WHITE);
		end2D();

		end_drawing();
	}

	//end
	dispose_window();

	return 0;
}
```

## Linking the library into Visual Studio

> If you aren't using Visual Studio 2017, this may not work. Upgrading to Visual Studio 2017 is highly recommended. Any 'p1' and 'p2' mismatch is likely due to having a wrong Visual Studio version. I could also recompile the engine on a different VS version if that is preferable.

If you are not using Visual Studio, make sure you are using an up-to-date compiler and link in the .lib

After creating your project, right click it and select properties.
Make sure your configuration is in "All Configurations" before continuing
```
Properties > VC++ Directories > Include Directories
```
Edit the include directories, and add the BahamutInclude directory to it.

```
Properties > Linker > Input > Additional Dependencies
```
Edit the additional dependencies and type the following into it:
```
bahamut.lib
```
Next, go to:
```
Properties > System > SubSystem
```
Change your SubSystem to Console.

Then change your configuration to debug.

```
Properties > VC++ Directories > Library Directories
```
Edit the library directories, and add the DebugBahamutLibrary directory to it.
Once finished with that, change your configuration to release.

```
Properties > VC++ Directories > Library Directories
```
Edit the library directories, and add the BahamutLibrary directory to it.

Finished!
Contact me if this did not work for you.

## Contact

Email: starkcorbin@gmail.com
