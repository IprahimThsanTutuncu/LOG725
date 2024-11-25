#include "EnemyRenderComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <thsan/ressourceManager/ShaderManager.h>
#include <SFML/Graphics/Glsl.hpp>
#include <glm/glm.hpp>

#include "gameObjects/healthbar/HealthBarRenderComponent.h"

EnemyRenderComponent::EnemyRenderComponent() :
    enemySprite("media/image/character/motobug.png")
{

}

void EnemyRenderComponent::init(Scene& scene)
{
    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
    temp->scale.x = 0.15;
    temp->scale.y = 0.15;

    enemySprite.setShininess(32.f);
    enemySprite.setKeyColor(sf::Color(255, 0, 255, 255));
    enemySprite.setCurrAnimation("idle");
    enemySprite.addFrame("idle", sf::IntRect(282, 57, 39, 28), sf::seconds(0.1f));
    enemySprite.addFrame("idle", sf::IntRect(323, 57, 39, 28), sf::seconds(0.1f));
    enemySprite.addFrame("idle", sf::IntRect(364, 56, 39, 28), sf::seconds(0.1f));
    enemySprite.addFrame("idle", sf::IntRect(405, 56, 39, 28), sf::seconds(0.1f));

    enemySprite.setColor(sf::Color(0, 255, 255));

    healthBar = scene.createGameObject("healthbar_" + parent->getName());

    sf::Color orange(255, 165, 0);
    scene.setGameObjectRenderComponent<HealthBarRenderComponent>(healthBar, parent->getName(), orange);
    
}

void EnemyRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer)
{

    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

    static float t = 0;
    t += dt.asSeconds();

    enemySprite.update(dt);
    auto p = temp->position;

    if (p.x > renderer.getHalfSize().x) {
        p.x = renderer.getHalfSize().x - 1;
    }
    else if (p.x < -renderer.getHalfSize().x) 
    {
        p.x = -renderer.getHalfSize().x + 1; 
    }

    if (p.y > renderer.getHalfSize().y) 
    {
        p.y = renderer.getHalfSize().y - 1;
    }
    else if (p.y < -renderer.getHalfSize().y)
    {
        p.y = -renderer.getHalfSize().y + 1;
    }

    renderer.add(&enemySprite, sf::Vector3f(temp->position.x, temp->position.y, temp->position.z), sf::Vector2f(temp->scale.x * 4.f, temp->scale.y * 4.f), 0);
    renderer.trackSpriteWithTag(&enemySprite, parent->getName());
}


EnemyRenderComponent::~EnemyRenderComponent()
{

}
