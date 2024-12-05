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


MainMenuState::MainMenuState(Game* parent) :
	State(parent)
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

void MainMenuState::init()
{
	m_scene = new Scene(this, "media/data/map/shop-1.json", VIEW_TYPE::DYNAMIC_VIEW);

	RessourceManager::SoundManager::setVolume(50);
	RessourceManager::MusicManager::stop();
	RessourceManager::MusicManager::setVolume(25);
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

	dialogue = RessourceManager::StateManager::create<DialogueState>(m_parent, curr_state, sf::Vector2f(0.45f, 0.20f), sf::Vector2f(0.4, 0.2), "media/data/dialogue/mainmenu.txt", *shader_param_ptr, sf::Color::Red, 0.3f, 0.0f);

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

		le sound mixer faut que tu le fasse toi même dans un
		pour le sound y'a "RessourceManager::SoundManager::setVolume(100);"
		pour la music y'a "RessourceManager::MusicManager::setVolume(25);"

		Faudra faire un "std::shared_ptr<DialogueState> dialogueSetting = RessourceManager::StateManager::create<DialogueState>(...);
"			lbl_bttn_2->connect(UI::Action::pressed, [&]() { m_parent->changeState(dialogueSetting, true); });

		sois créatif I guess pour le UI

		aaah, also, les position sont tous être 0 et 1, c'éest relatif au panel parent.
		si t'as un panel a position x 0.5, bah c'est 50% de l'ecran.
		Si ton text est a 0.25, bien c'est size du panel parent multiplier par 0.25
		confusing to work with, I hate it. Dans le new engine ca existe pas.

	*/

	lbl_bttn_2->connect(UI::Action::pressed, [&]()
		{
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
		lbl_bttn_music->left = lbl_bttn_sound;
		lbl_bttn_music->right = lbl_bttn_sound;

		lbl_bttn_sound->left = lbl_bttn_music;
		lbl_bttn_sound->right = lbl_bttn_music;

		/* TODO 
			EDIT: ACCIDENTAL CAPS LOCK

			HERE IS BUTTON FOR MUSIC AND SOUND
			ADD ANOTHER CALLED "NOTHING" QUI RAMÈNE AU MAINMEMU, MUSIC ET SOUND RAMENE AU MAINMENU EN CE MOMENT
		
		*/
		lbl_bttn_music->connect(UI::Action::pressed, [&]()
			{
				std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());

				RessourceManager::MusicManager::setVolume(25);
				std::cout << "Music volume set to 25." << std::endl;
				m_parent->changeState(curr_state, true);

			});

		lbl_bttn_sound->connect(UI::Action::pressed, [&]() 
			{
			std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());

			RessourceManager::SoundManager::setVolume(100);
			std::cout << "Sound volume set to 100." << std::endl;
			m_parent->changeState(curr_state, true);

			});

		lbl_bttn_music->connect(UI::Action::crossed, [lbl_bttn_music]() {lbl_bttn_music->setFontColor(sf::Color::White); });
		lbl_bttn_music->connect(UI::Action::hover, [lbl_bttn_music]() {lbl_bttn_music->setFontColor(sf::Color::Red); });

		lbl_bttn_sound->connect(UI::Action::crossed, [lbl_bttn_sound]() {lbl_bttn_sound->setFontColor(sf::Color::White); });
		lbl_bttn_sound->connect(UI::Action::hover, [lbl_bttn_sound]() {lbl_bttn_sound->setFontColor(sf::Color::Red); });

		bttnsSetting = { lbl_bttn_music, lbl_bttn_sound };
		dialogueSetting->addChoiceBox(bttnsSetting, 0, sf::Vector2f(0.4f, 0.35f), sf::Vector2f(0.3, 0.1));

		m_parent->changeState(dialogueSetting, true);
	});

	lbl_bttn_1->connect(UI::Action::crossed, [lbl_bttn_1]() {lbl_bttn_1->setFontColor(sf::Color::White); });

	lbl_bttn_1->connect(UI::Action::hover, [lbl_bttn_1]() {
		if (lbl_bttn_1)
			lbl_bttn_1->setFontColor(sf::Color::Cyan);
		});


	lbl_bttn_1->connect(UI::Action::pressed, [&]() {
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
		std::shared_ptr<TransitionState<TestMapState>> interpolate = RessourceManager::StateManager::create_transition<TestMapState>(this->getParent(), current, transition);

		this->getParent()->changeState(interpolate, true);
	});

	lbl_bttn_2->connect(UI::Action::crossed, [lbl_bttn_2]() {lbl_bttn_2->setFontColor(sf::Color::White); });
	lbl_bttn_2->connect(UI::Action::hover, [lbl_bttn_2]() {lbl_bttn_2->setFontColor(sf::Color::Red); });

	bttns.push_back(lbl_bttn_1);
	bttns.push_back(lbl_bttn_2);

	dialogue->addChoiceBox(bttns, 0, sf::Vector2f(0.4f, 0.35f), sf::Vector2f(0.3, 0.1));

	std::shared_ptr<PostProcess> pp = m_scene->getPostProcess();
	pp->disable();
	pp->snowfall_param.bg_layers = 2.f;
	pp->snowfall_param.fg_layers = 2.f;
	pp->snowfall_param.layer_gap = 1.f;
	pp->snowfall_param.size_rate = 0.1f;
	pp->snowfall_param.wind_direction = sf::Glsl::Vec2(0.2, 0.4);

	pp->enable_effect(PostProcess::Effect::snowfall);
	pp->bind_effect_to_layer(PostProcess::Effect::snowfall, 0);

}

void MainMenuState::start()
{
	m_parent->changeState(dialogue, true);
}


void MainMenuState::handleEvent(sf::Event event)
{

}

void MainMenuState::input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions)
{
	m_scene->input(deltaTime, inputActions);

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



