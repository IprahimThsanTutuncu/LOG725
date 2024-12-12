#include "UIFactory.h"
#include "states/choiceBox/ChoiceBoxState.h"
#include "gameObjects/plant/PlantFactory.h"
#include "states/mainMenu/MainMenuState.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>

#include  <thsan/UI/LongText.h>
#include  <thsan/UI/Panel.h>
#include  <thsan/UI/LabelButton.h>
#include  <thsan/UI/CustomButton.h>

#include <thsan/ressourceManager/StateManager.h>
#include <thsan/ressourceManager/SoundManager.h>
#include <thsan/ressourceManager/ThemeManager.h>

UIFactory::UIFactory(std::shared_ptr<State> current):
    current(current)
{

}

std::shared_ptr<State> UIFactory::createPlayerPause(GameObject& player, Scene& scene)
{
	// Verify that the player is the actual player
	if (player.getName() != "player" || !player.hasData(DATA_TYPE::PLAYER))
		return nullptr;

	// Initialize values required for the ChoiceBox
	std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");

	sf::FloatRect player_rect = scene.querySpriteRectAssociateWith("player", true);
	sf::Vector2f position = sf::Vector2f(player_rect.left + 0.10f + player_rect.width / 1.618f, player_rect.top - 0.10f);

	////////////////////////////////////
	//  Declaration of label buttons  //
	////////////////////////////////////

	std::vector<UI::LabelButton*> bttns;

	// Create Pause button
	UI::LabelButton* lbl_bttn_resume = new UI::LabelButton(
		"lbl_bttn_resume",
		"media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf",
		sf::Vector2f(0.15, -0.15),
		"Resume",
		0.6f,
		sf::Color::White
	);

	// Create Quit button
	UI::LabelButton* lbl_bttn_quit = new UI::LabelButton(
		"lbl_bttn_quit",
		"media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf",
		sf::Vector2f(0.55, -0.15),
		"Quit",
		0.6f,
		sf::Color::White
	);

	///////////////////////////////////
	//  Set label buttons direction  //
	///////////////////////////////////

	lbl_bttn_resume->left = lbl_bttn_quit;
	lbl_bttn_resume->right = lbl_bttn_quit;

	lbl_bttn_quit->left = lbl_bttn_resume;
	lbl_bttn_quit->right = lbl_bttn_resume;

	//////////////////////////////////
	//  Connect label buttons event //
	//////////////////////////////////

	// Pause button actions
	lbl_bttn_resume->connect(UI::Action::crossed, [lbl_bttn_resume]() {
		lbl_bttn_resume->setFontColor(sf::Color::White);
		});

	lbl_bttn_resume->connect(UI::Action::hover, [lbl_bttn_resume]() {
		lbl_bttn_resume->setFontColor(sf::Color::Yellow);
		});

	lbl_bttn_resume->connect(UI::Action::pressed, [&scene]() {
		//UIFactory factory(scene.getParent()->getParent()->peekState());
		//std::shared_ptr<State> state = factory.createPlayerActionChoiceBox(player, scene);
		//scene.getParent()->getParent()->changeState(state, true);
		});

	// Quit button actions
	lbl_bttn_quit->connect(UI::Action::crossed, [lbl_bttn_quit]() {
		lbl_bttn_quit->setFontColor(sf::Color::White);
		});

	lbl_bttn_quit->connect(UI::Action::hover, [lbl_bttn_quit]() {
		lbl_bttn_quit->setFontColor(sf::Color::Red);
		});

	lbl_bttn_quit->connect(UI::Action::pressed, [&scene]() {
		Game* game = scene.getParent()->getParent();
		game->changeState(RessourceManager::StateManager::create<MainMenuState>(
			game,
			"media/data/dialogue/mainmenu.txt",
			0
		), true);
		});

	/////////////////////////////////////
	//  Pack label buttons for ChoiceBox //
	/////////////////////////////////////

	bttns.push_back(lbl_bttn_resume);
	bttns.push_back(lbl_bttn_quit);

	////////////////////////////////////////
	//  Create and return ChoiceBox state //
	////////////////////////////////////////

	std::shared_ptr<State> actual_curr = scene.getParent()->getParent()->peekState();
	std::shared_ptr<State> pauseMenu = RessourceManager::StateManager::create<ChoiceBoxState>(
		actual_curr->getParent(),
		actual_curr,
		position,
		sf::Vector2f(0.2, 0.10),
		*shader_param_ptr,
		bttns,
		sf::Color::White,
		true
	);

	return pauseMenu;
}


