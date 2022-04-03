#include <Render.h>
#include <iostream>
#include <Texture.h>
#include <Game.h>

Render::Render(Actor* _owner) :
	Component(_owner),
	texture(nullptr),
	color(0.0f, 1.0f, 0.0f)
{
}


void Render::Update(float deltatime)
{
}

void Render::Clear()
{
}
