#pragma once

#include <Animation.h>
#include <Texture.h>
#include <Render.h>
#include <Game.h>
#include <Actor.h>

#include <iostream>

Animation::Animation(Actor* _owner) :
	Component(_owner),
	current_state(nullptr)
{
	states.reserve(MAX_STATES);
}

void Animation::Update(float deltatime)
{
	Render* render = owner->Get_Component<Render>();
	
	if (current_state->loop) 
	{
		if (current_state->current_frame >= current_state->frames.size()) 
		{
			current_state->current_frame = 0.0f;
			return;
		}

		render->texture = current_state->frames[(int)current_state->current_frame];
		current_state->current_frame += deltatime * current_state->speed;
	}
	else 
	{
		if (current_state->current_frame >= current_state->frames.size()) 
		{
			current_state->current_frame = current_state->frames.size() - 1;
			return;
		}

		render->texture = current_state->frames[(int)current_state->current_frame];
		current_state->current_frame += deltatime * current_state->speed;
	}
}

State* Animation::Create_State(std::string&& state)
{
	states.push_back(State());
	states_map[state] = &states.back();
	return &states.back();
}

void Animation::Set_State(std::string&& state)
{
	std::string test = state;

 	current_state = states_map[state];

	if (current_state->current_frame > 0.0f && current_state->loop == false) 
	{
		current_state->current_frame = 0.0f;
	}
}

void Animation::Clear()
{
	for (auto& state : states)
	{
		for (auto& frame : state.frames)
		{
			owner->scene_node->scene->game->Remove_Texture(frame);
		}
		state.frames.clear();
	}

	states.clear();
	states_map.clear();
	current_state = nullptr; 
}
