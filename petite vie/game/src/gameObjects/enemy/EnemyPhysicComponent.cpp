#include "EnemyPhysicComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <thsan/CollisionEngine/CollisionEngine.h>

EnemyPhysicComponent::EnemyPhysicComponent()
{
    // Constructor initialization
}

void EnemyPhysicComponent::init(Scene& scene)
{
    if (!parent->hasData(DATA_TYPE::PHYSIC_BODY)) {
        parent->setData(DATA_TYPE::PHYSIC_BODY, new physicBodyData());
    }

    if (!parent->hasData(DATA_TYPE::HITBOX)) {
        parent->setData(DATA_TYPE::HITBOX, new HitBox());
    }

    charState = nullptr;
    if (!parent->hasData(DATA_TYPE::CHARACTER_STAT)) {
        charState = new CharacterStatData();
        parent->setData(DATA_TYPE::CHARACTER_STAT, charState);
    }
    else {
        charState = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
    }

    scene.addToGroups(parent, { "enemy" });

    hitbox = parent->getData<HitBox>(DATA_TYPE::HITBOX);
    hitbox->position = glm::vec3(0, 0, 0);
    hitbox->spheres.push_back(Sphere{10.f, glm::vec3()});
    hitbox->collision_object = Collision_object::real;

    charState->atk = 5.f;
    charState->def = 1.f;
    charState->hp = 100.f;
    charState->curr.hp = 100;
    charState->speed = 15.f;

    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

    CollisionEngine::add(parent, temp, hitbox);

}

void EnemyPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
    Transform* transform = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
    physicBodyData* physics = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
    PlayerData* playerData = parent->getData<PlayerData>(DATA_TYPE::PLAYER);

    physics->friction = 0.95f; // si j'ai un masque, j'utiliserais le masque
    physics->masse = 0.29f;

    glm::vec3 direction = physics->direction;

    if (glm::length(direction) != 0.f) {
        direction = glm::normalize(direction);

        //acceleration
        velocity.x += direction.x * physics->force;
        velocity.z += direction.z * physics->force;

        // Cap velocity
        float max_speed = max_walking_speed;
        if (glm::length(velocity) > max_speed) 
        {
            velocity = glm::normalize(velocity) * max_speed;
        }
    }
    else 
    {
        // Apply friction
        velocity *= physics->friction;
    }

    // Check terrain height
    float currentHeight = transform->position.y;

    float groundHeight = scene.getPhysicalHeightOnPosition(sf::Vector2f(transform->position.x, transform->position.z));

    transform->position.y = groundHeight;
    

    // Apply force
    transform->position.x += velocity.x;
    transform->position.z += velocity.z;

    // Update physics force on reset la direction if force is negligible
    physics->force *= physics->friction;
    if (physics->force <= 0.05f) {
        physics->force = 0.f;
        physics->direction = glm::vec3(0, 0, 0);
    }

    if (charState->curr.hp <= 0)
    {
        scene.delete_child(parent->getName());
    }
}

EnemyPhysicComponent::~EnemyPhysicComponent()
{
    // Destructor cleanup
}
