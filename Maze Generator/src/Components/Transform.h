#pragma once

#include <Component.h>

#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtx/transform.hpp>

class Transform : public Component 
{
public:
	Transform(class Actor* _owner);
	void Update(float deltatime) override;
	void Clear() override; 


public:
	glm::vec3 translation; 
	float scale_x;
	float scale_y;

	glm::mat4 translation_mat; 
	glm::mat4 scale_mat; 
	glm::mat4 world_transform;
};
