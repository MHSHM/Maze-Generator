#include <Game.h>
#include <SceneNode.h>
#include <Input.h>
#include <Rigidbody.h>
#include <Collider.h>
#include <Camera.h>
#include <NodeState.h>
#include <random>
#include <thread>
#include <chrono>

#define GLEW_STATIC

#include <iostream>
#include <sstream>
#include <fstream>
#include <glew.h>
#include <glfw3.h>

Game::Game() :
	is_game_running(true),
	window(nullptr),
	time_since_last_frame(0.0f),
	renderer(this)
{
	
}

bool Game::Init()
{
	scene_nodes.reserve(MAX_SCENE_NODES); 
	actors.reserve(MAX_ACTOR);
	transforms.reserve(MAX_RENDERABLE);
	renders.reserve(MAX_RENDERABLE);
	states.reserve(MAX_SCENE_NODES);

	if (!glfwInit())
	{
		std::cerr << "failed to Initialize GLFW!\n";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze Generator", NULL, NULL);
	if (!window)
	{
		std::cerr << "failed to create window!\n";
		return false;
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}

	grid_scene.Init(this);

	std::thread maze_generator(&Grid::Backtracking_Maze_Generator, &grid_scene, grid_scene.start);
	maze_generator.detach();

	renderer.Initialize();

	Load_Shaders();

	return true;
}

void Game::Run_Game()
{
	while (is_game_running)
	{
		Process_Input();
		Update();
		Generate_Output();
	}
}

void Game::Shutdown()
{
	scene_nodes.clear();
	actors.clear();
	textures.clear();
	transforms.clear();
	renders.clear();
	states.clear();

	for (auto shader : shaders_table) 
	{
		shader.second.Clear();
	}

	shaders_table.clear();

	glfwTerminate();
}

void Game::Process_Input()
{
	glfwPollEvents();

	is_game_running = !glfwWindowShouldClose(window);
}


void Game::Update()
{
	float deltatime = Get_Deltatime();

	grid_scene.Update(deltatime);
}

void Game::Generate_Output()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	renderer.Draw_Instances();

	glfwSwapBuffers(window);
}

void Game::Load_Shaders()
{
	Shader shader; 
	shader.Create_Shader_Program("Shaders/shader.vert", "Shaders/shader.frag"); 
	shaders_table["shader"] = std::move(shader);
}

Texture* Game::Load_Texture(const std::string& texture_path)
{
	for (auto& texture : textures)
	{
		if (texture.first == texture_path) 
		{
			texture.second.counter++;
			return &texture.second;
		}
	}

	Texture texture;
	if (texture.Init(texture_path))
	{
		textures.push_back({texture_path, std::move(texture)});
		return &(textures.back().second);
	}

	return nullptr;
}

void Game::Remove_Texture(Texture* texture)
{

	if (texture == nullptr) 
	{
		return;
	}

	if (texture->counter > 1)
	{
		texture->counter -= 1;
		return;
	}

	texture->counter = 0;

	std::pair<std::string, Texture>* target = nullptr; 
	for (auto& pair : textures) 
	{
		if (&pair.second == texture) 
		{
			target = &pair;
		}
	}

	if (target == nullptr) 
	{
		return;
	}

	// find which render component has a pointer to the back texture
	// make it point to the correct slot
	Texture* back_texture = &textures.back().second;

	for (auto& scene_node : active_scene->scene_nodes)
	{
		Render* render = scene_node->actor->Get_Component<Render>();

		if (render == nullptr)	continue; 

		if (render->texture == back_texture) 
		{
			render->texture = texture;
			break; 
		}
	}

	std::iter_swap(target, textures.end() - 1);

	textures.back().second.Clear();
	textures.pop_back();
}

SceneNode* Game::Create_Scene_Node(Scene* scene, const std::string& _tag)
{
	scene_nodes.push_back(SceneNode(scene));
	scene_nodes.back().tag = _tag;
	scene_nodes.back().children.reserve(MAX_CHILDREN);
	actors.emplace_back(Actor(&scene_nodes.back()));
	scene_nodes.back().actor = &(actors.back());
	scene->scene_nodes.push_back(&scene_nodes.back());
	return &scene_nodes.back();
}

