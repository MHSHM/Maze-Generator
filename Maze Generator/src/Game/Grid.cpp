#include <Game.h>
#include <NodeState.h>
#include <random>
#include <stack>
#include <queue>
#include <set>
#include <glfw3.h>
#include <thread>
#include <chrono>
#include <iostream>

static glm::vec3 valid_color = glm::vec3(1.0f, 1.0f, 1.0f);
static glm::vec3 blocked_color = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 start_color = glm::vec3(1.0f, 0.0f, 0.0f);
static glm::vec3 destination_color = glm::vec3(0.0f, 0.0f, 1.0f); 
static glm::vec3 visited_color = glm::vec3(0.0f, 1.0f, 0.0f); 
static glm::vec3 valid_path_color = glm::vec3(1.0f, 0.0f, 0.0f);
static int di[] = {0, 1, -1, 0};
static int dj[] = {1, 0, 0, -1};
static int odd_di[] = {0, 2, -2, 0};
static int odd_dj[] = {2, 0, 0, -2};

Grid::Grid():
	Scene()
{

}

void Grid::Init(Game* _game)
{
	game = _game;
	scene_nodes.reserve(MAX_SCENE_NODES);
	dead_scene_nodes.reserve(MAX_SCENE_NODES);

	Load_Data();
}

void Grid::Load_Data()
{
	col_size = (int)(WINDOW_WIDTH / TILE_WIDTH);
	row_size = (int)(WINDOW_HEIGHT / TILE_HEIGHT);

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist_col(0, col_size - 2);
	std::uniform_int_distribution<std::mt19937::result_type> dist_row(0, row_size - 2);

	SceneNode* player = game->Create_Scene_Node(this, "player");
	game->Add_Component(player, ComponentType::Transform);
	game->Add_Component(player, ComponentType::Render);

	grid.resize(row_size);

	for (int i = 0; i < row_size; ++i)
	{
		grid[i].resize(col_size);
	}

	int cnt = 0;
	for (int i = 0; i < row_size; ++i)
	{
		for (int j = 0; j < col_size; ++j)
		{
			grid[i][j] = game->Create_Scene_Node(this, std::to_string(cnt++));
			
			game->Add_Component(grid[i][j], ComponentType::Transform);
			game->Add_Component(grid[i][j], ComponentType::Render);
			game->Add_Component(grid[i][j], ComponentType::NodeState);

			Transform* transform = grid[i][j]->actor->Get_Component<Transform>();
			transform->translation = glm::vec3(j * TILE_WIDTH, i * TILE_HEIGHT , 0.0f);
			
			NodeState* state = grid[i][j]->actor->Get_Component<NodeState>();
			state->row = i;
			state->col = j;

			if (state->is_blocked) 
			{
				Render* render = grid[i][j]->actor->Get_Component<Render>();
				render->color = blocked_color; 
			}
		}
	}

	// Random start and destination
	/*
	uint32_t start_row = dist_row(rng) * 2 + 1;
	uint32_t start_col = dist_col(rng) * 2 + 1;
	
	start_row = std::min(start_row, row_size - 2); 
	start_col = std::min(start_col, col_size - 2); 

	uint32_t destination_row = dist_row(rng) * 2 + 1; 
	uint32_t destination_col = dist_col(rng) * 2 + 1;

	destination_row = std::min(destination_row, row_size - 2);
	destination_col = std::min(destination_col, col_size - 2);
	*/
	start = grid[1][1];
	destination = grid[row_size - 2][col_size - 2];

	game->renderer.renderer_data.data_per_instance.translations.reserve(row_size * col_size);
	game->renderer.renderer_data.data_per_instance.colors.reserve(row_size * col_size);

	for (int i = 0; i < row_size; ++i)
	{
		for (int j = 0; j < col_size; ++j)
		{
			glm::vec3 translation = grid[i][j]->actor->Get_Component<Transform>()->translation;
			glm::vec3 color = grid[i][j]->actor->Get_Component<Render>()->color; 

			game->renderer.renderer_data.data_per_instance.translations.push_back(translation); 
			game->renderer.renderer_data.data_per_instance.colors.push_back(color);
		}
	}
}

