#include "AttackCommand.h"
#include <iostream>
#include <thsan/ressourceManager/SoundManager.h>

AttackCommand::AttackCommand()
{
}

void AttackCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	//make these line of code exist after the animation
	PlayerData* player = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
	if (player->curr_state != PlayerData::State::damage) 
	{
		player->curr_state = PlayerData::State::attack;
		RessourceManager::SoundManager::play("media/sound/punch.ogg");
	}
}

void AttackCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
}
