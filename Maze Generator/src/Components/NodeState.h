#pragma once

#include <Component.h>
#include <cstdint>
#include <limits.h>

class NodeState : public Component
{
public:
	NodeState(class Actor* _owner);
public:
	uint32_t col; 
	uint32_t row; 

	bool is_blocked;

	// for maze generation
	bool is_visited;

	// for finding the path
	bool is_visited_path;

	// for constructing the path
	bool is_valid_path; 

	// Dijkstra
	// The distance from the start to this node
	uint32_t distance = INT_FAST32_MAX;

	// A*
	uint32_t f_cost = INT_FAST32_MAX;

	class SceneNode* parent = nullptr;
};