void Grid::Update(float deltatime)
{
	// scene logic

	// Update Renderer data
	std::vector<glm::vec3> new_colors;
	new_colors.reserve(row_size * col_size);
	
	for (int i = 0; i < row_size; ++i) 
	{
		for (int j = 0; j < col_size; ++j)
		{
			SceneNode* node = grid[i][j];
			NodeState* state = node->actor->Get_Component<NodeState>();
			Render* render = node->actor->Get_Component<Render>(); 

			if (!state->is_blocked)
			{

				if (node == destination) 
				{
					render->color = destination_color; 
					new_colors.emplace_back(render->color);
					continue; 
				}

				if (node == start) 
				{
					render->color = start_color;
					new_colors.emplace_back(render->color);
					continue;
				}

				render->color = valid_color;
			}

			if (state->is_blocked)
			{
				render->color = blocked_color;
			}

			// has been visited while searching for a path
			if (state->is_visited_path) 
			{
				render->color = visited_color; 
			}

			if (state->is_valid_path) 
			{
				render->color = valid_path_color; 
			}
			
			new_colors.emplace_back(render->color);
		}
	}

	game->renderer.Update_Instances_Color(new_colors);
}


void Grid::Generate_Random_Maze(uint16_t difficulty)
{
	// This function randomly blocks cells in the grid
	// no path guaranteed
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist_col(0, col_size - 1);
	std::uniform_int_distribution<std::mt19937::result_type> dist_row(0, row_size - 1);

	int i = 0;

	for (int i = 0; i < difficulty; ++i)
	{
		int x = dist_row(rng);
		int y = dist_col(rng);

		SceneNode* node = grid[x][y]; 

		if (node == start || node == destination)
		{
			continue;
		}

		NodeState* state = node->actor->Get_Component<NodeState>(); 
		state->is_blocked = true;

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::thread path_finder(&Grid::Find_Path_A_Star, this, start, destination);
	path_finder.detach();

}

void Grid::Backtracking_Maze_Generator(SceneNode* start)
{
	std::stack<SceneNode*> st;
	st.push(start);

	while (!st.empty())
	{
		SceneNode* current = st.top(); 
		NodeState* current_state = current->actor->Get_Component<NodeState>();
		std::vector<SceneNode*> neighbours; 
		std::vector<SceneNode*> odd_neighbours;
		current_state->is_visited = true;
		current_state->is_blocked = false; 
		neighbours.reserve(4);
		odd_neighbours.reserve(4);

		for (int i = 0; i < 4; ++i)
		{
			int new_i = current_state->row + odd_di[i];
			int new_j = current_state->col + odd_dj[i];
			if (new_i >= 0 && new_i < row_size && new_j >= 0 && new_j < col_size)
			{
				NodeState* neigh_state = grid[new_i][new_j]->actor->Get_Component<NodeState>();
 				if (!neigh_state->is_visited)
				{
					odd_neighbours.emplace_back(grid[new_i][new_j]);
					neighbours.emplace_back(grid[current_state->row + di[i]][current_state->col + dj[i]]); 
				}
			}
		}
			
		if (!odd_neighbours.empty())
		{
			std::random_device d;
			std::mt19937 r(d());
			std::uniform_int_distribution<std::mt19937::result_type> random_index(0, odd_neighbours.size() - 1);
			int index = random_index(r);
			st.push(odd_neighbours[index]);
			
			neighbours[index]->actor->Get_Component<NodeState>()->is_blocked = false;
		}
		else
		{
			st.pop();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::thread path_finder(&Grid::Find_Path_A_Star, this, start, destination);
	path_finder.detach();
}

void Grid::Find_Path_DFS(SceneNode* start, SceneNode* destination)
{
	std::stack<SceneNode*> st; 
	st.push(start); 

	while (!st.empty())
	{
		SceneNode* current = st.top();

		if (current == destination) 
		{
			break;
		}

		NodeState* state = current->actor->Get_Component<NodeState>(); 
		state->is_visited_path = true;
		st.pop(); 

		for (int i = 0; i < 4; ++i)
		{
			int new_i = state->row + di[i];
			int new_j = state->col + dj[i];
			if (new_i >= 0 && new_i < row_size && new_j >= 0 && new_j < col_size)
			{
				NodeState* neigh_state = grid[new_i][new_j]->actor->Get_Component<NodeState>();
				if (!neigh_state->is_visited_path && !neigh_state->is_blocked)
				{
					neigh_state->parent = current; 
					st.push(grid[new_i][new_j]); 
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::thread path_construct(&Grid::Construct_Path, this, destination, start);
	path_construct.detach();
}

void Grid::Find_Path_BFS(SceneNode* start, SceneNode* destination)
{
	std::queue<SceneNode*> q;
	q.push(start); 

	while (!q.empty()) 
	{
		SceneNode* current = q.front(); 

		if (current == destination)
		{
			break;
		}

		NodeState* state = current->actor->Get_Component<NodeState>();
		state->is_visited_path = true; 
		q.pop();

		for (int i = 0; i < 4; ++i)
		{
			int new_i = state->row + di[i];
			int new_j = state->col + dj[i];
			if (new_i >= 0 && new_i < row_size && new_j >= 0 && new_j < col_size)
			{
				NodeState* neigh_state = grid[new_i][new_j]->actor->Get_Component<NodeState>();
				if (!neigh_state->is_visited_path && !neigh_state->is_blocked)
				{
					neigh_state->parent = current;
					q.push(grid[new_i][new_j]);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::thread path_construct(&Grid::Construct_Path, this, destination, start);
	path_construct.detach();

}

void Grid::Find_Path_Dijkstra(SceneNode* start, SceneNode* destination)
{
	auto comp = [](const NodeState* left, const NodeState* right) 
	{
		return left->distance < right->distance; 
	};

	NodeState* start_node_state = start->actor->Get_Component<NodeState>();
	start_node_state->distance = 0;

	std::multiset < NodeState*, decltype(comp)> st(comp); 
	st.insert(start_node_state); 

	while (!st.empty()) 
	{
		NodeState* current_node_state = *st.begin();

		if (current_node_state->owner->scene_node == destination) 
		{
			break; 
		}

		current_node_state->is_visited_path = true;

		st.erase(st.begin());

		for (int i = 0; i < 4; ++i)
		{
			int new_i = current_node_state->row + di[i];
			int new_j = current_node_state->col + dj[i];
			if (new_i >= 0 && new_i < row_size && new_j >= 0 && new_j < col_size)
			{
				NodeState* neigh_state = grid[new_i][new_j]->actor->Get_Component<NodeState>();

				if (neigh_state->is_visited_path || neigh_state->is_blocked) 
				{
					continue; 
				}

				uint32_t distance = current_node_state->distance + 1; 

				auto iter = st.find(neigh_state); 

				if (iter == st.end()) 
				{
					neigh_state->distance = distance; 
					neigh_state->parent = current_node_state->owner->scene_node; 
					st.insert(neigh_state); 
				}
				else 
				{
					if (distance < neigh_state->distance) 
					{
						st.erase(iter); 
						neigh_state->distance = distance; 
						neigh_state->parent = current_node_state->owner->scene_node; 
						st.insert(neigh_state); 
					}
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::thread path_construct(&Grid::Construct_Path, this, destination, start);
	path_construct.detach();
}

void Grid::Find_Path_A_Star(SceneNode* start, SceneNode* destination)
{
	auto comp = [](const NodeState* left, const NodeState* right)
	{
		return left->f_cost < right->f_cost;
	};

	auto dist = [](const NodeState* a, const NodeState* b)
	{
		return std::abs((int)a->row - (int)b->row) + std::abs((int)a->col - (int)b->col);
	};

	NodeState* start_node_state = start->actor->Get_Component<NodeState>();
	NodeState* destination_node_state = destination->actor->Get_Component<NodeState>(); 
	start_node_state->f_cost = dist(start_node_state, destination_node_state);

	std::multiset < NodeState*, decltype(comp)> st(comp);
	st.insert(start_node_state);

	while (!st.empty())
	{
		NodeState* current_node_state = *st.begin();

		if (current_node_state->owner->scene_node == destination)
		{
			break;
		}

		current_node_state->is_visited_path = true;

		st.erase(st.begin());

		for (int i = 0; i < 4; ++i)
		{
			int new_i = current_node_state->row + di[i];
			int new_j = current_node_state->col + dj[i];

			if (new_i >= 0 && new_i < row_size && new_j >= 0 && new_j < col_size)
			{
				NodeState* neigh_state = grid[new_i][new_j]->actor->Get_Component<NodeState>();

				if (neigh_state->is_visited_path || neigh_state->is_blocked)
				{
					continue;
				}

				uint32_t h_cost = dist(neigh_state, destination_node_state);
				uint32_t f_cost = h_cost;

				auto iter = st.find(neigh_state);

				if (iter == st.end())
				{
					neigh_state->f_cost = f_cost;
					neigh_state->parent = current_node_state->owner->scene_node;
					st.insert(neigh_state);
				}
				else
				{
					if (f_cost < neigh_state->f_cost)
					{
						st.erase(iter);
						neigh_state->f_cost = f_cost;
						neigh_state->parent = current_node_state->owner->scene_node;
						st.insert(neigh_state);
					}
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::thread path_construct(&Grid::Construct_Path, this, destination, start);
	path_construct.detach();
}

void Grid::Construct_Path(SceneNode* destination, SceneNode* start)
{
	std::stack<SceneNode*> st; 
	st.push(destination);

	while (!st.empty()) 
	{
		SceneNode* current = st.top(); 
		NodeState* state = current->actor->Get_Component<NodeState>(); 
		state->is_valid_path = true;

		if (current == start) 
		{
			break;
		}

		if (state->parent)
		{
			st.push(state->parent); 
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

