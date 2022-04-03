#pragma once

#include <Component.h>
#include <Quad.h>

class Render : public Component 
{
public:
	Render(class Actor* _owner);
	void Update(float deltatime) override;
	void Clear() override;

public:
	glm::vec3 color;
	class Texture* texture;
};