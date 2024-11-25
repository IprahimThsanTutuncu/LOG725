#pragma once

#include <thsan/gameObject/component/RenderComponent.h>
#include <thsan/graphic/SpriteAnimation.h>

class HealthBarRenderComponent : public RenderComponent
{
public:
    HealthBarRenderComponent(std::string target, sf::Color color);

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer) override;

    ~HealthBarRenderComponent() = default;
private:
    std::string targetName;
    SpriteAnimation background;
    SpriteAnimation foreground;
    sf::Color bg_color;
    sf::Color fg_color;

    float sprite_height{ 0.f };
};

