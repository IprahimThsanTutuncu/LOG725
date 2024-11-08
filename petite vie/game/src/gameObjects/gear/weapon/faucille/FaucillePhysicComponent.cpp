#include "FaucillePhysicComponent.h"
#include <thsan/util/helperMath.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>

FaucillePhysicComponent::FaucillePhysicComponent(std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> effect, std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> undo)
	:WeaponPhysicComponent(effect, undo)
{
}

void FaucillePhysicComponent::init_for_attack(Scene& scene)
{
	parent->getName();
}

void FaucillePhysicComponent::on_attack(Scene& scene, const sf::Time& dt)
{
	if (!target)
		return;

	if (target->hasData(DATA_TYPE::PLAYER))
		player_data = target->getData<PlayerData>(DATA_TYPE::PLAYER);

	if (player_data->curr_state == PlayerData::State::attack && !is_attack_happening){
		is_attack_happening = true;
	}

	if (is_attack_happening) {
		
		// todo: get the target that the player has hit

		// make some fancy shit right here tbh
		// ball moving right and left, pew pew
		// depends on weapon.
		// for faucille, just the init is enough
	}

	
}
