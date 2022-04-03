#pragma once

#include <Component.h>

struct AABB
{

	AABB() = default;

	AABB(float _min_x, float _min_y, float _max_x, float _max_y):
		min_x(_min_x),
		min_y(_min_y),
		max_x(_max_x),
		max_y(_max_y) {}

	float min_x;
	float min_y;
	float max_x;
	float max_y;

	float min_x_offset = 0.0f; 
	float min_y_offset = 0.0f; 
	float max_x_offset = 0.0f; 
	float max_y_offset = 0.0f;
};


class Collider : public Component 
{
public:
	Collider(class Actor* _owner);
	void Update(float deltatime) override;
	void Clear() override; 

public:
	AABB aabb;
};

bool Is_Colliding(const Collider& c1, const Collider& c2);
