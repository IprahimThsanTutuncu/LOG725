#include "MoveLeftCommand.h"
#include <thsan/ressourceManager/SoundManager.h>

MoveLeftCommand::MoveLeftCommand()
{

}

MoveLeftCommand::~MoveLeftCommand()
{
    //dtor
}

void MoveLeftCommand::execute(GameObject &parent, Scene &scene, const sf::Time& dt)
{
	const double pi = 3.14159265358979323846;

	PlayerData* player_data = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
	Transform* player_transform = parent.getData<Transform>(DATA_TYPE::TRANSFORM);
    physicBodyData* phy = parent.getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);


	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getData<CameraData>(DATA_TYPE::CAMERA)->angle;

	GameObject* target = scene.get_child(player_data->target_name);
	glm::vec3 n;

	if (player_data->target_name != "" && target) {
		if (target) {
			glm::vec3 target_position = target->getData<Transform>(DATA_TYPE::TRANSFORM)->position;
			n = player_transform->position - target_position;
			n = -glm::normalize(glm::vec3(-n.z, 0.f, n.x));

		}
	}
	else {
		n = glm::normalize(glm::vec3(sin(angle_camera + pi / 2), 0.f, cos(angle_camera + pi / 2)));
	}

	phy->direction += -n;
	
	phy->force = 2.f;
}

void MoveLeftCommand::undo(GameObject &parent, Scene &scene, const sf::Time& dt)
{
	PlayerData* data = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
	physicBodyData* phy = parent.getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getData<CameraData>(DATA_TYPE::CAMERA)->angle;

	const double pi = 3.14159265358979323846;
	glm::vec3 n = glm::normalize(glm::vec3(sin(angle_camera + pi / 2), 0.f, cos(angle_camera + pi / 2)));

	phy->direction += n;
	phy->force = 2.f;
} 

