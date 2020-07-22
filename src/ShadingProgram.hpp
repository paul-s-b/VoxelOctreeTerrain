#ifndef SHADINGPROGRAM_H_
#define SHADINGPROGRAM_H_

#include <glew.h>
#include <glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShadingProgram {
private:

	GLuint _program;

	std::string	_getShaderCode(std::string filepath);
	void _checkCompilation(GLuint, std::string filepath);
	void _checkLinking(void);

public:
	ShadingProgram(std::string vertexPath, std::string fragPath);
	void use(void);
	GLuint ID(void);
};


#endif // SHADINGPROGRAM_H_


