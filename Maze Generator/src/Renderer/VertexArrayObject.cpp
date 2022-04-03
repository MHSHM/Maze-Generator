
#include "VertexArrayObject.h"
#include <Renderer.h>

#include <glew.h>

void VertexArrayObject::Send_Data_To_Opengl(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices)
{
	element_buffer_size = indices.size();

	glGenVertexArrays(1, &vertex_array_obj_id);
	glBindVertexArray(vertex_array_obj_id);

	glGenBuffers(1, &array_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, array_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), (void*)&vertices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glGenBuffers(1, &element_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), (void*)&indices[0], GL_STATIC_DRAW);
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(vertex_array_obj_id);
}

void VertexArrayObject::Un_Bind() const
{
	glBindVertexArray(0);
}

void VertexArrayObject::Clear()
{
	glDeleteVertexArrays(1, &vertex_array_obj_id);
	glDeleteBuffers(1, &array_buffer_id);
	glDeleteBuffers(1, &element_buffer_id);
}