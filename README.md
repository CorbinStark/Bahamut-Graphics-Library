# BahamutEngine
2D and 3D compatible game engine created in C++ using OpenGL, OpenAL, and GLFW. BahamutEngine aims to streamline and simplify the games programming experience. Easy to learn, easy to master - without sacrificing power.

## Linking the engine into Visual Studio

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

## Documentation

The engine utilized aims to simplify the games programming experience. As such there is only one window,
so all functions related to input and drawing are global and can be called from anywhere.

To initialize the window

```cpp
initWindow(width, height, title, fullscreen, resizable, primary_monitor);

initWindow(800, 600, "Attack of the Unidead", false, true, 0);
```

After initialization, the features of the engine become available. The available functions are listed here,

### General

#### window.h

```cpp
void setWindowPos(int x, int y);
void setWindowSize(int width, int height);
void beginDrawing();
void endDrawing();
bool isWindowClosed();

void setClearColor(float r, float g, float b, float a);
void setClearColor(vec4f& color);
void setWindowResizeCallback(void(*resizecallback)(int width, int height));

void getMousePos(double* mousex, double* mousey);
vec2f getMousePos();
void disposeWindow();

void setFPSCap(double FPS);

void setMouseLocked(bool locked);
void setMouseHidden(bool hidden);
void setVSync(bool vsync);

int getWindowWidth();
int getWindowHeight();
double getElapsedTime();
```

### Input

#### window.h

```cpp
bool isKeyPressed(unsigned int keycode);
bool isKeyReleased(unsigned int keycode);
bool isButtonPressed(unsigned int button);
bool isButtonReleased(unsigned int button);
bool isKeyDown(unsigned int keycode);
bool isButtonDown(unsigned int button);
bool isKeyUp(unsigned int keycode);
bool isButtonUp(unsigned int button);
```

### Textures

#### texture.h

```cpp
Texture createBlankTexture(int width = 0, int height = 0);
Texture loadTexture(unsigned char* pixels, int width, int height, int param);
Texture loadTexture(std::string filepath, int param);
void disposeTexture(Texture& texture);

void blitTexture(Texture& src, Texture& dest, Rectangle drawFrom, Rectangle drawTo);

void setTexturePixels(Texture& texture, unsigned char* pixels, int width, int height);
void setTexturePixelsFromFile(Texture& texture, std::string filepath);

void bindTexture(Texture& texture, unsigned int slot);
void unbindTexture(unsigned int slot);
```

### Drawing

All drawing must be done in between begin2D() and end2D().
begin2D() and end2D() must be in between beginDrawing() and endDrawing()

#### Example

```cpp
while(true) {
	beginDrawing();
	begin2D();
	
	drawRectangle(50, 50, 100, 100);
	
	end2D();
	endDrawing();
}
```
beginDrawing and endDrawing handle clearing and updating the screen, in addition to a few other things like capping framerates.
begin2D() prepares a buffer to be added to using the draw functions, and end2D() flushes the buffer to the screen (in the order the draw functions were called).

#### render2D.h

```cpp
void begin2D();
void drawTexture(Texture& tex, int xPos, int yPos);
void drawTexture(Texture& tex, int xPos, int yPos, float r, float g, float b, float a);
void drawTextureRot(Texture& tex, int xPos, int yPos, float rotateDegree);
void drawTextureRot(Texture& tex, int xPos, int yPos, vec2f origin, float rotation);
void drawTextureEX(Texture& tex, Rectangle source, Rectangle dest);
void drawTextureEX(Texture& tex, Rectangle source, Rectangle dest, vec2f origin);
void drawRectangle(int x, int y, int width, int height, float r, float g, float b, float a);
void drawRectangle(int x, int y, int width, int height, vec4f& color);
void drawText(Font& font, std::string str, int x, int y);
void drawText(Font& font, std::string str, int xPos, int yPos, float r, float g, float b);
void end2D();

void set2DRenderViewport(int x, int y, int width, int height);
GLuint rgbaToUint(int r, int g, int b, int a);
```

