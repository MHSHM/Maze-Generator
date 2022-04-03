#pragma once

#include <cinttypes>
#include <Component.h>

class Input : public Component 
{
public:
	Input(class Actor* _owner); 
	void Update(float deltatime) override;

public:
	uint16_t move_right;
	uint16_t move_left;
	uint16_t move_up;
	uint16_t move_down;

	float x_step = 150.0f;
	float y_step = 150.0f;

	bool take_input;

};