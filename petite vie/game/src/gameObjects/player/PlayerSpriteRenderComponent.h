#pragma once

#include <thsan/gameObject/component/RenderComponent.h>
#include <thsan/GameObject/data/DataType.h>
#include <thsan/graphic/SpriteAnimation.h>
#include <thsan/graphic/light/PointLight.h>

#include <SFML/System/Time.hpp>

class Scene;
class GameObject;
class RendererPseudo3D;

class PlayerSpriteRenderComponent : public RenderComponent
{
    public:
        PlayerSpriteRenderComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, sf::RenderTarget &target, RendererPseudo3D& renderer) override;

        ~PlayerSpriteRenderComponent() = default;
    private:
        glm::vec3 calculateVelocity(glm::vec3 position, glm::vec3 target, float speed);
        SpriteAnimation playerSprite;
        std::shared_ptr<SpriteAnimation> slashSprite;
        std::shared_ptr<SpriteAnimation> plantSprite;

        SpriteAnimation sparkleSprite;

        glm::vec3 sparklePosition;

		bool isLookingLeft;
        bool isCollisionAdded{ false };
        HitBox attack_hitbox_start;
        HitBox attack_hitbox_mid;
        HitBox player_hitbox;

        Transform* player_transform;
};

