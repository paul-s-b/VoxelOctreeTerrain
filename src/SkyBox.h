#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "FreeCamera.h"
#include "Texture.h"
#include "ShadingProgram.h"
#include <glm.hpp>
#include <ext.hpp>

class SkyBox {
	static constexpr const char* _vertexPath = "src/shaders/skybox.vert";
	static constexpr const char* _fragPath = "src/shaders/skybox.frag";

	static const std::vector<float> _vertexArray;

	ShadingProgram* _program;
	GLuint _vertexArrayID;
	GLuint _textureID;
	GLuint _textureLocationID;
	GLuint _projectionID;
	GLuint _VAO;

	void _loadArrayBuffers();
	void _loadTextures(const std::vector<Texture*>&);
	void _makeVAO();

public:
	SkyBox(std::string right, std::string left, std::string top,
		std::string bot, std::string back, std::string front);
	~SkyBox(void);
	void render(const CameraData& cam_data);
};


#endif // SKYBOX_H_