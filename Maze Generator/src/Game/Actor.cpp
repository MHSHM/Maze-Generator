#include <Actor.h>
#include <Game.h>
#include <Component.h>

#include <iostream>

Actor::Actor(SceneNode* _scene_node):
	scene_node(_scene_node)
{
	components.reserve(MAX_COMPONENTS);
}