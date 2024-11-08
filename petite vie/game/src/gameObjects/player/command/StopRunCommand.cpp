#include "StopRunCommand.h"

StopRunCommand::StopRunCommand()
{
}

void StopRunCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	PlayerData* playerData{ nullptr };
	if (parent.hasData(DATA_TYPE::PLAYER)) {
		playerData = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
		if (playerData->curr_state == PlayerData::State::run)
			playerData->curr_state = PlayerData::State::walk;
	}
}

void StopRunCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	PlayerData* playerData{ nullptr };
	if (parent.hasData(DATA_TYPE::PLAYER)) {
		playerData = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
		if (playerData->curr_state == PlayerData::State::walk)
			playerData->curr_state = PlayerData::State::run;
	}
}
