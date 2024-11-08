#include "RotateLeftCommand.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>

RotateLeftCommand::RotateLeftCommand()
{
	//ctor
}

RotateLeftCommand::~RotateLeftCommand()
{
	//dtor
}

void RotateLeftCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* go = scene.get_child("camera");
	CameraData* camera_data = go->getData<CameraData>(DATA_TYPE::CAMERA);
	PlayerData* player_data = parent.getData<PlayerData>(DATA_TYPE::PLAYER);

	if (player_data->target_name == "" && scene.pseudo3DRendering_getViewType() == VIEW_TYPE::DYNAMIC_VIEW)
		camera_data->angle += dt.asSeconds() * 3.f;
}

void RotateLeftCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* go = scene.get_child("camera");
	CameraData* camera_data = go->getData<CameraData>(DATA_TYPE::CAMERA);
	PlayerData* player_data = parent.getData<PlayerData>(DATA_TYPE::PLAYER);

	if (player_data->target_name == "" && scene.pseudo3DRendering_getViewType() == VIEW_TYPE::DYNAMIC_VIEW)
		camera_data->angle -= dt.asSeconds() * 3.f;
}

