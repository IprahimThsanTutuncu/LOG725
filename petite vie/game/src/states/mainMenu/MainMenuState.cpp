#include "MainMenuState.h"

#include <thsan/gameObject/component/NativeScriptComponent.h>

#include "gameObjects/camera/FollowCameraPhysicComponent.h"
#include "gameObjects/camera/CameraRenderComponent.h"

#include "gameObjects/player/PlayerInputComponent.h"
#include "gameObjects/player/PlayerPhysicComponent.h"
#include "gameObjects/player/PlayerSpriteRenderComponent.h"

#include "gameObjects/NPC/SimpleDialogNPCInputComponent.h"
#include "gameObjects/NPC/FixedOnGroundPhysicComponent.h"
#include "gameObjects/NPC/OrangeCuteRenderComponent.h"

#include "../dialogue/DialogueState.h"
#include "../choiceBox/ChoiceBoxState.h"

#include <thsan/state/transition/TransitionState.h>
#include <thsan/state/transition/transition/Transition.h>
#include <thsan/ressourceManager/StateManager.h>
#include <thsan/ressourceManager/ThemeManager.h>
#include <thsan/ressourceManager/SoundManager.h>
#include <thsan/ressourceManager/MusicManager.h>
#include <thsan/util/helper.h>

#include "gameObjects/bar/BarRenderComponent.h"
#include <states/test/TestMapState.h>
#include <states/map/Map0000.h>


MainMenuState::MainMenuState(Game* parent, std::string dialogue_path, int playAtParagrapheNum) :
	State(parent),
	begin_dialogue_path(dialogue_path),
	playAtParagrapheNum(playAtParagrapheNum)
{
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
                  << mode.width << "x" << mode.height << " - "
                  << mode.bitsPerPixel << " bpp" << std::endl;
    }
}

std::string MainMenuState::getType()
{
    return "mainMenu";
}

