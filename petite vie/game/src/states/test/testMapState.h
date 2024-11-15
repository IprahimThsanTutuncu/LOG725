#pragma once

#include <Thsan.h>

class TestMapState : public State
{
public:
    TestMapState(Game* parent);
    std::string getType() override;
    void init() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    ~TestMapState() = default;

private:
    bool isInit{ false };
    Scene scene;
    std::shared_ptr<PostProcess> pp;
    GameObject* go_volume_pp;

};

