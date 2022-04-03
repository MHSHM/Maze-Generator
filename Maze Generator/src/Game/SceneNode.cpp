#include <Game.h>

SceneNode::SceneNode(Scene* _scene) :
	actor(nullptr),
	parent(nullptr),
	scene(_scene)
{
}

void SceneNode::Add_Child(SceneNode* child)
{
	child->parent = this;
	children.emplace_back(child);
}

void SceneNode::Remove_Child(SceneNode* child)
{
	auto iter = std::find(children.begin(), children.end(), child); 

	if (iter == children.end()) 
	{
		return; 
	}

	child->parent = nullptr;

	children.erase(iter);
}