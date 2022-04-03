#include "Quad.h"

#include <vector>


Quad::Quad() :
	width(0.0f),
	height(0.0f) 
{
}

Quad::Quad(float _width, float _height) :
	width(_width),
	height(_height)
{
	vertices.resize(4);
	vertices[0] = { 0.0f, height, 0.0f };
	vertices[1] = { width, height, 0.0f };
	vertices[2] = { width, 0.0f, 0.0f };
	vertices[3] = { 0.0f,  0.0f, 0.0f };

	indices.resize(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;
}