#include <vector>
#include <string>
#include <unordered_map>

#include <SceneNode.h>
#include <Actor.h>


class Scene
{
public:
	Scene();

public:
	virtual void Init(class Game* _game);
	virtual void Load_Data();
	virtual void Update(float deltatime);
	virtual void Draw(); 
	void Clear();

public:
	std::string tag;
	std::vector<SceneNode*> scene_nodes;
	std::vector<SceneNode*> dead_scene_nodes;
	class Game* game;	 
};