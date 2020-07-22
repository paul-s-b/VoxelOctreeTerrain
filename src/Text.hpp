#ifndef TEXT_H_
#define TEXT_H_

#include "ShadingProgram.hpp"
#include "Texture.hpp"
#include <glm.hpp>

class Text {
private:
	static constexpr const char* _fontFile = "assets/textures/game_font.png";
	static constexpr const char* _vertexPath = "src/shaders/text.vert";
	static constexpr const char* _fragPath = "src/shaders/text.frag";

	static ShadingProgram* _program;

	std::string _message;

	static std::vector<float> _square;
	static std::vector<float> _uv;

	static GLuint _squareID;
	static GLuint _UVID;
	static GLuint _textureID;
	static GLuint _textureLocationID;
	static GLuint _VAO;

	static bool _init;

	void _renderChar(char c, glm::vec2 topleft, glm::vec2 botright);

public:

	Text(std::string message);

	void render(float aspect);
};

#endif // TEXT_H_