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
	CLW::Init();

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
	
	Chunk::Init();

	while (!window.ShouldClose())
	{
		//std::cout << cam.GetCameraData().position.x << " " << cam.GetCameraData().position.y << " " << cam.GetCameraData().position.z << "\n";

		if ((err = glGetError()) != GL_NO_ERROR)
			std::cerr << err << std::endl;
		clock.Step();
		window.Clear();
		cam.Update(clock.Delta());
		landscape.Render(cam.GetCameraData(), cam);
		sky.Render(cam.GetCameraData());
		fps.Render(window);
		window.Render();
		if (window.Key(GLFW_KEY_ESCAPE))
			break;
	}
	window.Close();
}
