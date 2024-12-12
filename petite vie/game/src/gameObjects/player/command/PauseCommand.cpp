#include "PauseCommand.h"
#include "../../../states/factory/UIFactory.h"
#include <thsan/ressourceManager/StateManager.h>

PauseCommand::PauseCommand()
{
}
void PauseCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	std::shared_ptr<State> current = RessourceManager::StateManager::get<State>(scene.getParent()->getId());
	if (current) {
		UIFactory uiFactory(current);

		std::shared_ptr<State> playerPause = uiFactory.createPlayerPause(parent, scene);
		current->getParent()->changeState(playerPause, true);
	}
}