### Font

#### Example

```cpp
Font font = loadFont("data/font.ttf", 28);
while(true) {
	beginDrawing();
	begin2D();
	
	drawText(font, "Hello World", 50, 50);
	
	end2D();
	endDrawing();
}
```
#### font.h

```cpp
Texture createTextureFromString(Font& font, const std::string str, GLubyte r = 0, GLubyte g = 0, GLubyte b = 0);
Font loadFont(const GLchar* filepath, unsigned int size);
void disposeFont(Font& font);

const char* formatText(const char* text, ...);
```

### Entity-Component-System

#### entity.h

```cpp
template<class T, class... TArgs>
T* addComp(TArgs&&... mArgs);
    
template<class T>
T* getComp();
    
template<class T>
void removeComp();
```

### Maths

#### mat4f.h

```cpp
mat4f(float diagonal);
static mat4f identity();
static mat4f orthographic(float x, float y, float width, float height, float near, float far);
static mat4f perspective(float fov, float aspectRatio, float near, float far);
static mat4f createTransformationMatrix(const vec3f& translation, const vec3f& rotation, const vec3f& scale);
static mat4f createTransformationMatrix(const float x, const float y, const float z, 
	const float rotX, const float rotY, const float rotZ, 
	const float scaleX, const float scaleY, const float scaleZ
);
static mat4f createViewMatrix(Camera& cam);

static mat4f translation(const vec3f& translation);
static mat4f translation(const float x, const float y, const float z);
static mat4f rotation(float angle, const vec3f& axis);
static mat4f rotation(float angle, const float axisX, const float axisY, const float axisZ);
static mat4f rotationX(float angle);
static mat4f rotationY(float angle);
static mat4f rotationZ(float angle);
static mat4f scaleMatrix(const vec3f& scale);
static mat4f scaleMatrix(const float scaleX, const float scaleY, const float scaleZ);

void translate(const vec3f& translation);
void translate(const float x, const float y, const float z);

void rotate(const float angle, const vec3f& axis);
void rotate(const float angle, const float axisX, const float axisY, const float axisZ);

void scale(const vec3f& scale);
void scale(const float scaleX, const float scaleY, const float scaleZ);

mat4f& multiply(const mat4f& other);
friend mat4f operator*(mat4f left, const mat4f& right);
mat4f& operator*=(const mat4f& other);
friend std::ostream& operator<<(std::ostream& stream, const mat4f& mat);
```

### Collision Detection

##### Example

```cpp
Rectangle first(60, 60, 5, 5);
Rectangle second(50, 50, 10, 10);
if(colliding(first, second)) {
	std::cout << "Collision!" << std::endl;
}
```

#### rectangle.h

```cpp
Rectangle();
Rectangle(float x, float y, float width, float height);

bool colliding(Rectangle& first, Rectangle& second);
bool colliding(Rectangle& rect, float x, float y, float width, float height);
bool colliding(Rectangle& rect, vec2f& point);
```

### Sample program

```cpp
void main() {
    initWindow(800, 600, "Attack of the Unidead", false, true, 0);

    int pos_x = 0;
    int pos_y = 0;

    setMouseHidden(true);
    Texture cursor = loadTexture("data/art/cursor.png", TEXTURE_PARAM);

    while (!isWindowClosed()) {
        beginDrawing();

        if (isKeyDown(KEY_RIGHT))    pos_x -= SCROLL_SPEED;
        if (isKeyDown(KEY_LEFT))    pos_x += SCROLL_SPEED;
        if (isKeyDown(KEY_DOWN))    pos_y -= SCROLL_SPEED;
        if (isKeyDown(KEY_UP))        pos_y += SCROLL_SPEED;

        begin2D();
            drawTexture(cursor, getMousePos().x, getMousePos().y);
        end2D();

        endDrawing();
    }
    disposeTexture(cursor);
    disposeWindow();
}
```
## Contact

Email: starkcorbin@gmail.com
