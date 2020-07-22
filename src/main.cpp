#include "Chunk.hpp"
#include "Window.hpp"
#include "FreeCamera.hpp"
#include "FPSDisplay.hpp"
#include "Time.hpp"
#include "Landscape.hpp"
#include "SkyBox.hpp"

int	main(void)
{
	GLenum err;
	
	Window window(1920, 1080, "Landscape");
	glClearColor(0.2, 0.25, 0.3, 1);
	CLW::init();

	LandscapeManager landscape;
	FPSDisplay fps;
	FreeCamera cam(window);
	Time clock;
	SkyBox sky(
		"assets/textures/skybox/right.png",
		"assets/textures/skybox/left.png",
		"assets/textures/skybox/top.png",
		"assets/textures/skybox/bottom.png",
		"assets/textures/skybox/front.png",
		"assets/textures/skybox/back.png"
	);
	
	Chunk::init();

	while (!window.shouldClose())
	{
		//std::cout << cam.GetCameraData().position.x << " " << cam.GetCameraData().position.y << " " << cam.GetCameraData().position.z << "\n";

		if ((err = glGetError()) != GL_NO_ERROR)
			std::cerr << err << std::endl;
		clock.step();
		window.clear();
		cam.update(clock.delta());
		landscape.render(cam.getCameraData(), cam);
		sky.render(cam.getCameraData());
		fps.render(window);
		window.render();
		if (window.key(GLFW_KEY_ESCAPE))
			break;
	}
	window.close();
}
