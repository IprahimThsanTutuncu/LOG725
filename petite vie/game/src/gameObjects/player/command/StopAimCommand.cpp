#include "StopAimCommand.h"
#include <iostream>

StopAimCommand::StopAimCommand()
{
}

void StopAimCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* bars = scene.get_child("bars");

	PlayerData* player = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
	if (player->target_name != "")
		player->target_name = "";

	player->isAiming = false;

	if (!bars)
		return;

	if (bars->hasData(DATA_TYPE::BARS))
		bars->getData<BarsData>(DATA_TYPE::BARS)->show_bar = false;
}

void StopAimCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{

}
