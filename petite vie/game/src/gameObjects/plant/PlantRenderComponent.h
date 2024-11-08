#pragma once

#include <thsan/gameObject/component/RenderComponent.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/SpriteAnimation.h>
#include <SFML/System/Time.hpp>
#include <string>

class scene;
class GameObject;
class RendererPseudo3D;

class PlantRenderComponent : public RenderComponent
{
public:
    PlantRenderComponent(const std::string& spriteSheetMetaDataPath, bool hasEmissionMap = false, bool hasDepthMap = false);
    ~PlantRenderComponent();

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer) override;
    
private:

    enum class Stage {
        seed,
        sprout,
        seeding,
        vegetative,
        budding,
        flowering,
        ripping
    };

    std::unordered_map<PlantData::Stage, std::string> plant_animation_map{
        {PlantData::Stage::seed, "seed"},
        {PlantData::Stage::sprout, "sprout"},
        {PlantData::Stage::seeding, "seeding"},
        {PlantData::Stage::vegetative, "vegetative"},
        {PlantData::Stage::budding, "budding"},
        {PlantData::Stage::flowering, "flowering"},
        {PlantData::Stage::ripping, "ripping"}
    };

    PlantData* plant_data{ nullptr };
    EditableSpriteAnimationData* editableSpriteAnimationData{ nullptr };
    sf::Time elapsedTime{ sf::seconds(0.f) };

    std::string spriteSheetMetaDataPath;
    bool hasEmissionMap;
    bool hasDepthMap;
    bool isPhysicalObject;

    SpriteAnimation spriteAnimation;
};
