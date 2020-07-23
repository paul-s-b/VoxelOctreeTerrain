#include "ShadingProgram.h"

ShadingProgram::ShadingProgram(std::string vp, std::string fp) {
	std::string shader;
	GLuint shader_id;
	const GLchar *source;

	_program = glCreateProgram();

	shader = _getShaderCode(vp);
	shader_id = glCreateShader(GL_VERTEX_SHADER);
	source = shader.c_str();
	glShaderSource(shader_id, 1, &source, nullptr);
	glCompileShader(shader_id);
	_checkCompilation(shader_id, vp);
	glAttachShader(_program, shader_id);

	shader = _getShaderCode(fp);
	shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	source = shader.c_str();
	glShaderSource(shader_id, 1, &source, nullptr);
	glCompileShader(shader_id);
	_checkCompilation(shader_id, fp);
	glAttachShader(_program, shader_id);
	glLinkProgram(_program);
	_checkLinking();
}

std::string	ShadingProgram::_getShaderCode(std::string filepath) {
	std::ifstream fileStream(filepath);

	if (fileStream.fail() || !fileStream.good())
	{
		std::cerr << "Cannot read shader file" << std::endl;
	}
	std::stringstream buf;
	buf << fileStream.rdbuf();
	return buf.str();
}

void ShadingProgram::_checkCompilation(GLuint id, std::string path) {
	GLint success = 0;
	GLint logsize;

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logsize);

		char *log = new char[logsize];

		glGetShaderInfoLog(id, logsize, nullptr, log);
		std::cerr << "Error compiling shader" << std::endl
			<< "src: \"" << path << '"' << std::endl
			<< log << std::endl << std::endl;
		delete[] log;
	}
}

void ShadingProgram::_checkLinking(void) {
	GLint success = 0;
	GLint logsize;

	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logsize);

		char *log = new char[logsize];

		glGetProgramInfoLog(_program, logsize, nullptr, log);
		std::cerr << "Error linking shader program:" << std::endl << log << std::endl;
		delete[] log;
	}
}

void ShadingProgram::use(void) {
	glUseProgram(_program);
}

GLuint ShadingProgram::ID(void) {
	return _program;
}
