#pragma once

#include <VertexArrayObject.h>

class Quad
{
public:
	Quad();
	Quad(float _width, float _height);

public:
	std::vector<glm::vec3> vertices; 
	std::vector<unsigned int> indices;
	float width;
	float height;
};