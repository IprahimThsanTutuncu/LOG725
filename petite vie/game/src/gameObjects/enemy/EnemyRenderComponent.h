#pragma once

#include <thsan/gameObject/component/RenderComponent.h>
#include <thsan/graphic/SpriteAnimation.h>

class EnemyRenderComponent : public RenderComponent
{
public:
    EnemyRenderComponent();

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer) override;

    ~EnemyRenderComponent();
private:
    SpriteAnimation enemySprite;
};

