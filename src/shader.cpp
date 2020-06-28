#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glew.h>


Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}


unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	unsigned int shader_id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader_id, 1, &src, nullptr);
	glCompileShader(shader_id);
	
	switch (type)
	{
	case GL_VERTEX_SHADER:
		if (IsCompileError(shader_id, "VERTEX"))
			return 0;

	case GL_FRAGMENT_SHADER:
		if (IsCompileError(shader_id, "FRAGMENT"))
			return 0;
	}

	return shader_id;
}



Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	
	// ensure file stream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		std::stringstream vShaderStream, fShaderStream;
	
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::UNABLE TO READ FILE" << std::endl;
	}

	unsigned int vertexShaderId = CompileShader(vertexCode, GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = CompileShader(fragmentCode, GL_FRAGMENT_SHADER);

	// Shader Program
	m_ShaderID = glCreateProgram();
	glAttachShader(m_ShaderID, vertexShaderId);
	glAttachShader(m_ShaderID, fragmentShaderId);
	glLinkProgram(m_ShaderID);
	IsCompileError(m_ShaderID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}


bool Shader::IsCompileError(unsigned int component_id, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(component_id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(component_id, 1024, NULL, infoLog);
			std::cout << "ERROR::FAILED TO COMPILE: " << type << " SHADER" <<"\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			glDeleteShader(component_id);
			return true;
		}
	}
	else
	{
		glGetProgramiv(component_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(component_id, 1024, NULL, infoLog);
			std::cout << "ERROR::FAILED TO LINK: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			glDeleteProgram(component_id);
			return true;
		}
	}
	return false;
}


void Shader::use()
{
	glUseProgram(m_ShaderID);
}


void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), (int)value);
}


void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), value);
}


void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ShaderID, name.c_str()), value);
}


void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y);
}


void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y, z);
}


void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y, z, w);
}


void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}