void MainMenuState::createSettingsMenu() {
        std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");
		std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());

		std::shared_ptr<DialogueState> dialogueSetting = RessourceManager::StateManager::create<DialogueState>(
			m_parent,
			curr_state,
			sf::Vector2f(0.45f, 0.20f),
			sf::Vector2f(0.4, 0.2),
			"media/data/dialogue/mainmenu_setting.txt",
			*shader_param_ptr,
			sf::Color::Red,
			0.3f,
			0.0f
		);

		std::vector<UI::LabelButton*> bttnsSetting;
		UI::LabelButton* lbl_bttn_music = new UI::LabelButton("lbl_bttn_music", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.15), "Music", 0.75f, sf::Color::White);
		UI::LabelButton* lbl_bttn_sound = new UI::LabelButton("lbl_bttn_sound", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, -0.15), "Sound", 0.75f, sf::Color::White);
			UI::LabelButton* lbl_bttn_menu = new UI::LabelButton("lbl_bttn_menu", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.13, 0.65), "Back to menu", 0.95f, sf::Color::White);

		lbl_bttn_music->left = lbl_bttn_sound;
		lbl_bttn_music->right = lbl_bttn_sound;

		lbl_bttn_sound->left = lbl_bttn_music;
		lbl_bttn_sound->right = lbl_bttn_music;

		lbl_bttn_menu->up = lbl_bttn_music;
        lbl_bttn_sound->down = lbl_bttn_menu;
        lbl_bttn_music->down = lbl_bttn_menu;

		/* TODO
			EDIT: ACCIDENTAL CAPS LOCK

			HERE IS BUTTON FOR MUSIC AND SOUND
			ADD ANOTHER CALLED "NOTHING" QUI RAM�NE AU MAINMEMU, MUSIC ET SOUND RAMENE AU MAINMENU EN CE MOMENT

		*/
		lbl_bttn_music->connect(UI::Action::pressed, [&]()
			{
				std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());

				std::shared_ptr<MainMenuState> main_menu_state = std::dynamic_pointer_cast<MainMenuState>(curr_state);

				if (main_menu_state)
				{
					main_menu_state->playAtParagrapheNum = 0;
					main_menu_state->begin_dialogue_path = "media/data/dialogue/mainmenu.txt";
				}
				RessourceManager::MusicManager::setVolume(25);
				std::cout << "Music volume set to 25." << std::endl;
				//m_parent->changeState(curr_state, true);
				createMusicMenu();

			});

		lbl_bttn_sound->connect(UI::Action::pressed, [&]()
			{
			std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());
			std::shared_ptr<MainMenuState> main_menu_state = std::dynamic_pointer_cast<MainMenuState>(curr_state);

			if (main_menu_state)
			{
				main_menu_state->playAtParagrapheNum = 0;
				main_menu_state->begin_dialogue_path = "media/data/dialogue/mainmenu.txt";
			}
			RessourceManager::SoundManager::setVolume(100);
			std::cout << "Sound volume set to 100." << std::endl;
			//m_parent->changeState(curr_state, true);
            createSoundMenu();
			});

		lbl_bttn_menu->connect(UI::Action::pressed, [&]()
        	{
        	std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());
        	m_parent->changeState(curr_state, true);
        });

		lbl_bttn_music->connect(UI::Action::crossed, [lbl_bttn_music]() {lbl_bttn_music->setFontColor(sf::Color::White); });
		lbl_bttn_music->connect(UI::Action::hover, [lbl_bttn_music]() {lbl_bttn_music->setFontColor(sf::Color::Red); });

		lbl_bttn_sound->connect(UI::Action::crossed, [lbl_bttn_sound]() {lbl_bttn_sound->setFontColor(sf::Color::White); });
		lbl_bttn_sound->connect(UI::Action::hover, [lbl_bttn_sound]() {lbl_bttn_sound->setFontColor(sf::Color::Red); });

		lbl_bttn_menu->connect(UI::Action::crossed, [lbl_bttn_menu]() {lbl_bttn_menu->setFontColor(sf::Color::White); });
        	lbl_bttn_menu->connect(UI::Action::hover, [lbl_bttn_menu]() {lbl_bttn_menu->setFontColor(sf::Color::Magenta); });

        bttnsSetting = { lbl_bttn_music, lbl_bttn_sound , lbl_bttn_menu };

		dialogueSetting->addChoiceBox(bttnsSetting, 0, sf::Vector2f(0.4f, 0.35f), sf::Vector2f(0.3, 0.1));

		m_parent->changeState(dialogueSetting, true);
}

