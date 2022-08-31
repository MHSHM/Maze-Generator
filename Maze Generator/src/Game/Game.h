#pragma once

#define WINDOW_WIDTH 495
#define WINDOW_HEIGHT 495

#define MAX_SCENES 10
#define MAX_SCENE_NODES 50000
#define MAX_ACTOR 50000
#define MAX_RENDERABLE 50000

#include <Component.h>
#include <Renderer.h>
#include <Shader.h>
#include <Grid.h>
#include <Transform.h>
#include <Render.h>
#include <NodeState.h>
#include <thread>
#include <unordered_map>

class Game
{
public:
	Game();
	bool Init();
	void Run_Game();
	void Shutdown();
	void Process_Input();
	void Update();
	void Generate_Output();
	void Load_Shaders();

public:
	float Get_Deltatime();
	class SceneNode* Create_Scene_Node(class Scene* scene, const std::string& _tag);
	void Remove_Scene_Node(class Scene* scene, SceneNode* scene_node);
	void Remove_Actor(class Scene*scene, class Actor* actor);
	void Add_Component(class SceneNode* scene_node, ComponentType type);
	void Remove_Component(Scene* scene, Component* cmp);
	class Texture* Load_Texture(const std::string& texture_path);
	void Remove_Texture(class Texture* texture);

public:
	class GLFWwindow* window;

	Renderer renderer;
	Grid grid_scene;
	class Scene* active_scene;

	bool is_game_running;
	float time_since_last_frame;

	// Data
	std::vector<SceneNode> scene_nodes;
	std::vector<Actor> actors;
	std::vector<std::pair<std::string, Texture> > textures;
	std::vector<Transform> transforms; 
	std::vector<Render> renders;
	std::vector<NodeState> states;

	std::unordered_map<std::string, Shader> shaders_table;

	~Game();
};

