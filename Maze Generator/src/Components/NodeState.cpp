#include <NodeState.h>
#include <Actor.h>


NodeState::NodeState(Actor* _owner) :
	Component(_owner),
	is_blocked(true),
	is_visited(false),
	is_visited_path(false),
	is_valid_path(false)
{

}