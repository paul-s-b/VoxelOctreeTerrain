#include "Window.hpp"

Window::Window(int width, int height, std::string name) :
	_screenCornerX(0),
	_screenCornerY(0),
	_width(1),
	_height(1),
	_windowWidth(width),
	_windowHeight(height) {

	if (glfwInit() == GLFW_FALSE)
	{
		throw std::runtime_error("Failed to initialize GLFW");
		exit(EXIT_FAILURE);
	}
	windowHints();
	glfwSetErrorCallback(errorCallback);
	_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	if (!_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize window");
	}
	glfwFocusWindow(_window);
	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, windowResizeCallback);
	glfwSetWindowPosCallback(_window, windowMoveCallback);
	glfwSetKeyCallback(_window, keyCallback);
	glfwSetMouseButtonCallback(_window, mouseButtonCallback);
	glfwSetCursorPosCallback(_window, mousePositionCallback);
	glfwMakeContextCurrent(_window);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
	glfwSwapInterval(0);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Fullscreen();
	if (glewInit() == GLFW_TRUE);
}

void Window::windowHints(void) {
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
}

void Window::getWindowSize(int &width, int &height) {
	width = _windowWidth;
	height = _windowHeight;
}

void Window::getMaxRenderSize(float &width, float &height) {
	int iwidth, iheight;

	glfwGetFramebufferSize(_window, &iwidth, &iheight);

	width = static_cast<float>(iwidth);
	height = static_cast<float>(iheight);
}

float Window::getAspect(void) {
	float width, height;
	getSize(width, height);
	return width / height;
}

bool Window::shouldClose(void) {
	return glfwWindowShouldClose(_window);
}

void Window::close(void) {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::getSize(float &width, float &height) {
	float actualWidth, actualHeight;

	getMaxRenderSize(actualWidth, actualHeight);

	width = _width * actualWidth;
	height = _height * actualHeight;
}

void Window::fullscreen() {
	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwSetWindowMonitor(
		_window,
		glfwGetPrimaryMonitor(),
		0, 0, mode->width, mode->height, mode->refreshRate
	);
}

void Window::setRenderMask(float x, float y, float width, float height) {
	float windowWidth, windowHeight;

	_width = width;
	_height = height;
	_screenCornerX = x;
	_screenCornerY = y;

	getMaxRenderSize(windowWidth, windowHeight);
	glEnable(GL_SCISSOR_TEST);
	glViewport(windowWidth * x,
		   windowHeight * y,
		   windowWidth * width,
		   windowHeight * height);
	glScissor(windowWidth * x,
		  windowHeight * y,
		  windowWidth * width,
		  windowHeight * height);
}

void Window::removeRenderMask(void) {
	_screenCornerX = 0;
	_screenCornerY = 0;
	_width = 1;
	_height = 1;

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
	glDisable(GL_SCISSOR_TEST);
}

void Window::render(void) {

	for (bool &status : _mouseButtonClicked)
		status = false;

	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void Window::clear(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::refreshRenderMask(void) {
	setRenderMask(_screenCornerX, _screenCornerY, _width, _height);
}

void windowResizeCallback(GLFWwindow *glfwWindow, int width, int height) {
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->_windowWidth = width;
	window->_windowHeight = height;
	window->refreshRenderMask();
}

void Window::windowMoveCallback(GLFWwindow *glfwWindow, int, int) {
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

	window->refreshRenderMask();
}

void Window::errorCallback(int, const char *description) {
	std::cerr << description << std::endl;
}

GLFWwindow* Window::getGLWindow(void) {
	return _window;
}

void keyCallback(GLFWwindow *glfwWindow, int key, int, int action, int) {
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

	if (action == GLFW_PRESS)
	{
		window->_keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
		window->_keys[key] = false;
}

bool Window::key(int key) {
	if (key >= 0 && key < 512)
		return _keys[key];
	else
		return false;
}

void mouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int) {
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

	if (action == GLFW_PRESS)
	{
		window->_mouseButtons[button] = true;
		window->_mouseButtonClicked[button] = true;
	}
	else if (action == GLFW_RELEASE)
		window->_mouseButtons[button] = false;
}

bool Window::mouseButton(int button) {
	if (button >= 0 && button < 8)
		return _mouseButtons[button];
	else
		return false;
}

bool Window::mouseClick(int button) {
	if (button >= 0 && button < 8)
		return _mouseButtonClicked[button];
	else
		return false;
}

void mousePositionCallback(GLFWwindow *glfwWindow, double x, double y) {
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	int width, height;
	window->getWindowSize(width, height);
	window->_mousePosition.x = (x * 2 / width) - 1;
	window->_mousePosition.y = (y * -2 / height) + 1;
}

const glm::vec2& Window::mousePos(void) {
	return const_cast<const glm::vec2&>(_mousePosition);
}
