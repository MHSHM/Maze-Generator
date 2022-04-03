#pragma once

#include <Camera.h>
#include <Game.h>


#include <iostream>

Camera::Camera(Actor* _owner) :
	Component(_owner),
	view(glm::mat4(1.0f)),
	ortho_proj(glm::mat4(1.0f))
{
	ortho_proj = glm::ortho(0.0f, float(WINDOW_WIDTH), 0.0f, float(WINDOW_HEIGHT), 0.0f, 10.0f);
}

void Camera::Update(float deltatime)
{
	Transform* transform = owner->Get_Component<Transform>();

	if (transform == nullptr) 
	{
		return;
	}

	view = glm::translate(glm::mat4(1.0f), transform->translation * glm::vec3(-1.0f, 0.0f, 1.0f));
}

void Camera::Clear()
{
}
