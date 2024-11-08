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

	std::shared_ptr<DialogueState> dialogue = RessourceManager::StateManager::create<DialogueState>(m_parent, curr_state, sf::Vector2f(0.45f, 0.20f), sf::Vector2f(0.4, 0.2), "media/data/dialogue/mainmenu.txt", *shader_param_ptr, sf::Color::Red, 0.3f, 0.0f);

	std::vector<UI::LabelButton*> bttns;
	UI::LabelButton* lbl_bttn_1 = new UI::LabelButton("lbl_bttn_1", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.15), "Play", 0.75f, sf::Color::White);
	UI::LabelButton* lbl_bttn_2 = new UI::LabelButton("lbl_bttn_2", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, -0.15), "Setting", 0.75f, sf::Color::White);
	lbl_bttn_1->left = lbl_bttn_2;
	lbl_bttn_1->right = lbl_bttn_2;

	lbl_bttn_2->left = lbl_bttn_1;
	lbl_bttn_2->right = lbl_bttn_1;

	lbl_bttn_2->connect(UI::Action::pressed, [&]() { /*SHOW LES SETTINGS*/ });
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
	m_parent->changeState(dialogue, true);

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



