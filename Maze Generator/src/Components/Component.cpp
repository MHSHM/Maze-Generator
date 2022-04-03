#include <Component.h>
#include <Actor.h>

Component::Component(Actor* _owner) :
	owner(_owner) {}

void Component::Update(float deltatime)
{
}

void Component::Clear()
{
}

Component::~Component()
{
}
