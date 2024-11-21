#include "PlantCommand.h"

#include <glm/gtx/string_cast.hpp>
#include <thsan/ressourceManager/SoundManager.h>

PlantCommand::PlantCommand()
{
}

void PlantCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	//make sure the animationis set to be played
	if (!parent.hasData(DATA_TYPE::PLAYER)) {
		data = new PlayerData();
		parent.setData(DATA_TYPE::PLAYER, data);
	}
	else {
		data = parent.getData<PlayerData>(DATA_TYPE::PLAYER);
	}

	if (data->curr_state == PlayerData::State::idle)
		data->curr_state = PlayerData::State::plant;
	//remove the seed from existence

	//make these line of code exist after the animation

	RessourceManager::SoundManager::play("media/sound/crop.mp3");

	parent.addOnSpriteAnimationRenderEvent("plant", 0.8f, true, [&](GameObject& go, Scene& scene, const sf::Time& at) {
		plantFactory = new PlantFactory(&scene);
		GameObject* plant = plantFactory->createTrilleRouge();
		PlantData* plant_data = plant->getData<PlantData>(DATA_TYPE::PLANT);

		Transform* tr_plant = plant->getData<Transform>(DATA_TYPE::TRANSFORM);
		Transform* tr_player = parent.getData<Transform>(DATA_TYPE::TRANSFORM);

		plant_data->current_state = PlantData::State::planted;

		tr_plant->position = tr_player->position;
		tr_plant->scale = glm::vec2(0.25f, 0.25f);
	});
}

void PlantCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
}
