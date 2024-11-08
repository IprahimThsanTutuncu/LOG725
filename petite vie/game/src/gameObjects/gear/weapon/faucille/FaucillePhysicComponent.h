#pragma once

#include "../WeaponPhysicComponent.h"

class PlayerData;

class FaucillePhysicComponent : public WeaponPhysicComponent {
public:
	FaucillePhysicComponent(
		std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> effect,
		std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> undo
	);

	void init_for_attack(Scene& scene) override;
	void on_attack(Scene& scene, const sf::Time& dt) override;
private:
	sf::Time elapsedTime{ sf::Time::Zero };
	PlayerData* player_data;

	bool is_attack_happening{false};
};