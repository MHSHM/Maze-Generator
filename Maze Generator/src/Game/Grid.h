#pragma once

#include <Scene.h>
#include <mutex>

#define TILE_WIDTH 5
#define TILE_HEIGHT 5

class Grid : public Scene 
{
public:
	Grid();
	void Load_Data() override;
	void Init(class Game* _game) override;
	void Update(float deltatime) override;
public:
	void Generate_Random_Maze(uint16_t difficulty);
	void Backtracking_Maze_Generator(SceneNode* start);
	void Find_Path_DFS(SceneNode* start, SceneNode* destination);
	void Find_Path_BFS(SceneNode* start, SceneNode* destination); 
	void Find_Path_Dijkstra(SceneNode* start, SceneNode* destination); 
	void Find_Path_A_Star(SceneNode* start, SceneNode* destination); 
	void Construct_Path(SceneNode* destination, SceneNode* start);
public: 
	std::vector< std::vector<SceneNode*> > grid;
	SceneNode* start;
	SceneNode* destination;

	uint32_t row_size;
	uint32_t col_size;
};