#pragma once

#include <Component.h>

#include <vector>

#define FORCES 20

#include <vec2.hpp>

class Rigidbody : public Component
{
public:
	Rigidbody(class Actor* _owner);
	void Update(float deltatime) override;
	void Clear() override;

	void Add_Force(const glm::vec2& direction, float magnitude);

public:
	float mass; 

	glm::vec2 velocity;
	glm::vec2 prev_velocity;
	float max_velocity_x;
	float max_velocity_y;

	std::vector<std::pair<glm::vec2, float> > forces;

};