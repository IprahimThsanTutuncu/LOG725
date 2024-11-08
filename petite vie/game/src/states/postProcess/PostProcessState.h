#pragma once
#include <Thsan.h>

#include <iostream>

class PostProcessState : public State
{
public:
    PostProcessState(Game* parent, std::shared_ptr<State> previous, Scene* scene);

    std::string getType() override;
    void init() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    ~PostProcessState() = default;

private:

    sf::RenderTexture texture;
    sf::Texture frame_texture;
    sf::RenderWindow* window;
    std::shared_ptr<State> target_state;
    Scene* scene;
    sf::Sprite s;
    std::shared_ptr<sf::Shader> shader_pp;
};

