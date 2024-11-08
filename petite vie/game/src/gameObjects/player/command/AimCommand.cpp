#include "AimCommand.h"
#include <thsan/CollisionEngine/CollisionEngine.h>
#include <unordered_set>
#include <iostream>

AimCommand::AimCommand()
{
}

void AimCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* bars = scene.get_child("bars");

	if (parent.hasData(DATA_TYPE::PLAYER)) {
		PlayerData* player = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
		if (player->target_name == "") {
			GameObject* go = CollisionEngine::getOneClosestGameObjectWithinRadius(parent.getData<Transform>(DATA_TYPE::TRANSFORM), 100, -100, 100, Collision_object::real);
			if (go) {
				player->target_transform = go->getData<Transform>(DATA_TYPE::TRANSFORM);
				player->target_name = go->getName();

				player->isAiming = true;
				player->curr_state = PlayerData::State::charge;

				if (!bars)
					return;

				if (bars->hasData(DATA_TYPE::BARS))
					bars->getData<BarsData>(DATA_TYPE::BARS)->show_bar = true;
			}
		}
	}
}

void AimCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
}

void AimCommand::show_bar()
{
}

void AimCommand::hide_bar()
{
}
