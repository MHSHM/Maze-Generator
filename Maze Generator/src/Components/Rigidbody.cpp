#pragma once

#include <Rigidbody.h>
#include <Actor.h>
#include <Transform.h>

#include <iostream>

Rigidbody::Rigidbody(Actor* _owner) :
	Component(_owner),
	mass(0.3f),
	velocity(0.0f),
	max_velocity_x(200.0f),
	max_velocity_y(300.0f)
{
	forces.reserve(FORCES);
}

void Rigidbody::Update(float deltatime)
{
	Transform* transform = owner->Get_Component<Transform>();

	if (transform == nullptr)
	{
		return;
	}
	
	prev_velocity = velocity;

	glm::vec2 total_force = glm::vec2(0.0f);

	for (auto& force : forces) 
	{
		glm::vec2 f = force.first * force.second;
		total_force += f;
	}

	glm::vec2 acceleration = total_force * (1.0f / mass);

	velocity += acceleration * deltatime;
	velocity.x = std::min(velocity.x, max_velocity_x);
	velocity.x = std::max(velocity.x, -max_velocity_x);

	velocity.y = std::min(velocity.y, max_velocity_y);

	transform->translation += glm::vec3(velocity * deltatime, 0.0f);

	forces.clear();
}

void Rigidbody::Clear()
{
}

void Rigidbody::Add_Force(const glm::vec2& direction, float magnitude)
{
	forces.push_back({direction, magnitude});
}
