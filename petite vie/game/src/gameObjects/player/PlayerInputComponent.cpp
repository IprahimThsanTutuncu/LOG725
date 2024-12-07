#include "PlayerInputComponent.h"

#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/ressourceManager/StateManager.h>
#include "../../states/postProcess/blackBar/BlackBarPostProcessState.h"
#include "gameObjects/healthbar/HealthBarRenderComponent.h"


PlayerInputComponent::PlayerInputComponent()
{
}

void PlayerInputComponent::init(Scene& scene)
{
	if (!parent->hasData(DATA_TYPE::PLAYER)) {
		player_data = new PlayerData();
		parent->setData(DATA_TYPE::PLAYER, player_data);
	}
	else {
		player_data = parent->getData<PlayerData>(DATA_TYPE::PLAYER);
	}

	if (!parent->hasData(DATA_TYPE::BAG)) {
		bag_data = new BagData();
		parent->setData(DATA_TYPE::BAG, bag_data);
	}
	else {
		bag_data = parent->getData<BagData>(DATA_TYPE::BAG);
	}

	if (!parent->hasData(DATA_TYPE::CHARACTER_STAT)) {
		character_stat_data = new CharacterStatData();
		parent->setData(DATA_TYPE::CHARACTER_STAT, character_stat_data);
	}
	else {
		character_stat_data = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
	}

	player_data->curr_state = PlayerData::State::idle;

	//load from BD...DTO
	character_stat_data->hp = 100;
	character_stat_data->mp = 10;
	character_stat_data->atk = 5;
	character_stat_data->curr.hp = 75;
	character_stat_data->curr.mp = 5;

	aim = new AimCommand();
	stop_aim = new StopAimCommand();


	healthBar = scene.createGameObject("healthbar_" + parent->getName());
	scene.setGameObjectRenderComponent<HealthBarRenderComponent>(healthBar, parent->getName(), sf::Color::Red);
}

void PlayerInputComponent::update(Scene& scene,  const sf::Time& dt, std::vector<Config::InputAction> inputActions)
{
	physicBodyData* phy = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
	PlayerData* player_data = parent->getData<PlayerData>(DATA_TYPE::PLAYER);

	if(phy)
		phy->direction = glm::vec3(0, 0, 0);

	if (player_data->curr_state == PlayerData::State::death)
		return;

    std::vector<Command*> commands = handleInput(inputActions);

	for (auto c : commands) {
		c->execute(*parent, scene, dt);
		historyInput.push_back(c);
	}
	
	commands.clear();
}

std::vector<Command*> PlayerInputComponent::handleInput(std::vector<Config::InputAction> inputActions)
{
	std::vector<Command*> commands;
	
	for (Config::InputAction inputAction : inputActions) {
		if (inputAction == Config::InputAction::up)
			commands.push_back(up);
		if (inputAction == Config::InputAction::down)
			commands.push_back(down);
		if (inputAction == Config::InputAction::left)
			commands.push_back(left);
		if (inputAction == Config::InputAction::right)
			commands.push_back(right);
		if (inputAction == Config::InputAction::aim)
			commands.push_back(aim);
		if (inputAction == Config::InputAction::jump)
			commands.push_back(jump);
		if (inputAction == Config::InputAction::stop_aim)
			commands.push_back(stop_aim);
		if (inputAction == Config::InputAction::rotateLeft)
			commands.push_back(rotateLeft);
		if (inputAction == Config::InputAction::rotateRight)
			commands.push_back(rotateRight);
		if (inputAction == Config::InputAction::run)
			commands.push_back(run);
		if (inputAction == Config::InputAction::stop_run)
			commands.push_back(stop_run);
		if (inputAction == Config::InputAction::plant)
			commands.push_back(plant);
		if (inputAction == Config::InputAction::action)
			commands.push_back(action);
		if (inputAction == Config::InputAction::attack)
			commands.push_back(attack);
	}

	return commands;
}

PlayerInputComponent::~PlayerInputComponent()
{
	delete left;
	delete down;
	delete up;
	delete right;
	delete plant;
	delete jump;
	delete aim;
	delete stop_aim;
	delete rotateLeft;
	delete rotateRight;
	delete action;
	delete attack;
}
