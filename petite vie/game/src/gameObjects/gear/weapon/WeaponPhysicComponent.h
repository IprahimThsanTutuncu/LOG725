#pragma once

#include <thsan/gameObject/component/PhysicComponent.h>
#include <functional>

class CharacterStatData;
class GearData;
class GameObject;

class WeaponPhysicComponent : public PhysicComponent
{
public:
    WeaponPhysicComponent( 
        std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> effect,
        std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> undo
    );

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt) override;
    void onDelete(Scene& scene) override;

    virtual void init_for_attack(Scene& scene) = 0;
    virtual void on_attack(Scene& scene, const sf::Time& dt) = 0;


    ~WeaponPhysicComponent() = default;
protected:
    bool isEffectGiven{false};

    CharacterStatData* charStatData{nullptr};
    GearData* gearData{nullptr};
    GameObject* target{nullptr};

    std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> effect;
    std::function<void(GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat)> undo;
};
