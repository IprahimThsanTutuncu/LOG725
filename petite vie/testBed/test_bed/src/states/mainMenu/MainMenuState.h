#pragma once

#include <Thsan.h>

class MainMenuState : public State
{
    public:
        MainMenuState(Game* parent);
        void init() override;
        void handleEvent(sf::Event event) override;
        void input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions) override;
        void update(const sf::Time &deltaTime) override;
        void draw(sf::RenderTarget& target, const sf::Time &deltaTime) override;

        ~MainMenuState() = default;

    private:

        Scene* m_scene;
};
