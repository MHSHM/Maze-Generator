#pragma once

#include <vector>
#include <string>

class Texture
{
public:

	bool Init(const std::string& path);

	void Bind(int id);
	void Clear();


	void Un_Bind();

	int width = 0;
	int height = 0;

	// how many entites are using this texture
	int counter = 1;

	unsigned int texture_id;
};