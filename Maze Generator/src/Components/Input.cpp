#include <Input.h>
#include <Actor.h>
#include <Game.h>

#include <glfw3.h>
#include <iostream>

Input::Input(Actor* _owner) :
	Component(_owner),
	move_right(GLFW_KEY_RIGHT),
	move_left(GLFW_KEY_LEFT),
	move_up(GLFW_KEY_UP),
	move_down(GLFW_KEY_DOWN),
	take_input(true)
{
	
}

void Input::Update(float deltatime)
{
}
