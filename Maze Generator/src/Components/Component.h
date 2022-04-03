#pragma once

enum class ComponentType
{
	Transform = 0,
	Render = 1,
	NodeState = 2
};

class Component
{
public:

	Component(class Actor* _owner);

	virtual void Update(float deltatime);
	virtual void Clear(); 

public:
	class Actor* owner;

	virtual ~Component();

};