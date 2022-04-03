#pragma once

#include <mat4x4.hpp>
#include <gtx/transform.hpp>

#include <Texture.h>
#include <VertexArrayObject.h>

struct DataPerInstance 
{
	std::vector<glm::vec3> translations; 
	std::vector<glm::vec3> colors;
};

// for instance rendering
struct InstanceRenderData
{
	unsigned int vao; 
	unsigned int vbo;
	unsigned int ibo;
	unsigned int translation_buffer;
	unsigned int color_buffer;

	DataPerInstance data_per_instance;
};


struct ShaderData 
{
	glm::mat4x4 ortho_proj;
};

class Renderer
{
public:
	Renderer(class Game* _game);
	void Initialize();
	void Update_Instances_Color(const std::vector<glm::vec3>& new_colors);
	void Draw_Instances();
	void Draw(); 
public:
	class Game* game;
	InstanceRenderData renderer_data;
	ShaderData shader_data;

	~Renderer();

};