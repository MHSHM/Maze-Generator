#include <Renderer.h>
#include <Game.h>
#include <glew.h>

#include <vec3.hpp>

static Quad quad(TILE_WIDTH, TILE_HEIGHT);

Renderer::Renderer(Game* _game) :
	game(_game)
{
	shader_data.ortho_proj = glm::ortho(0.0f, float(WINDOW_WIDTH), 0.0f, float(WINDOW_HEIGHT), 0.0f, 10.0f);
}

void Renderer::Initialize()
{
	uint32_t instance_count = game->grid_scene.row_size * game->grid_scene.col_size;

	glGenVertexArrays(1, &renderer_data.vao);
	glBindVertexArray(renderer_data.vao);

	glGenBuffers(1, &renderer_data.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_data.vbo);
	glBufferData(GL_ARRAY_BUFFER, quad.vertices.size() * sizeof(glm::vec3), (void*)&quad.vertices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glGenBuffers(1, &renderer_data.translation_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_data.translation_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instance_count, (void*)&renderer_data.data_per_instance.translations[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &renderer_data.color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_data.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instance_count, (void*)&renderer_data.data_per_instance.colors[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &renderer_data.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_data.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.indices.size() * sizeof(unsigned int), (void*)&quad.indices[0], GL_STATIC_DRAW);
}

void Renderer::Update_Instances_Color(const std::vector<glm::vec3>& new_colors)
{
	if (!new_colors.empty()) 
	{
		glBindVertexArray(renderer_data.vao);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_data.color_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, new_colors.size() * sizeof(glm::vec3), (const void*)&new_colors[0]);
		glBindVertexArray(0);
	}
}


void Renderer::Draw_Instances()
{
	uint32_t instance_count = game->grid_scene.row_size * game->grid_scene.col_size;

	// Bind Shader
	game->shaders_table["shader"].Bind();
	game->shaders_table["shader"].Set_Matrix4_Uniform("u_orthoproj", game->renderer.shader_data.ortho_proj); 

	glBindVertexArray(renderer_data.vao); 

	glDrawElementsInstanced(GL_TRIANGLES, quad.indices.size(), GL_UNSIGNED_INT, (void*)quad.indices[0],
		instance_count);

	glBindVertexArray(0); 
	game->shaders_table["shader"].Un_Bind();
}

void Renderer::Draw()
{
}

Renderer::~Renderer() 
{

}