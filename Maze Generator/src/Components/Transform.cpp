#include <Transform.h>

Transform::Transform(Actor* _owner) :
	Component(_owner),
	scale_x(1.0f),
	scale_y(1.0f),
	scale_mat(glm::mat4(1.0f)),
	translation(glm::vec3(0.0f, 0.0f, 0.0f)),
	translation_mat(glm::mat4(1.0f)),
	world_transform(glm::mat4(1.0f))
{

}

void Transform::Update(float deltatime)
{
	translation_mat = glm::translate(glm::mat4(1.0f), translation);
	scale_mat = glm::scale(glm::vec3(scale_x, scale_y, 1.0f));
	world_transform = translation_mat * scale_mat;
}

void Transform::Clear()
{
}