void MainMenuState::createMusicMenu() {
	std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");
	std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());

	std::shared_ptr<DialogueState> dialogueMusic = RessourceManager::StateManager::create<DialogueState>(
		m_parent,
		curr_state,
		sf::Vector2f(0.45f, 0.20f),
		sf::Vector2f(0.4, 0.2),
		"media/data/dialogue/music_mainmenu_setting.txt",
		*shader_param_ptr,
		sf::Color::Red,
		0.3f,
		0.0f
	);
	UI::LabelButton* lbl_music_up = new UI::LabelButton("lbl_music_up", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.65), "More", 0.75f, sf::Color::White);
	UI::LabelButton* lbl_music_down = new UI::LabelButton("lbl_music_down", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.15), "Less", 0.75f, sf::Color::White);
	UI::LabelButton* lbl_menu = new UI::LabelButton("lbl_menu", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, 0.45), "Back to setting", 0.75f, sf::Color::White);

	lbl_music_up->up = lbl_menu;
	lbl_music_up->down = lbl_music_down;

	lbl_music_down->up = lbl_music_up;
	lbl_music_down->down = lbl_menu;

	lbl_menu->down = lbl_music_up;
	lbl_menu->up = lbl_music_down;

	lbl_music_up->connect(UI::Action::pressed, [&]()
		{
			float volume = RessourceManager::MusicManager::getVolume();
			std::cout << "Music volume is " << volume << "." << std::endl;

			volume += 10;

			if (volume > 100)
				volume = 100;

			RessourceManager::MusicManager::setVolume(volume);

			std::cout << "Music volume now set to " << volume << "." << std::endl;

			createMusicMenu();

		});


	lbl_music_down->connect(UI::Action::pressed, [&]()
		{
			float volume = RessourceManager::MusicManager::getVolume();
			std::cout << "Music volume set to " << volume << "." << std::endl;

			volume -= 10;

			if (volume <= 0)
				volume = 0;

			RessourceManager::MusicManager::setVolume(volume);

			std::cout << "Music volume set to " << volume << "." << std::endl;

			createMusicMenu();

		});

	lbl_menu->connect(UI::Action::pressed, [&]()
		{
			createSettingsMenu();
		});


	lbl_music_up->connect(UI::Action::crossed, [lbl_music_up]() {lbl_music_up->setFontColor(sf::Color::White); });
	lbl_music_up->connect(UI::Action::hover, [lbl_music_up]() {lbl_music_up->setFontColor(sf::Color::Magenta); });

	lbl_music_down->connect(UI::Action::crossed, [lbl_music_down]() {lbl_music_down->setFontColor(sf::Color::White); });
	lbl_music_down->connect(UI::Action::hover, [lbl_music_down]() {lbl_music_down->setFontColor(sf::Color::Magenta); });

	lbl_menu->connect(UI::Action::crossed, [lbl_menu]() {lbl_menu->setFontColor(sf::Color::White); });
	lbl_menu->connect(UI::Action::hover, [lbl_menu]() {lbl_menu->setFontColor(sf::Color::Magenta); });


	std::vector<UI::LabelButton*> bttnsSettingMusicSound = { lbl_music_up, lbl_music_down , lbl_menu };

	dialogueMusic->addChoiceBox(bttnsSettingMusicSound, 0, sf::Vector2f(0.4f, 0.35f), sf::Vector2f(0.5, 0.1));

	m_parent->changeState(dialogueMusic, true);
}

void MainMenuState::createSoundMenu() {
	std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");
	std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());

	std::shared_ptr<DialogueState> dialogueSound = RessourceManager::StateManager::create<DialogueState>(
		m_parent,
		curr_state,
		sf::Vector2f(0.45f, 0.20f),
		sf::Vector2f(0.4, 0.2),
		"media/data/dialogue/sound_mainmenu_setting.txt",
		*shader_param_ptr,
		sf::Color::Red,
		0.3f,
		0.0f
	);
	UI::LabelButton* lbl_music_up = new UI::LabelButton("lbl_music_up", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.65), "More", 0.75f, sf::Color::White);
	UI::LabelButton* lbl_music_down = new UI::LabelButton("lbl_music_down", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.15), "Less", 0.75f, sf::Color::White);
	UI::LabelButton* lbl_menu = new UI::LabelButton("lbl_menu", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, 0.45), "Back to setting", 0.75f, sf::Color::White);

	lbl_music_up->up = lbl_menu;
	lbl_music_up->down = lbl_music_down;

	lbl_music_down->up = lbl_music_up;
	lbl_music_down->down = lbl_menu;

	lbl_menu->down = lbl_music_up;
	lbl_menu->up = lbl_music_down;

	lbl_music_up->connect(UI::Action::pressed, [&]()
		{
			float volume = RessourceManager::SoundManager::getVolume();
			std::cout << "Sound volume is " << volume << "." << std::endl;

			volume += 10;

			if (volume > 100)
				volume = 100;

			RessourceManager::SoundManager::setVolume(volume);

			std::cout << "Sound volume now set to " << volume << "." << std::endl;

			createSoundMenu();
		});


	lbl_music_down->connect(UI::Action::pressed, [&]()
		{
			float volume = RessourceManager::SoundManager::getVolume();
			std::cout << "Sound volume set to " << volume << "." << std::endl;

			volume -= 10;

			if (volume <= 0)
				volume = 0;

			RessourceManager::SoundManager::setVolume(volume);

			std::cout << "Sound volume set to " << volume << "." << std::endl;

			createSoundMenu();
		});

	lbl_menu->connect(UI::Action::pressed, [&]()
		{
			createSettingsMenu();
		});


	lbl_music_up->connect(UI::Action::crossed, [lbl_music_up]() {lbl_music_up->setFontColor(sf::Color::White); });
	lbl_music_up->connect(UI::Action::hover, [lbl_music_up]() {lbl_music_up->setFontColor(sf::Color::Magenta); });

	lbl_music_down->connect(UI::Action::crossed, [lbl_music_down]() {lbl_music_down->setFontColor(sf::Color::White); });
	lbl_music_down->connect(UI::Action::hover, [lbl_music_down]() {lbl_music_down->setFontColor(sf::Color::Magenta); });

	lbl_menu->connect(UI::Action::crossed, [lbl_menu]() {lbl_menu->setFontColor(sf::Color::White); });
	lbl_menu->connect(UI::Action::hover, [lbl_menu]() {lbl_menu->setFontColor(sf::Color::Magenta); });


	std::vector<UI::LabelButton*> bttnsSettingMusicSound = { lbl_music_up, lbl_music_down , lbl_menu };

	dialogueSound->addChoiceBox(bttnsSettingMusicSound, 0, sf::Vector2f(0.4f, 0.35f), sf::Vector2f(0.5, 0.1));

	m_parent->changeState(dialogueSound, true);
}

