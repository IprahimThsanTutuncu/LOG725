#include "FixedOnGroundPhysicComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <glm/glm.hpp>

FixedOnGroundPhysicComponent::FixedOnGroundPhysicComponent()
{
    //ctor
}

void FixedOnGroundPhysicComponent::init(Scene& scene)
{
    if (!parent->hasData(DATA_TYPE::HITBOX))
        parent->setData(DATA_TYPE::HITBOX, new HitBox());

    CharacterStatData* charState{ nullptr };
    if (!parent->hasData(DATA_TYPE::CHARACTER_STAT)) {
        charState = new CharacterStatData();
        parent->setData(DATA_TYPE::CHARACTER_STAT, charState);
    }
    else {
        charState = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
    }

    scene.addToGroups(parent, { "proud fighter" });

    hitbox = parent->getData<HitBox>(DATA_TYPE::HITBOX);
    hitbox->position = glm::vec3(0, 0, 0);
    hitbox->collision_object = Collision_object::real;

    charState->atk = 5.f;
    charState->def = 1.f;
    charState->hp = 2.f;
    charState->speed = 15.f;
}


void FixedOnGroundPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
	temp->position.y = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));
}

FixedOnGroundPhysicComponent::~FixedOnGroundPhysicComponent()
{
    //dtor
}
