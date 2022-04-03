#pragma once

#include <Collider.h>
#include <Game.h>
#include <Actor.h>
#include <Transform.h>
#include <Render.h>

#include <iostream>

Collider::Collider(Actor* _owner) :
	Component(_owner),
	aabb(0.0f, 0.0f, 0.0f, 0.0f)
{
}

void Collider::Update(float deltatime)
{
	/*
	Transform* transform = owner->Get_Component<Transform>();
	Render* render = owner->Get_Component<Render>(); 

	if (!transform || !render) 
	{
		return;
	}

	aabb.min_x = transform->translation.x + aabb.min_x_offset;
	aabb.min_y = transform->translation.y + aabb.min_y_offset;
	aabb.max_x = ((render->quad.width * transform->scale_x) + transform->translation.x) + aabb.max_x_offset;
	aabb.max_y = ((render->quad.height * transform->scale_y) + transform->translation.y) + aabb.max_y_offset;
	*/
}

void Collider::Clear()
{
}

bool Is_Colliding(const Collider& c1, const Collider& c2)
{
	bool c1_right_c2 = c1.aabb.min_x > c2.aabb.max_x;
	bool c1_left_c2  = c1.aabb.max_x < c2.aabb.min_x;
	bool c1_above_c2 = c1.aabb.min_y > c2.aabb.max_y;
	bool c1_under_c2 = c1.aabb.max_y < c2.aabb.min_y;

	if (c1_right_c2 | c1_left_c2 | c1_above_c2 | c1_under_c2) 
	{
		return false;
	}

	return true;
}