void MainMenuState::init()
{
	m_scene = new Scene(this, "media/data/map/shop-1.json", VIEW_TYPE::DYNAMIC_VIEW);

	//RessourceManager::SoundManager::setVolume(50);
	RessourceManager::MusicManager::stop();
	//RessourceManager::MusicManager::setVolume(25);
	RessourceManager::MusicManager::play("media/music/shop.mp3");

	UI::ShaderUIparam shader_param;
	shader_param.borderSize[0] = 0.25f;
	shader_param.borderSize[1] = 0.25f;
	shader_param.borderSize[2] = 0.25f;
	shader_param.borderSize[3] = 0.25f;
	shader_param.colorBg[0] = sf::Color(0, 0, 168);
	shader_param.colorBg[1] = sf::Color(0, 0, 128);
	shader_param.colorBg[2] = sf::Color::Black;
	shader_param.colorBg[3] = sf::Color(0, 0, 128);
	shader_param.colorBorder = sf::Color(220, 220, 220);
	shader_param.colorBorderBorders = sf::Color(255, 0, 0, 128);
	shader_param.cornerRadius[0] = 0.05f;
	shader_param.cornerRadius[1] = 0.05f;
	shader_param.cornerRadius[2] = 0.05f;
	shader_param.cornerRadius[3] = 0.05f;
	shader_param.type = UI::Type::fill;
	shader_param.enableImage = true;

	std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");

	std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());
	dialogue = RessourceManager::StateManager::create<DialogueState>(m_parent, curr_state, sf::Vector2f(0.45f, 0.20f), sf::Vector2f(0.4, 0.2), begin_dialogue_path, *shader_param_ptr, sf::Color::Red, 0.3f, 0.0f);

	id_dialogue = dialogue->getId();

	std::vector<UI::LabelButton*> bttns;
	UI::LabelButton* lbl_bttn_1 = new UI::LabelButton("lbl_bttn_1", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.15), "Play", 0.75f, sf::Color::White);
	UI::LabelButton* lbl_bttn_2 = new UI::LabelButton("lbl_bttn_2", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, -0.15), "Setting", 0.75f, sf::Color::White);
	lbl_bttn_1->left = lbl_bttn_2;
	lbl_bttn_1->right = lbl_bttn_2;

	lbl_bttn_2->left = lbl_bttn_1;
	lbl_bttn_2->right = lbl_bttn_1;

	/*	TODO

		for yulia:
		->add a setting window which will ask if you want to modify sound or music
			note:there's a dialogue in media/data/dialogue/mainmenu_setting.txt for the shopkeeper

		le sound mixer faut que tu le fasse toi m�me dans un
		pour le sound y'a "RessourceManager::SoundManager::setVolume(100);"
		pour la music y'a "RessourceManager::MusicManager::setVolume(25);"

		Faudra faire un "std::shared_ptr<DialogueState> dialogueSetting = RessourceManager::StateManager::create<DialogueState>(...);
"			lbl_bttn_2->connect(UI::Action::pressed, [&]() { m_parent->changeState(dialogueSetting, true); });

		sois cr�atif I guess pour le UI

		aaah, also, les position sont tous �tre 0 et 1, c'�est relatif au panel parent.
		si t'as un panel a position x 0.5, bah c'est 50% de l'ecran.
		Si ton text est a 0.25, bien c'est size du panel parent multiplier par 0.25
		confusing to work with, I hate it. Dans le new engine ca existe pas.

	*/

	lbl_bttn_2->connect(UI::Action::pressed, [&]()
		{
		createSettingsMenu();
	});

	lbl_bttn_1->connect(UI::Action::crossed, [lbl_bttn_1]() {lbl_bttn_1->setFontColor(sf::Color::White); });

	lbl_bttn_1->connect(UI::Action::hover, [lbl_bttn_1]() {
		if (lbl_bttn_1)
			lbl_bttn_1->setFontColor(sf::Color::Cyan);
		});


	lbl_bttn_1->connect(UI::Action::pressed, [&]() {
		startGameState = true;
	});

	lbl_bttn_2->connect(UI::Action::crossed, [lbl_bttn_2]() {lbl_bttn_2->setFontColor(sf::Color::White); });
	lbl_bttn_2->connect(UI::Action::hover, [lbl_bttn_2]() {lbl_bttn_2->setFontColor(sf::Color::Red); });

	bttns.push_back(lbl_bttn_1);
	bttns.push_back(lbl_bttn_2);

	dialogue->addChoiceBox(bttns, playAtParagrapheNum, sf::Vector2f(0.4f, 0.35f), sf::Vector2f(0.3, 0.1));

	std::shared_ptr<PostProcess> pp = m_scene->getPostProcess();
	pp->disable();
	pp->snowfall_param.bg_layers = 2.f;
	pp->snowfall_param.fg_layers = 2.f;
	pp->snowfall_param.layer_gap = 1.f;
	pp->snowfall_param.size_rate = 0.1f;
	pp->snowfall_param.wind_direction = sf::Glsl::Vec2(0.2, 0.4);

	pp->enable_effect(PostProcess::Effect::snowfall);
	pp->bind_effect_to_layer(PostProcess::Effect::snowfall, 0);

	m_parent->changeState(dialogue, true);
}

