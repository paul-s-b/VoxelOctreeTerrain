#define _USE_MATH_DEFINES

#include "FreeCamera.hpp"
#include "Landscape.hpp"

FreeCamera::FreeCamera(Window& window, double yaw, double pitch) : _window(window)
{
	_data.position = glm::vec3(0, 0, 0);
	_data.direction = glm::vec3(0, 0, -1);
	_aspect = 1.0;
	_near = 0.1;
	_far = 1000000000.0;
	_fov = 80.0;
	_yaw = yaw;
	_pitch = pitch;
	
	updateView();
	_data.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	_data.VP = _data.projection * _data.view;
	_mouse_pos_old = _window.MousePos();
}

const Frustum& FreeCamera::getFrustum() const noexcept
{
	return _frustum;
}

void	FreeCamera::updateView(void)
{
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

void	FreeCamera::Update(double dt)
{
	bool moved = false;

	if (_aspect != _window.GetAspect())
	{
		_aspect = _window.GetAspect();
		_data.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	}
	float speed = 100.0f;
	if (_window.Key(' '))
		speed *= 10.0f;
	if (_window.Key('C'))
		speed *= 40.0f;
	if (_window.Key('W'))
	{
		_data.position += _data.direction * dt * speed;
		moved = true;
	}
	if (_window.Key('S'))
	{
		_data.position -= _data.direction * dt * speed;
		moved = true;
	}
	if (_window.Key('A'))
	{
		_data.position -= glm::cross(_data.direction, _up) * dt * speed;
		moved = true;
	}
	if (_window.Key('D'))
	{
		_data.position += glm::cross(_data.direction, _up) * dt * speed;
		moved = true;
	}
	if (_window.Key('Z'))
	{
		_data.position += _up * dt * speed;
		moved = true;
	}
	if (_window.Key('X'))
	{
		_data.position -= _up * dt * speed;
		moved = true;
	}

	//if (_window.MouseClick(GLFW_MOUSE_BUTTON_LEFT))
	//	//LandscapeManager::CastRay(_data);

	if (_window.Key('1'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (_window.Key('2'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (_window.MousePos() != _mouse_pos_old)
	{
		glm::vec2 mouse_delta = _mouse_pos_old - _window.MousePos();
		_yaw += mouse_delta.x * 1; //mouse movement speed
		_pitch -= mouse_delta.y * 1; //mouse movement speed

		_mouse_pos_old = _window.MousePos();
		moved = true;
	}

	if (moved)
	{
		updateView();
		_data.VP = _data.projection * _data.view;
	}
	_frustum.update(_data.VP);
}

const CameraData& FreeCamera::GetCameraData(void)
{
	return _data;
}
