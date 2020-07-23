#define _USE_MATH_DEFINES

#include "FreeCamera.h"
#include "Landscape.h"

FreeCamera::FreeCamera(Window& window, double yaw, double pitch) : _window(window) {
	_data.position = glm::vec3(0, 0, 0);
	_data.direction = glm::vec3(0, 0, -1);
	_aspect = 1.0;
	_near = 0.1;
	_far = 1000000000.0;
	_fov = 80.0;
	_yaw = yaw;
	_pitch = pitch;
	
	_updateView();
	_data.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	_data.VP = _data.projection * _data.view;
	_mouse_pos_old = _window.mousePos();
}

const Frustum& FreeCamera::getFrustum() const noexcept {
	return _frustum;
}

void FreeCamera::_updateView(void) {
	bool constrainPitch = true;
	if (constrainPitch)
	{
		if (_pitch > M_PI / 2.0)
			_pitch = M_PI / 2.0;
		if (_pitch < -M_PI / 2.0)
			_pitch = -M_PI / 2.0;
	}

	if (_yaw > 2 * M_PI)
		_yaw = 0;
	if (_yaw < -2 * M_PI)
		_yaw = 0; 

	_data.direction = glm::vec3(
		cos(_pitch) * sin(_yaw),
		sin(_pitch),
		cos(_pitch) * cos(_yaw)
	);
	_up = glm::cross(
		glm::vec3(
			sin(_yaw - M_PI / 2.0),
			0.0,
			cos(_yaw - M_PI / 2.0)
		),
		_data.direction
	);
	_data.view = glm::lookAt(
		_data.position,
		_data.position + _data.direction,
		_up
	);
}

void FreeCamera::update(double dt) {
	bool moved = false;

	if (_aspect != _window.getAspect())
	{
		_aspect = _window.getAspect();
		_data.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	}
	float speed = 100.0f;
	if (_window.key(' '))
		speed *= 10.0f;
	if (_window.key('C'))
		speed *= 40.0f;
	if (_window.key('W'))
	{
		_data.position += _data.direction * dt * speed;
		moved = true;
	}
	if (_window.key('S'))
	{
		_data.position -= _data.direction * dt * speed;
		moved = true;
	}
	if (_window.key('A'))
	{
		_data.position -= glm::cross(_data.direction, _up) * dt * speed;
		moved = true;
	}
	if (_window.key('D'))
	{
		_data.position += glm::cross(_data.direction, _up) * dt * speed;
		moved = true;
	}
	if (_window.key('Z'))
	{
		_data.position += _up * dt * speed;
		moved = true;
	}
	if (_window.key('X'))
	{
		_data.position -= _up * dt * speed;
		moved = true;
	}

	//if (_window.MouseClick(GLFW_MOUSE_BUTTON_LEFT))
	//	//LandscapeManager::CastRay(_data);

	if (_window.key('1'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (_window.key('2'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (_window.mousePos() != _mouse_pos_old)
	{
		glm::vec2 mouse_delta = _mouse_pos_old - _window.mousePos();
		_yaw += mouse_delta.x * 1; //mouse movement speed
		_pitch -= mouse_delta.y * 1; //mouse movement speed

		_mouse_pos_old = _window.mousePos();
		moved = true;
	}

	if (moved)
	{
		_updateView();
		_data.VP = _data.projection * _data.view;
	}
	_frustum.update(_data.VP);
}

const CameraData& FreeCamera::getCameraData(void) {
	return _data;
}
