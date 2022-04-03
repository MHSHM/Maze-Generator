#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glew.h>
#include <vector>

std::string Shader::Parse_Source(const std::string& path)
{
	std::stringstream source_code;
	std::ifstream shader_file(path);
	std::string line;

	if (!shader_file.is_open())
	{
		std::cerr << "failed to read shader with path: " << path << '\n';
		return "";
	}

	while (std::getline(shader_file, line))
	{
		source_code << line << '\n';
	}

	shader_file.close();

	return source_code.str();
}

unsigned int Shader::Compile(unsigned int type, const std::string source)
{
	unsigned int id = glCreateShader(type);

	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	int success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
		for (auto& c : errorLog) std::cout << c;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

Shader::Shader() :
	shader_program_id(0)
{
}

void Shader::Create_Shader_Program(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	std::string vertex_source = Parse_Source(vertex_shader_path);
	std::string fragment_source = Parse_Source(fragment_shader_path);

	unsigned int vertex_shader = Compile(GL_VERTEX_SHADER, vertex_source);
	unsigned int fragment_shader = Compile(GL_FRAGMENT_SHADER, fragment_source);

	shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertex_shader);
	glAttachShader(shader_program_id, fragment_shader);

	glLinkProgram(shader_program_id);
}


void Shader::Set_Matrix4_Uniform(std::string uniform, const glm::mat4& matrix)
{
	GLuint location = glGetUniformLocation(shader_program_id, uniform.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::Set_Vec3_Uniform(std::string uniform, glm::vec3 vector)
{
	GLuint location = glGetUniformLocation(shader_program_id, uniform.c_str());
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::Set_Float_Uniform(std::string uniform, float val)
{
	GLuint location = glGetUniformLocation(shader_program_id, uniform.c_str());
	glUniform1f(location, val);
}

void Shader::Set_Int_Uniform(std::string uniform, int val)
{
	GLuint location = glGetUniformLocation(shader_program_id, uniform.c_str());
	glUniform1i(location, val);
}

void Shader::Bind()
{
	glUseProgram(shader_program_id);
}

void Shader::Un_Bind()
{
	glUseProgram(0);
}

void Shader::Clear()
{
	glDeleteShader(shader_program_id);
}