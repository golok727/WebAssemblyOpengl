#include "renderer.h"
#include "shader.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource)
{

	m_Src = ShaderSource(vertexShaderSource, fragmentShaderSource);
	makeProgram();
}

Shader::~Shader()
{
	glDeleteProgram(m_ProgramId);
}

Shader *Shader::LoadFromFiles(const char *vertexShaderFile, const char *fragmentShaderFile)
{
	std::string vsSrc = readShaderFile(vertexShaderFile);
	std::string fsSrc = readShaderFile(fragmentShaderFile);

	return new Shader(vsSrc.c_str(), fsSrc.c_str());
}

Shader Shader::LoadFromFile(const char *filepath)
{
	std::string shaderContent = readShaderFile(filepath);

	auto ss = parseSpecialShader(shaderContent);
	std::string vsSrc = ss[0].str();
	std::string fsSrc = ss[1].str();
	return Shader(vsSrc.c_str(), fsSrc.c_str());
}

void Shader::setUniform1f(const char *name, float x)
{
	int location = getUniformLocation(name);
	glUniform1f(location, x);
}

void Shader::setUniform2f(const char *name, float x, float y)
{
	int location = getUniformLocation(name);
	glUniform2f(location, x, y);
}

void Shader::setUniform3f(const char *name, float x, float y, float z)
{
	int location = getUniformLocation(name);
	glUniform3f(location, x, y, z);
}

void Shader::setUniform4f(const char *name, float x, float y, float z, float w)
{
	int location = getUniformLocation(name);
	glUniform4f(location, x, y, z, w);
}

void Shader::setUniformMat4f(const char *name, const glm::mat4 &matrix)
{
	int location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::use()
{
	glUseProgram(m_ProgramId);
}

void Shader::detach()
{
	(glUseProgram(0));
}

void Shader::makeProgram()
{

	unsigned int vs = createShader(GL_VERTEX_SHADER, m_Src.vertex);
	unsigned int fs = createShader(GL_FRAGMENT_SHADER, m_Src.fragment);

	unsigned int program = glCreateProgram();

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	checkProgramLinkStatus(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	m_ProgramId = program;
}

unsigned int Shader::createShader(unsigned int type, const char *src)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	compileShader(type, shader);

	return shader;
}

void Shader::compileShader(unsigned int type, unsigned int shader)
{
	glCompileShader(shader);
	checkShaderCompilation(shader, (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT");
}

int Shader::getUniformLocation(const char *name, int warnLevel)
{

	// Check cache;
	auto uniformCache = m_UniformCache.find(name);
	if (uniformCache != m_UniformCache.end())
		return uniformCache->second;

	int location = glGetUniformLocation(m_ProgramId, name);
	if (location < 0 && warnLevel > 0)
	{
		switch (warnLevel)
		{
		case 1:
			std::cout << "Uniform '" << name << "'"
								<< " is not set";
			break;
		case 2:
			std::cout << "Uniform '" << name << "'"
								<< " is not set";
			break;
		}
		return location;
	}

	m_UniformCache.insert({name, location});

	return location;
}

std::array<std::stringstream, 2> Shader::parseSpecialShader(const std::string &content)
{
	std::array<std::stringstream, 2> ss;

	std::stringstream stream(content);
	std::string line;

	ShaderType currentType = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		if (line.find("#type") != std::string::npos)
		{
			if (line.find("#type vertex") != std::string::npos)
				currentType = ShaderType::VERTEX;
			else if (line.find("#type fragment") != std::string::npos)
				currentType = ShaderType::FRAGMENT;
			else
			{
				std::cout << "Unspecified shader type..." << std::endl;
				currentType = ShaderType::NONE;
			}
			continue;
		}

		if (currentType != ShaderType::NONE)
			ss[static_cast<int>(currentType)] << line << "\n";
	}
	return ss;
}

std::string Shader::readShaderFile(const char *filepath)
{

	std::ifstream shaderFile(filepath);
	if (!shaderFile.is_open())
	{
		std::cout << "Cannot open file: " << filepath << " :(\n";
		return "";
	}

	std::string line;

	std::stringstream ss;
	while (std::getline(shaderFile, line))
		ss << line << "\n";

	return ss.str();
}

// Error handling

void Shader::checkShaderCompilation(unsigned int shader, const char *label)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		int infoLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		char *message = (char *)alloca(infoLen * sizeof(char));

		glGetShaderInfoLog(shader, infoLen, &infoLen, message);

		std::cerr << "Shader " << label << ": Failed!\n";
		std::cerr << message << std::endl;

		return;
	}
	std::cout << "Shader " << label << ": Compiled!\n";
}

void Shader::checkProgramLinkStatus(unsigned int program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int infoLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		char *message = (char *)alloca(sizeof(char) * infoLen);
		glGetProgramInfoLog(program, infoLen, &infoLen, message);

		std::cerr << "Error linking program\n";
		std::cout << message << std::endl;
		return;
	}
	std::cout << "Program linked successfully\n";
}
