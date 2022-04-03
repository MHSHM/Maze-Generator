#pragma once

#include <Component.h>
#include <vector>
#include <unordered_map>

#define MAX_FRAMES 20

#define MAX_STATES 10

struct State 
{
	State():
		current_frame(0.0f),
		loop(true),
		speed(1.0f)
	{
		frames.reserve(MAX_FRAMES);
	}

	std::vector<class Texture*> frames;
	float current_frame;
	bool loop;
	float speed;
};

class Animation : public Component 
{
public:
	Animation(class Actor* _owner);
	void Update(float deltatime) override;
	void Clear() override;

	class State* Create_State(std::string&& state);
	void Set_State(std::string&& state);

public:
	State* current_state;

	std::unordered_map<std::string, State*> states_map;
	std::vector<State> states;
};
