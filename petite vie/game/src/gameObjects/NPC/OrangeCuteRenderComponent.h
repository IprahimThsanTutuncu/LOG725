#pragma once

#include <thsan/gameObject/component/RenderComponent.h>
#include <thsan/graphic/SpriteAnimation.h>

class OrangeCuteRenderComponent : public RenderComponent
{
    public:
        OrangeCuteRenderComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, sf::RenderTarget &target, RendererPseudo3D& renderer) override;

        ~OrangeCuteRenderComponent();
    private:
        SpriteAnimation orangeCuteSprite;
};

