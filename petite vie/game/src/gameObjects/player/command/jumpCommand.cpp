#include "jumpCommand.h"
#include <thsan/ressourceManager/SoundManager.h>

JumpCommand::JumpCommand()
{
}

void JumpCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	if (!parent.hasData(DATA_TYPE::PHYSIC_BODY))
		return;

	physicBodyData* phy = parent.getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);

	if (phy->onGround) {
		phy->isJump = true;
		phy->onGround = false;
		RessourceManager::SoundManager::play("media/sound/jump.ogg");

	}
	else {
		phy->isDoubleJump = true;
	}

}

void JumpCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
}
