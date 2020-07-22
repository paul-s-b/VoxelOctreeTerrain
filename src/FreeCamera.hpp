#ifndef FREECAMERA_H_
#define FREECAMERA_H_

#include "Window.hpp"
#include "Frustum.h"

struct	CameraData {
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 VP;
	glm::vec3 position;
	glm::vec3 direction;
};

class	FreeCamera {
	static glm::vec3 const _forward;

	Window& _window;

	glm::vec2 _mouse_pos_old;
	float _aspect;
	float _near;
	float _far;
	float _fov;
	float _yaw;
	float _pitch;
	glm::vec3 _up;
	Frustum _frustum;
	CameraData _data;

	void _updateView(void);

public:
	const Frustum& getFrustum() const noexcept;
	FreeCamera(Window& window, double yaw = 0.0, double pitch = 0.0);
	void update(double dt);
	const CameraData& getCameraData(void);
};

#endif // FREECAMERA_H_