void MainMenuState::start()
{
}


void MainMenuState::handleEvent(sf::Event event)
{

}

void MainMenuState::input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions)
{
	m_scene->input(deltaTime, inputActions);


	if (startGameState == true)
	{
		std::shared_ptr<State> peek = this->getParent()->peekState();

		sf::Vector2u resolution = sf::Vector2u(this->getParent()->getWidth(), this->getParent()->getHeight());
		//PostProcess::Letterbox_param::Bar, sf::Color, Ease, InterpolationType
		std::unique_ptr<Transition> transition = std::make_unique<Transition>(
			sf::seconds(1.0f),
			sf::seconds(2.0f),
			Transition::Type::fade_to_color,
			Transition::Type::fade_to_color,
			Transition::Fade_type::step,
			Transition::Fade_type::step,
			sf::Color::Black,
			sf::Color::Cyan,
			sf::Color::Black,
			sf::Color::Black
		);

		auto current = RessourceManager::StateManager::get<State>(this->getId());
		std::shared_ptr<TransitionState<Map0000>> interpolate = RessourceManager::StateManager::create_transition<Map0000>(this->getParent(), current, transition);

		this->getParent()->changeState(interpolate, true);
		startGameState = false;

	}
}

void MainMenuState::update(const sf::Time &deltaTime)
{
    m_scene->update(deltaTime);
    static float t = 0;
    t += deltaTime.asSeconds();
}

void MainMenuState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    m_scene->setRenderTarget(&target);
    LightProperties dir = m_scene->getDirectionalLightProperties();
    target.clear(dir.ambient);
    m_scene->render(deltaTime);
}



