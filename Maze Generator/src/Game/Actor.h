#pragma once

#define MAX_COMPONENTS 10

#include <vector>
#include <string>


#include <vec3.hpp>
#include <mat4x4.hpp>

class Actor
{
public:
	Actor(class SceneNode* _scene_node);

public:
	inline bool operator==(const Actor& other)
	{
		return (this == &other);
	}

	template<class T>
	T* Get_Component();

public:

	std::vector<class Component*> components;
	class SceneNode* scene_node;
};

template<class T>
T* Actor::Get_Component()
{
	for (int i = 0; i < components.size(); ++i)
	{
		T* t = dynamic_cast<T*>(components[i]);
		if (t != nullptr)
		{
			return t;
		}
	}
	return nullptr;
}