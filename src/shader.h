#pragma once
#include <string>
#include <sstream>
#include <array>
#include <unordered_map>
#include <glm/glm.hpp>
struct ShaderSource
{
	const char *vertex;
	const char *fragment;
	ShaderSource() : vertex(""), fragment("") {}
	ShaderSource(const char *vertexSource, const char *fragmentSource) : vertex(vertexSource), fragment(fragmentSource) {}
};

class Shader
{

public:
	Shader() : m_Src(ShaderSource("", "")), m_ProgramId(0) {}
	Shader(const char *vertexShaderSource, const char *fragmentShaderSource);
	~Shader();
	static Shader *LoadFromFiles(const char *vertexShaderFile, const char *fragmentShaderFile);
	static Shader LoadFromFile(const char *filepath);
	void setUniform1f(const char *name, float x);
	void setUniform2f(const char *name, float x, float y);
	void setUniform3f(const char *name, float x, float y, float z);
	void setUniform4f(const char *name, float x, float y, float z, float w);
	void setUniformMat4f(const char *name, const glm::mat4 &matrix);
	inline ShaderSource getSrc() { return m_Src; }
	void use();
	void detach();

private:
	static std::string readShaderFile(const char *filepath);
	void makeProgram();
	void checkProgramLinkStatus(unsigned int program);
	void checkShaderCompilation(unsigned int shader, const char *label);
	void compileShader(unsigned int type, unsigned int shader);
	int getUniformLocation(const char *name, int warnLevel = 0);
	static std::array<std::stringstream, 2> parseSpecialShader(const std::string &content);
	unsigned int createShader(unsigned int type, const char *src);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX,
		FRAGMENT
	};

private:
	ShaderSource m_Src;
	std::unordered_map<const char *, int> m_UniformCache;
	unsigned int m_ProgramId;
};