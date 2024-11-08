#pragma once

#pragma once

#include <thsan/state/State.h>
#include <thsan/UI/Gui.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace UI {
	struct ShaderUIparam;

	class Panel;
	class LabelButton;
	class CustomButton;
}

class ChoiceBoxState;

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class ChoiceBoxState : public State
{
public:
	ChoiceBoxState(Game* parent, std::shared_ptr<State> previous, sf::Vector2f position, sf::Vector2f size, UI::ShaderUIparam shaderParam, std::vector<UI::LabelButton*> choices, sf::Color textColor = sf::Color::White, bool parent_is_checkbox_to_be_removed = false);
	
	std::string getType() override;
	void init() override;
	void handleEvent(sf::Event event) override;
	void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
	void update(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

	void setChoices(std::vector<UI::LabelButton*> choices);

	~ChoiceBoxState();

private:
	bool parent_is_checkbox_to_be_removed{false};
	UI::Gui gui;
	std::vector<UI::LabelButton*> choices;
	UI::Panel* choicesPanel;

	sf::Color defaultTextColor;
	std::shared_ptr<State> previous;
};

