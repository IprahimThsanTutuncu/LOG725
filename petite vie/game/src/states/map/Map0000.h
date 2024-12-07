#pragma once

#include "MapState.h"
#include <glm/vec3.hpp>

class PostProcess;

class Map0000 : public MapState
{
public:
    Map0000(Game* parent);
    void init() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    ~Map0000() = default;

private:
    void spawnTrilleRougeWithPosition(float x, float z);
    void generatePlantsInRange(int n, float range, GameObject* player);

    bool isInit{ false };
    std::shared_ptr<PostProcess> pp;
    GameObject* go_volume_pp;
    std::vector<GameObject*> mechants;
};

