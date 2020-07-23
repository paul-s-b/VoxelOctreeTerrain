#ifndef WINDOW_H_
#define WINDOW_H_

#define GLM_ENABLE_EXPERIMENTAL
#define GLFW_INCLUDE_GLCOREARB

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <stdexcept>

void keyCallback(GLFWwindow*, int key, int, int action, int);
void mouseButtonCallback(GLFWwindow*, int button, int action, int);
void mousePositionCallback(GLFWwindow*, double x, double y);
void windowResizeCallback(GLFWwindow*, int width, int height);

//! Application window
/*!
Currently windows represent an application window
and the ability to specify a subsection that application window
for rendering.
All coordinates are in normalized device coordinates.
*/
class Window {
	friend void	keyCallback(GLFWwindow*, int key, int, int action, int);
	friend void	mouseButtonCallback(GLFWwindow*, int button, int action, int);
	friend void	mousePositionCallback(GLFWwindow*, double x, double y);
	friend void	windowResizeCallback(GLFWwindow*, int width, int hieght);
	static void	errorCallback(int, const char*);
	static void	windowMoveCallback(GLFWwindow*, int, int);
	GLFWwindow* _window;

	float _screenCornerX;
	float _screenCornerY;
	float _width;
	float _height;
	int _windowWidth;
	int _windowHeight;

	void windowHints(void);
	void refreshRenderMask(void);

	//! Array of bools indicating if a key is down.
	bool _keys[512] = { false };

	//! Array of bools indicating if a mouse button is pressed.
	bool _mouseButtons[8] = { false };

	//! Array of bools indicating if mouse button was clicked
	bool _mouseButtonClicked[8] = { false };

	glm::vec2 _mousePosition = glm::vec2(0.0f, 0.0f);
public:

	//! Create a new window with x width and y height the window title is set to name.
	Window(int x, int y, std::string name);

	//! Size of the framebuffer in pixels, ignores RenderMask
	void getMaxRenderSize(float& width, float& height);

	//! Size of the OS Application window in pixels.
	void getWindowSize(int& width, int& height);

	//! Gets the aspect ratio of the currently drawable area.
	float getAspect(void);

	//! Should window close?
	bool shouldClose(void);

	//! Closes the window.
	void close(void);


	//! Set a rendering stencil.
	/*! Specify the area of the window you wish to draw to. (x, y) is the coordinate of the bottom left part
	of the rectangle you wish to draw to. Width and height define the rectangle that will be drawable with
	its lower left corner at that point.
	Implemented with glStencil.
	*/
	void setRenderMask(float x, float y, float width, float height);

	//! Removes render stencil.
	void removeRenderMask(void);

	//! Get the current drawable area, value depends on the current stencil
	void getSize(float& width, float& height);

	//! Make window fullscreen.
	void fullscreen(void);

	//! Clears render zone with the current glClearColor.
	void clear(void);

	//! Renders entire window, call once per frame.
	void render(void);

	//! Check if a key is down.
	bool key(int key);

	//! Check if a mouse button is pressed.
	bool mouseButton(int button);

	//! Check if mouse button was clicked.
	bool mouseClick(int button);

	//! Get the current mouse position.
	const glm::vec2& mousePos(void);

	//! Returns the pointer to the internal GLFW window.
	GLFWwindow* getGLWindow(void);
};


#endif // !WINDOW_H_