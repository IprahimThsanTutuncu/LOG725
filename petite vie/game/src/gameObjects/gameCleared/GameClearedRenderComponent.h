#pragma once

#include <thsan/gameObject/component/RenderComponent.h>
#include <thsan/graphic/SpriteAnimation.h>

class GameClearedRenderComponent : public RenderComponent
{
public:
    GameClearedRenderComponent();

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer) override;

    ~GameClearedRenderComponent() = default;
private:
    std::shared_ptr<SpriteAnimation> ssbm_fx;

    float sprite_height{ 0.f };
};

#pragma once
