#pragma once

#include <string>

#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>


class Shader
{
public:
	Shader();


	void Create_Shader_Program(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

	unsigned int& Get_Shader_program() { return shader_program_id; }

	void Set_Matrix4_Uniform(std::string uniform, const glm::mat4& matrix);
	void Set_Vec3_Uniform(std::string uniform, glm::vec3 vector);
	void Set_Float_Uniform(std::string uniform, float val);
	void Set_Int_Uniform(std::string uniform, int val);

	void Bind();
	void Un_Bind();

	void Clear();

private:
	std::string Parse_Source(const std::string& path);
	unsigned int Compile(unsigned int type, const std::string source);

private:
	unsigned int shader_program_id;
};