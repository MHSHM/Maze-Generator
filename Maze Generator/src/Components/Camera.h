#pragma once

#include <Component.h>

#include <mat4x4.hpp>

class Camera : public Component 
{
public:
	Camera(class Actor* _owner);
	void Update(float deltatime) override;
	void Clear() override; 

public:
	glm::mat4 view; 
	glm::mat4 ortho_proj;
};