std::shared_ptr<State> UIFactory::createPlayerActionChoiceBox(GameObject& player, Scene& scene, PlantData plant)
{
	// verification if player is the actual player

    if (player.getName() != "player" || !player.hasData(DATA_TYPE::PLAYER))
        return nullptr;

	// initialization of value required for the ChoiceBox
    std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");

    sf::FloatRect player_rect = scene.querySpriteRectAssociateWith("player", true);
    sf::Vector2f position = sf::Vector2f(player_rect.left + 0.10 + player_rect.width / 1.618f, player_rect.top - 0.10f);

	////////////////////////////////////
	//	Delcaration of label buttons  //
	////////////////////////////////////

	std::vector<UI::LabelButton*> bttns;
	UI::LabelButton* lbl_bttn_eat = new UI::LabelButton("lbl_bttn_action_player_eat", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.15), "eat", 0.6f, sf::Color::White);
	UI::LabelButton* lbl_bttn_plant = new UI::LabelButton("lbl_bttn_action_player_plant", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, -0.15), "plant", 0.6f, sf::Color::White);
	UI::LabelButton* lbl_bttn_throw = new UI::LabelButton("lbl_bttn_action_player_throw", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, 0.15), "throw", 0.6f, sf::Color::White);
	UI::LabelButton* lbl_bttn_wear = new UI::LabelButton("lbl_bttn_action_player_wear", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, 0.15), "wear", 0.6f, sf::Color::White);
	
	///////////////////////////////////
	//	Set label buttons direction  //
	///////////////////////////////////

	lbl_bttn_eat->left = lbl_bttn_plant;
	lbl_bttn_eat->right = lbl_bttn_plant;
	lbl_bttn_eat->up = lbl_bttn_throw;
	lbl_bttn_eat->down = lbl_bttn_throw;

	lbl_bttn_plant->left = lbl_bttn_eat;
	lbl_bttn_plant->right = lbl_bttn_eat;
	lbl_bttn_plant->up = lbl_bttn_wear;
	lbl_bttn_plant->down = lbl_bttn_wear;

	lbl_bttn_throw->left = lbl_bttn_wear;
	lbl_bttn_throw->right = lbl_bttn_wear;
	lbl_bttn_throw->up = lbl_bttn_eat;
	lbl_bttn_throw->down = lbl_bttn_eat;

	lbl_bttn_wear->left = lbl_bttn_throw;
	lbl_bttn_wear->right = lbl_bttn_throw;
	lbl_bttn_wear->up = lbl_bttn_plant;
	lbl_bttn_wear->down = lbl_bttn_plant;

	///////////////////////////////////
	//	Connect label buttons event  //
	///////////////////////////////////

	// eat ////////////////////////////

	lbl_bttn_eat->connect(UI::Action::crossed, [lbl_bttn_eat]() {
		lbl_bttn_eat->setFontColor(sf::Color::White);
		});

	lbl_bttn_eat->connect(UI::Action::hover, [lbl_bttn_eat]() {
		if (lbl_bttn_eat)
			lbl_bttn_eat->setFontColor(sf::Color(255, 128, 0));
		});

	lbl_bttn_eat->connect(UI::Action::pressed, [&, plant]() {
		//make sure the animationis set to be played
		PlayerData* player_data{ nullptr };
		CharacterStatData* char_data{ nullptr };
		BagData* bag_data{ nullptr };

		if (player.hasData(DATA_TYPE::PLAYER)) {
			player_data = player.getData<PlayerData>(DATA_TYPE::PLAYER);
		}
		else
			return;


		if (player.hasData(DATA_TYPE::CHARACTER_STAT)) {
			char_data = player.getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
			char_data->curr.hp += 25;
			if (char_data->curr.hp > 100)
			{
				char_data->curr.hp = 100;
			}
		}
		else
			return;

		if (player.hasData(DATA_TYPE::BAG)) {
			bag_data = player.getData<BagData>(DATA_TYPE::BAG);
		}
		else
			return;

		bag_data->removePlant(plant.getId());
	});

	// plant ////////////////////////////

	lbl_bttn_plant->connect(UI::Action::crossed, [lbl_bttn_plant]() {
		lbl_bttn_plant->setFontColor(sf::Color::White);
	});

	lbl_bttn_plant->connect(UI::Action::hover, [lbl_bttn_plant]() {
		if (lbl_bttn_plant)
			lbl_bttn_plant->setFontColor(sf::Color::Magenta);
	});

	lbl_bttn_plant->connect(UI::Action::pressed, [&, plant]() {
		//make sure the animationis set to be played
		PlayerData* player_data{ nullptr };
		BagData* bag_data{ nullptr };
		RessourceManager::SoundManager::play("media/sound/crop.ogg");

		if (player.hasData(DATA_TYPE::PLAYER)) {
			player_data = player.getData<PlayerData>(DATA_TYPE::PLAYER);
		}
		else
			return;

		if (player.hasData(DATA_TYPE::BAG)) {
			bag_data = player.getData<BagData>(DATA_TYPE::BAG);
		}
		else
			return;

		if (player_data->curr_state == PlayerData::State::idle)
			player_data->curr_state = PlayerData::State::plant;

		PlantFactory plantFactory(&scene);
		GameObject* plant_object = plantFactory.createTrilleRouge();
		PlantData* plant_data = plant_object->getData<PlantData>(DATA_TYPE::PLANT);

		Transform* tr_plant = plant_object->getData<Transform>(DATA_TYPE::TRANSFORM);
		Transform* tr_player = player.getData<Transform>(DATA_TYPE::TRANSFORM);

		plant_data->current_state = PlantData::State::planted;

		tr_plant->position = tr_player->position;
		tr_plant->scale = glm::vec2(0.1f, 0.1f);
		bag_data->removePlant(plant.getId());
	});

	// throw ////////////////////////////

	lbl_bttn_throw->connect(UI::Action::crossed, [lbl_bttn_throw]() {
		lbl_bttn_throw->setFontColor(sf::Color::White);
	});

	lbl_bttn_throw->connect(UI::Action::hover, [lbl_bttn_throw]() {
		if (lbl_bttn_throw)
			lbl_bttn_throw->setFontColor(sf::Color::Cyan);
	});

	lbl_bttn_throw->connect(UI::Action::pressed, [&]() {

	});

	// wear ////////////////////////////

	lbl_bttn_wear->connect(UI::Action::crossed, [lbl_bttn_wear]() {
		lbl_bttn_wear->setFontColor(sf::Color::White);
	});

	lbl_bttn_wear->connect(UI::Action::hover, [lbl_bttn_wear]() {
		if (lbl_bttn_wear)
			lbl_bttn_wear->setFontColor(sf::Color::Green);
	});

	lbl_bttn_wear->connect(UI::Action::pressed, [&]() {

	});

	///////////////////////////////////////
	//  pack label button for choiceBox  //
	///////////////////////////////////////

	bttns.push_back(lbl_bttn_eat);
	bttns.push_back(lbl_bttn_plant);
	bttns.push_back(lbl_bttn_throw);
	bttns.push_back(lbl_bttn_wear);

	///////////////////////////////////////
	//  create ChoiceBox for action      //
	///////////////////////////////////////
	// 
	//vite vite explanation: current scene is for UIFactory. actual current scene that current scene isn't the same that the one of UIFactory.
	std::shared_ptr<State> actual_curr = scene.getParent()->getParent()->peekState();
	std::shared_ptr<State> actionChoiceBox = RessourceManager::StateManager::create<ChoiceBoxState>(
		actual_curr->getParent(),
		actual_curr,
		position,
		sf::Vector2f(0.2, 0.10),
		*shader_param_ptr,
		bttns,
		sf::Color::White,
		true
	);
		
	return actionChoiceBox;
}

