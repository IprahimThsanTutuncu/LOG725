#pragma once

#include <thsan/gameObject/component/PhysicComponent.h>
#include <glm/vec3.hpp>
#include <SFML/System/Time.hpp>

class Scene;
class GameObject;
class PlantData;
class HitBox;

class PlantPhysicComponent : public PhysicComponent
{
public:
    PlantPhysicComponent();

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt) override;
    ~PlantPhysicComponent();
private:
    bool isPhysicalObject{ false };
    bool onGround{ false };
    bool firstFrameNotOnGround{ false };
    bool isNearPlayer{ false };
    float random_val{0.f};
    float random_sign{0.f};
    glm::vec3 original{ 0.f };
    GameObject* player{ nullptr };
    PlantData* plant_data{ nullptr };
    sf::Time timer{ sf::Time::Zero };
    HitBox* hitbox;



};

