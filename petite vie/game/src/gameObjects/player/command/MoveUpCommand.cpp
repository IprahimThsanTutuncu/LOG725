#include "MoveUpCommand.h"
#include <iostream>
MoveUpCommand::MoveUpCommand()
{
    //ctor
}

MoveUpCommand::~MoveUpCommand()
{
    //dtor
}

void MoveUpCommand::execute(GameObject &parent, Scene &scene, const sf::Time& dt)
{
	physicBodyData* phy = parent.getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
	Transform* trans = parent.getData<Transform>(DATA_TYPE::TRANSFORM);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getData<CameraData>(DATA_TYPE::CAMERA)->angle;

	PlayerData* pd = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
	const double pi = 3.14159265358979323846;

	glm::vec3 n;

	if (pd->target_name != "") {
		GameObject* target = scene.get_child(pd->target_name);
		if (target) {
			glm::vec3 target_position = target->getData<Transform>(DATA_TYPE::TRANSFORM)->position;
			n = trans->position - target_position;
			n = glm::vec3(n.x, 0.f, n.z);
			if (glm::length(n) < 1.f)
				n = glm::vec3(0.f);
			else
				n = glm::normalize(n);

		}
	}
	else {
		n = glm::normalize(glm::vec3(sin(angle_camera), 0.f, cos(angle_camera)));
	}
	phy->direction += -n;
	phy->force = 2.f;
}

void MoveUpCommand::undo(GameObject &parent, Scene &scene, const sf::Time& dt)
{
	physicBodyData* phy = parent.getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getData<CameraData>(DATA_TYPE::CAMERA)->angle;

	const double pi = 3.14159265358979323846;
	glm::vec3 n = glm::normalize(glm::vec3(sin(angle_camera), 0.f, cos(angle_camera)));

	phy->direction += n;
	phy->force = 2.f;
}

