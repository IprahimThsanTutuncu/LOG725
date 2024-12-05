#pragma once

#include <Thsan.h>

class DialogueState;

class MainMenuState : public State
{
    public:
        MainMenuState(Game* parent);

        std::string getType() override;
        void init() override;
        void start() override;
        void handleEvent(sf::Event event) override;
        void input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions) override;
        void update(const sf::Time &deltaTime) override;
        void draw(sf::RenderTarget& target, const sf::Time &deltaTime) override;

        ~MainMenuState() = default;

    private:
        Scene* m_scene;

        std::string id_dialogue;
        std::string id_dialogue_setting;

        std::shared_ptr<DialogueState> dialogue;
};
