#pragma once

#include <vector>
#include <string>

#define MAX_CHILDREN 10

class SceneNode
{
public:

	SceneNode(class Scene* _scene);

	void Add_Child(SceneNode* child);
	void Remove_Child(SceneNode* child);

	inline bool operator==(const SceneNode& other)
	{
		return (this == &other);
	}

	std::string tag;
	class Actor* actor;
	class Scene* scene;
	SceneNode* parent;
	std::vector<SceneNode*> children;
};