void Game::Remove_Scene_Node(Scene* scene, SceneNode* scene_node)
{

	for (auto &child : scene_node->children)
	{
		Remove_Scene_Node(scene, child);
	}

	// Here to delete a scene node we swap the node with the back of the vector
	// to avoid shifting when using erase
	// we still need to fix the children of the back as they will be pointing
	// to different parent after swapping
	SceneNode* back_scene_node = &scene_nodes.back();

	for (auto& node : scene->scene_nodes) 
	{
		if (node == back_scene_node) 
		{
			node = scene_node; 
		}
	}

	if (back_scene_node->parent) 
	{
		for (auto& child : back_scene_node->parent->children) 
		{
			if (child == back_scene_node) 
			{
				child = scene_node;
			}
		}
	}

	if (scene_node->parent)
	{
		scene_node->parent->Remove_Child(scene_node);
	}

	for (auto& actor : actors) 
	{
		if (actor.scene_node == back_scene_node) 
		{
			actor.scene_node = scene_node;
		}
	}

	for (auto& child : back_scene_node->children)
	{
		child->parent = scene_node;
	}
	
	Remove_Actor(scene, scene_node->actor);
	
	std::iter_swap(scene_node, scene_nodes.end() - 1);

	scene_nodes.pop_back();
}

void Game::Remove_Actor(Scene* scene, Actor* actor)
{

	for (auto& node : scene->scene_nodes) 
	{
		if (node->actor == &actors.back()) 
		{
			node->actor = actor; 
		}
	}

	auto iter = std::find(actors.begin(), actors.end(), *actor);

	if (iter != actors.end()) 
	{
		for (auto& cmp : actor->components) 
		{
			cmp->Clear();
		}

		for (auto& cmp : actor->components) 
		{
			Remove_Component(scene, cmp);
		}

		for (auto& scene_node : scene_nodes) 
		{
			if (scene_node.actor == &actors.back()) 
			{
				scene_node.actor = actor;
			}
		}

		for (auto& cmp : actors.back().components) 
		{
			cmp->owner = actor;
		}

		std::iter_swap(actor, actors.end() - 1);
		actors.pop_back();
	}
}

void Game::Add_Component(SceneNode* scene_node, ComponentType type)
{
	switch (type)
	{
	case ComponentType::Transform:
	{
		transforms.push_back(Transform(scene_node->actor));
		scene_node->actor->components.push_back(&(transforms.back()));
	}
	break;
	case ComponentType::Render:
	{
		renders.push_back(Render(scene_node->actor));
		scene_node->actor->components.push_back(&(renders.back()));
	}
	break;
	case ComponentType::NodeState:
	{
		states.push_back(NodeState(scene_node->actor));
		scene_node->actor->components.push_back(&(states.back()));
	}
	break;
	}
}

void Game::Remove_Component(Scene* scene, Component* cmp)
{
	Transform* transform = dynamic_cast<Transform*>(cmp);
	if (transform != nullptr) 
	{
		Transform* back_transform = &(transforms.back());

		Actor* owner = back_transform->owner;

		for (auto& ptr : owner->components)
		{
			if (ptr == back_transform) 
			{
				ptr = transform; 
			}
		}

		std::iter_swap(transform, transforms.end() - 1);
		transforms.pop_back();

		return;
	}

	Render* render = dynamic_cast<Render*>(cmp);
	if (render != nullptr)
	{
		Render* back_render = &(renders.back());

		Actor* owner = back_render->owner;

		for (auto& ptr : owner->components)
		{
			if (ptr == back_render)
			{
				ptr = render; 
			}
		}

		std::iter_swap(render, renders.end() - 1);
		renders.pop_back();

		return;
	}

	NodeState* state = dynamic_cast<NodeState*>(cmp);
	if (state != nullptr)
	{
		NodeState* back_nodeState = &(states.back());

		Actor* owner = back_nodeState->owner;

		for (auto& ptr : owner->components)
		{
			if (ptr == back_nodeState)
			{
				ptr = state; 
			}
		}

		std::iter_swap(state, states.end() - 1);
		states.pop_back();

		return;
	}
}

float Game::Get_Deltatime()
{
	float delta_time = glfwGetTime() - time_since_last_frame;
	time_since_last_frame = glfwGetTime();
	return delta_time;
}

Game::~Game()
{
}