std::shared_ptr<State> UIFactory::createPlayerActionChoiceBox(GameObject& player, Scene& scene, BagData::Item item)
{
	return std::shared_ptr<State>();
}

std::shared_ptr<State> UIFactory::createPlayerActionChoiceBox(GameObject& player, Scene& scene, BagData::Gear gear)
{
	return std::shared_ptr<State>();
}

std::shared_ptr<State> UIFactory::createPlayerActionChoiceBox(GameObject& player, Scene& scene)
{
	return std::shared_ptr<State>();
}

std::shared_ptr<State> UIFactory::createPlayerListChoiceBox(GameObject& player, Scene& scene)
{
	// verification if player is the actual player

	if (player.getName() != "player" || !player.hasData(DATA_TYPE::PLAYER) || !player.hasData(DATA_TYPE::BAG))
		return nullptr;

	// initialization of value required for the item listbox
	std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");

	sf::FloatRect player_rect = scene.querySpriteRectAssociateWith("player", true);
	sf::Vector2f position = sf::Vector2f(player_rect.left + player_rect.width / 1.618f, player_rect.top - 0.05f);

	BagData* bagData = player.getData<BagData>(DATA_TYPE::BAG);

	////////////////////////////////////
	//	Delcaration of label buttons  //
	////////////////////////////////////

	std::vector<UI::LabelButton*> bttns;
	std::vector<PlantData> bagPlants = bagData->getPlants();
	std::vector<std::pair<BagData::Item, int>> bagItems = bagData->getItems();
	std::vector<std::pair<BagData::Gear, int>> bagGears = bagData->getGears();

	const int I_MAX = 1;
	const int J_MAX = 6;

	std::array<UI::LabelButton*, I_MAX* J_MAX> lbl_bbtns{nullptr};
	{
		int i = 0.f;
		int j = 0.f;

		for (auto& plant : bagPlants) {

			UI::LabelButton* curr = new UI::LabelButton("lbl_bttn_plant_" + plant.getId(),
				"media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf",
				sf::Vector2f(0.1f + i * 0.5f, j * 0.1f),
				plant.name,
				0.25f,
				sf::Color::White
			);

			///////////////////////////////////
			//	Connect label buttons event  //
			///////////////////////////////////

			// eat ////////////////////////////

			curr->connect(UI::Action::crossed, [curr]() {
				curr->setFontColor(sf::Color::White);
				});

			curr->connect(UI::Action::hover, [curr]() {
				if (curr)
					curr->setFontColor(sf::Color(255, 128, 0));
				});

			curr->connect(UI::Action::pressed, [&, plant]() {
				UIFactory factory(scene.getParent()->getParent()->peekState());
				std::shared_ptr<State> state = factory.createPlayerActionChoiceBox(player, scene, plant);
				scene.getParent()->getParent()->changeState(state, true);
				});

			lbl_bbtns[i + j * I_MAX] = curr;
			i++;
			if (i >= I_MAX) {
				i = 0;
				j++;
			}
			if (j >= J_MAX)
				break;
		}
	}
	for (int i = 0; i < I_MAX; i++)
		for (int j = 0; j < J_MAX; j++) {
			UI::LabelButton* curr{ nullptr };
			UI::LabelButton* left{ nullptr };
			UI::LabelButton* right{ nullptr };
			UI::LabelButton* up{ nullptr };
			UI::LabelButton* down{ nullptr };

			curr = lbl_bbtns[i + j * I_MAX];

			//right
			if (i + 1 < I_MAX)
				right = lbl_bbtns[(i + 1) + j * I_MAX];
			else
				right = lbl_bbtns[0 + j * I_MAX];

			//left
			if (i > 0)
				left = lbl_bbtns[(i - 1) + j * I_MAX];
			else
				left = lbl_bbtns[(I_MAX - 1) + j * I_MAX];
			

			//down
			if (j + 1 < J_MAX)
				down = lbl_bbtns[i + (j + 1) * I_MAX];
			else
				down = lbl_bbtns[i + 0 * I_MAX];

			//up
			if (j > 0)
				up = lbl_bbtns[i + (j - 1) * I_MAX];
			else
				up = lbl_bbtns[i + (J_MAX - 1) * I_MAX];
			

			if (curr) {
				curr->up = up;
				curr->down = down;
				curr->left = left;
				curr->right = right;
			}
		}

	for(auto& bttn: lbl_bbtns)
		bttns.push_back(bttn);

	//////////////////////////////////////////////////////////////////////
	// create or retrieve and update the listchoiceBoxState if it exist //
	//////////////////////////////////////////////////////////////////////

	std::shared_ptr<ChoiceBoxState> listChoiceBox{ nullptr };

	if (RessourceManager::StateManager::there_exists(playerListChoiceBoxId))
	{
		listChoiceBox = RessourceManager::StateManager::get<ChoiceBoxState>(playerListChoiceBoxId);
		listChoiceBox->setChoices(bttns);
	}
	else{

		listChoiceBox = RessourceManager::StateManager::create<ChoiceBoxState>(
			current->getParent(),
			current,
			position,
			sf::Vector2f(0.2, 0.3),
			*shader_param_ptr,
			bttns,
			sf::Color::White
		);

		playerListChoiceBoxId = listChoiceBox->getId();
	}

	return listChoiceBox;
}
