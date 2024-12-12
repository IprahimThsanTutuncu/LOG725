#pragma once

#include <memory>
#include <string>

//todo in 2000 years,
//included due to BagData having nested struct. Can't forward declare
//consider decoupling
#include <thsan/gameObject/data/DataType.h>

class State;
class GameObject;
class Scene;

class UIFactory {

public:
	UIFactory(std::shared_ptr<State> current);
	~UIFactory() = default;
	std::shared_ptr<State> createPlayerActionChoiceBox(GameObject& player, Scene& scene, PlantData plant);
	std::shared_ptr<State> createPlayerActionChoiceBox(GameObject& player, Scene& scene, BagData::Item item);
	std::shared_ptr<State> createPlayerActionChoiceBox(GameObject& player, Scene& scene, BagData::Gear gear);
	std::shared_ptr<State> createPlayerListChoiceBox(GameObject& player, Scene& scene);
	std::shared_ptr<State> createPlayerPause(GameObject& player, Scene& scene);
	std::shared_ptr<State> createPlayerActionChoiceBox(GameObject& player, Scene& scene);

private:
	std::shared_ptr<State> current;
	std::string playerListChoiceBoxId{""};
};