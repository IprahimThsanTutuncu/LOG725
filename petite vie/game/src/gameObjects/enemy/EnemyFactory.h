#pragma once

#include <glm/glm.hpp>
#include <string>

class GameObject;
class Scene;

class EnemyFactory {
public:
    enum class TypeAI
    {
        Dummy, // sample a point inside a radius based on an origin position and will go there
        Aggressif, // like dummy, but if he perceive an enemy, he won't sample near his radius. Can sample from anywhere from -1024 to 1024 in x and y. Has a radius. the center of the radius can move if an enemy is near
        PlayerAvoider // like enemy avoider but only if he percevie de player, much more slower than other
    };
    // Static method to create an enemy at a given position
    EnemyFactory(Scene* scene) : scene(scene) {}
    GameObject* createEnemy(const glm::vec3& position, TypeAI type);
    bool isValidSpawnPosition(const glm::vec3& position, const glm::vec3& spawnCenter, const glm::vec3& mapBounds);
    GameObject* spawner(GameObject* player, float minDistance, float maxDistance, const glm::vec3& spawnCenter, const glm::vec3& mapBounds);

private:
    Scene* scene;
};

