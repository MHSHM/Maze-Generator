#include <Game.h>
#include <thread>

#include <glfw3.h>

Scene::Scene():
	game(nullptr)
{

}

void Scene::Init(Game* _game)
{

}

void Scene::Update(float deltatime)
{

}

void Scene::Draw()
{
}

void Scene::Load_Data()
{
}

void Scene::Clear()
{
	while (!scene_nodes.empty())
	{
		game->Remove_Scene_Node(this, scene_nodes.back());
		scene_nodes.pop_back(); 
	}
}
