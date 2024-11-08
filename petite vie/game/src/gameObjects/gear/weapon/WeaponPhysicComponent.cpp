#include "WeaponPhysicComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>

WeaponPhysicComponent::WeaponPhysicComponent(std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> effect, std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> undo)
	:effect(effect), undo(undo)
{

}

void WeaponPhysicComponent::init(Scene& scene)
{
	if (!parent->hasData(DATA_TYPE::CHARACTER_STAT)) {
		charStatData = new CharacterStatData();
		parent->setData(DATA_TYPE::CHARACTER_STAT, charStatData);
	}
	else
		charStatData = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);

	if (!parent->hasData(DATA_TYPE::GEAR)) {
		gearData = new GearData();
		parent->setData(DATA_TYPE::GEAR, gearData);
	}else
		gearData = parent->getData<GearData>(DATA_TYPE::CHARACTER_STAT);

	init_for_attack(scene);
}

void WeaponPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
	gearData = parent->getData<GearData>(DATA_TYPE::CHARACTER_STAT);

	if (gearData->isEquiped && !isEffectGiven && gearData->owner_name != "") {
		target = scene.get_child(gearData->owner_name);
		effect(*parent, *target, scene, *charStatData);
		isEffectGiven = true;
	}
	else if (!gearData->isEquiped && isEffectGiven) {
		undo(*parent, *target, scene, *charStatData);
		isEffectGiven = false;
	}

	if (gearData->owner_name == "") {
		//to do:
		// assuming it has a position and is on ground
		// try to move closer to the nearest player
		// not done yet due to not being physical on ground
	}

	on_attack(scene, dt);
}

void WeaponPhysicComponent::onDelete(Scene& scene)
{
	if (!gearData->isEquiped && isEffectGiven) {
		undo(*parent, *target, scene, *charStatData);
		isEffectGiven = false; // genuinely useless in this context considering it gets deleted
	}
}
