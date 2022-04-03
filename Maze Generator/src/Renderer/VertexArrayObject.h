#pragma once

#include <vector>


#include <vec2.hpp>
#include <vec3.hpp>

class VertexArrayObject
{
public:

	VertexArrayObject() = default;

	void Send_Data_To_Opengl(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);

	void Bind() const;
	void Un_Bind() const;

	unsigned int Get_Element_Buffer_Size() { return element_buffer_size; }

	void Clear();

public:
	unsigned int vertex_array_obj_id;
	unsigned int array_buffer_id;
	unsigned int element_buffer_id;

	unsigned int element_buffer_size;
};