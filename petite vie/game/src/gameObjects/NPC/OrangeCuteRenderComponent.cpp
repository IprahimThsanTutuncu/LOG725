#include "OrangeCuteRenderComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <thsan/ressourceManager/ShaderManager.h>

#include <SFML/Graphics/Glsl.hpp>
#include <glm/glm.hpp>

OrangeCuteRenderComponent::OrangeCuteRenderComponent():
    orangeCuteSprite("media/image/character/cutebonhomme.png")
{

}

void OrangeCuteRenderComponent::init(Scene& scene)
{
    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
    temp->scale.x = 0.15;
    temp->scale.y = 0.15;

    orangeCuteSprite.setShininess(32.f);
    orangeCuteSprite.setKeyColor(sf::Color(0, 0, 0, 0));
    orangeCuteSprite.setCurrAnimation("idle");
    orangeCuteSprite.addFrame("idle", sf::IntRect(20, 0, 56, 89), sf::seconds(0.1f));
    orangeCuteSprite.addFrame("idle", sf::IntRect(109, 0, 56, 89), sf::seconds(0.1f));
    orangeCuteSprite.addFrame("idle", sf::IntRect(198, 0, 56, 89), sf::seconds(0.1f));
    orangeCuteSprite.addFrame("idle", sf::IntRect(288, 0, 56, 89), sf::seconds(0.1f));
}

void OrangeCuteRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer)
{

    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
	
	static float t = 0;
	t += dt.asSeconds();

    orangeCuteSprite.update(dt);
    renderer.add(&orangeCuteSprite, sf::Vector3f(temp->position.x, temp->position.y, temp->position.z), sf::Vector2f(temp->scale.x, temp->scale.y), 0);
    renderer.trackSpriteWithTag(&orangeCuteSprite, parent->getName());
   
}


OrangeCuteRenderComponent::~OrangeCuteRenderComponent()
{

}
