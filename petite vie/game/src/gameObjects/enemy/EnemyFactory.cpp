#include "EnemyFactory.h"

#include <thsan/gameObject/component/NativeScriptComponent.h>
#include <thsan/gameObject/data/DataHolder.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/scene/Scene.h>

#include "EnemyPhysicComponent.h"
#include "EnemyRenderComponent.h"


/***********************************************
 * BasicEnemyController
 * ---------------------------------------------
 * Desc: L'ennemi se d�place dans une direction
 *       al�atoire et r��value cette direction
 *       selon trois conditions :
 *       1. Sortie du rayon d�fini.
 *       2. Altitude trop �lev�e.
 *       3. Temps �coul� trop long.
 ***********************************************/

class BasicEnemyController : public NativeScriptComponent {
public:
    static constexpr float Speed = 0.3f;                  
    static constexpr float Radius = 200.f;                
    static constexpr float ChangeDirectionInterval = 2.0f;
    static constexpr float ElevationLimit = 90.0f;

    glm::vec3 targetDirection;
    glm::vec3 origin;
    bool hasOrigin = false;
    float timeSinceLastDirectionChange = 0.0f;

    glm::vec3 getRandomDirection() const {
        float angle = static_cast<float>(rand()) / RAND_MAX * glm::two_pi<float>();
        return glm::normalize(glm::vec3(cos(angle), 0.0f, sin(angle)));
    }

    glm::vec3 findValidPosition(Scene& scene, const glm::vec3& currentPosition) {

        const int numSamples = 10;
        glm::vec3 newDirection = currentPosition;
        for (int i = 0; i < numSamples; ++i) 
        {
            float angle = static_cast<float>(rand()) / RAND_MAX * glm::two_pi<float>();
            glm::vec3 randomDirection = glm::normalize(glm::vec3(cos(angle), 0.0f, sin(angle)));

            glm::vec3 potentialPosition = currentPosition + randomDirection * Radius;

            float groundHeight = scene.getPhysicalHeightOnPosition(sf::Vector2f(potentialPosition.x, potentialPosition.z));

            if (groundHeight <= ElevationLimit) {
                newDirection = randomDirection;
                break;
            }
        }

        return newDirection;
    }

public:
    void init(Scene& scene) override 
    {
        targetDirection = getRandomDirection();
    }

    void update(Scene& scene, const sf::Time& dt) override
    {
        physicBodyData* physics = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
        Transform* transform = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

        
        if (physics && transform) 
        {
            if (!hasOrigin) {
                origin = transform->position;
                hasOrigin = true;
            }

            timeSinceLastDirectionChange += dt.asSeconds();
            bool shouldChangeDirection = timeSinceLastDirectionChange >= ChangeDirectionInterval ||
                glm::length(transform->position - origin) > Radius;

            float groundHeight = scene.getPhysicalHeightOnPosition(sf::Vector2f(transform->position.x, transform->position.z));

            if (shouldChangeDirection) 
            {
                targetDirection = getRandomDirection();
                timeSinceLastDirectionChange = 0.0f;
            }

            if (groundHeight > ElevationLimit) 
            {
                targetDirection = findValidPosition(scene, transform->position);
            }
            

            physics->force = Speed * dt.asSeconds();
            physics->direction = targetDirection;
        }
    }
};

/***************************************************************
 * EnemyAvoiderEnemyController
 * --------------------------------------------------------------
 * Purpose: L'ennemi se d�place dans une direction al�atoire et
 *          r��value cette direction selon trois conditions :
 *          1. Sortie du rayon d�fini.
 *          2. Altitude trop �lev�e.
 *          3. Temps �coul� trop long.
 *
 *          Si l'ennemi per�oit un autre ennemi dans son rayon,
 *          il se dirige vers l'oppos�e de celui-ci. Cette logique
 *          s'applique uniquement lors de la g�n�ration et n'est pas
 *          v�rifi�e en tout temps.
 * --------------------------------------------------------------
 ***************************************************************/


//TODO

 /***************************************************************
  * AggressifEnemyController
  * --------------------------------------------------------------
  * Purpose: L'ennemi se d�place vers le joueur lorsqu'il est �
  *          l'int�rieur d'un rayon d�fini. Lorsqu'il est suffisamment
  *          proche du joueur, l'ennemi bouge vers le player.
  * --------------------------------------------------------------
  ***************************************************************/


class AggressifEnemyController : public NativeScriptComponent {
public:
    static constexpr float Speed = 0.3f;             
    static constexpr float DetectionRadius = 300.0f;  
    static constexpr float AttackRadius = 5.0f;      
    static constexpr float AttackDelay = 1.0f;
    static constexpr float Radius = 300.f;
    static constexpr float ChangeDirectionInterval = 2.0f;
    static constexpr float ElevationLimit = 90.0f;

    Transform* target;
    GameObject* player;                  
    float timeSinceLastAttack = 0.0f;
    glm::vec3 targetDirection{0.f};
    glm::vec3 origin;
    bool hasOrigin = false;
    float speed;

    float timeSinceLastDirectionChange = 0.0f;


public:
    void init(Scene& scene) override
    {
        player = scene.get_child("player");
        if (player) {
            target = player->getData<Transform>(DATA_TYPE::TRANSFORM);
        }
    }

    void update(Scene& scene, const sf::Time& dt) override
    {
        physicBodyData* physics = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
        Transform* transform = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
        speed = Speed;
        if (physics && transform && player)
        {
            if (!hasOrigin)
            {
                origin = transform->position;
                hasOrigin = true;
            }

            glm::vec3 playerPos = target->position;
            glm::vec3 enemyPos = transform->position;

            float distanceToPlayer = glm::length(playerPos - enemyPos);

            // If within range
            if (distanceToPlayer <= DetectionRadius) 
            {
                speed = Speed * 5.f;
                targetDirection = glm::normalize(playerPos - enemyPos);
            }
            else 
            {
                //act exactly like the dummy

                timeSinceLastDirectionChange += dt.asSeconds();
                bool shouldChangeDirection = timeSinceLastDirectionChange >= ChangeDirectionInterval ||
                    glm::length(transform->position - origin) > Radius;


                if (shouldChangeDirection) {
                    targetDirection = getRandomDirection();
                    timeSinceLastDirectionChange = 0.0f;
                }
            }

            float groundHeight = scene.getPhysicalHeightOnPosition(sf::Vector2f(transform->position.x, transform->position.z));

            // Update the physics and apply the movement
            physics->force = Speed * dt.asSeconds();
            physics->direction = targetDirection;
        }
    }

    glm::vec3 getRandomDirection() const {
        float angle = static_cast<float>(rand()) / RAND_MAX * glm::two_pi<float>();
        return glm::normalize(glm::vec3(cos(angle), 0.f, sin(angle)));
    }
};


GameObject* EnemyFactory::createEnemy(const glm::vec3& position, TypeAI type)
{
    static int enemyId = 0;
    std::string uniqueName = "enemy_" + std::to_string(enemyId++);

    GameObject* enemy = scene->createGameObject(uniqueName);
    scene->setGameObjectPhysicComponent<EnemyPhysicComponent>(enemy);
    scene->setGameObjectRenderComponent<EnemyRenderComponent>(enemy);

    Transform* transform = enemy->getData<Transform>(DATA_TYPE::TRANSFORM);
    if (transform) {
        transform->position = position;
    }

    // Set the correct AI controller based on the TypeAI enum
    if (type == TypeAI::Dummy) 
    {
        enemy->setNativeScript<BasicEnemyController>();
    }
    else if (type == TypeAI::Aggressif)
    {
        enemy->setNativeScript<AggressifEnemyController>(); //to remove
       // enemy->setNativeScript<EnemyAvoiderEnemyController>(); TODO
    }
    else if (type == TypeAI::PlayerAvoider) 
    {
        enemy->setNativeScript<BasicEnemyController>(); //to remove
       // enemy->setNativeScript<PlayerAvoiderEnemyController>(); TODO
    }


    return enemy;
}

bool EnemyFactory::isValidSpawnPosition(const glm::vec3& position, const glm::vec3& spawnCenter,
    const glm::vec3& mapBounds)
{
    if (position.x < spawnCenter.x - mapBounds.x || position.x > spawnCenter.x + mapBounds.x ||
        position.z < spawnCenter.z - mapBounds.z || position.z > spawnCenter.z + mapBounds.z)
    {
        return false;
    }

    return true;
}

GameObject* EnemyFactory::spawner(GameObject* player, float minDistance, float maxDistance,
    const glm::vec3& spawnCenter, const glm::vec3& mapBounds)
{
    if (minDistance > maxDistance) std::swap(minDistance, maxDistance);

    Transform* playerTransform = player->getData<Transform>(DATA_TYPE::TRANSFORM);
    if (!playerTransform) return nullptr;

    glm::vec3 playerPos = playerTransform->position;

    glm::vec3 spawnPosition;
    bool validSpawn = false;

    while (!validSpawn)
    {
        float angle = static_cast<float>(rand()) / RAND_MAX * glm::two_pi<float>();
        glm::vec3 randomDirection = glm::normalize(glm::vec3(cos(angle), 0.f, sin(angle)));

        float distance = minDistance + static_cast<float>(rand()) / RAND_MAX * (maxDistance - minDistance);
        spawnPosition = playerPos + randomDirection * distance;
        spawnPosition.y = 0.f;

        if (isValidSpawnPosition(spawnPosition, spawnCenter, mapBounds))
        {
            validSpawn = true;
        }
    }

    return createEnemy(spawnPosition, TypeAI::Aggressif);
}