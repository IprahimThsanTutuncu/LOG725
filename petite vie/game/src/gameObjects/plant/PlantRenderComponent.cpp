#include "PlantRenderComponent.h"

#include <thsan/ressourceManager/ShaderManager.h>
#include <thsan/vendors/nlohmann/json.hpp>
#include <thsan/util/helper.h>

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;


PlantRenderComponent::PlantRenderComponent(const std::string& spriteSheetMetaDataPath, bool hasEmissionMap, bool hasDepthMap)
	:spriteSheetMetaDataPath(spriteSheetMetaDataPath),
	hasEmissionMap(hasEmissionMap),
	hasDepthMap(hasDepthMap)
{
}

PlantRenderComponent::~PlantRenderComponent()
{
}

void PlantRenderComponent::init(Scene& scene)
{
	if(parent->hasData(DATA_TYPE::PLANT))
		plant_data = parent->getData<PlantData>(DATA_TYPE::PLANT);
	else {
		plant_data = new PlantData();
		parent->setData(DATA_TYPE::PLANT, plant_data);
	}

    if (!parent->hasData(DATA_TYPE::EDITABLE_SPRITE_ANIMATION)) {
        editableSpriteAnimationData = new EditableSpriteAnimationData();
        parent->setData(DATA_TYPE::EDITABLE_SPRITE_ANIMATION, editableSpriteAnimationData);
    }
    else
        editableSpriteAnimationData = parent->getData<EditableSpriteAnimationData>(DATA_TYPE::EDITABLE_SPRITE_ANIMATION);

    nlohmann::ordered_json j;
    readJson(spriteSheetMetaDataPath, &j);

    std::string img_path = j["meta"]["image"];
    std::size_t last_slash = 0;

    auto found = img_path.find("\\");
    if (found != std::string::npos)
        last_slash = img_path.find_last_of("\\");

    img_path = img_path.substr(last_slash, img_path.size());

    fs::path parent_path = spriteSheetMetaDataPath;
    spriteAnimation.init(parent_path.parent_path().string() + "\\" + img_path);

    std::vector<json> frames;

    for (auto& [key, value] : j["frames"].items()) 
    {
        frames.push_back(value);
    }

    for (auto& [key, value] : j["meta"]["frameTags"].items()) 
    {
        std::string name = value["name"];
        int begin = value["from"];
        int end = value["to"];

        editableSpriteAnimationData->list_of_animations.push_back(name);
        spriteAnimation.setCurrAnimation(name);
        //std::cout << name << "begin: " << begin << ", end" << end << std::endl;

        for (int i = begin; i <= end; i++) 
        {
            auto& frame = frames[i]["frame"];
            auto& duration = frames[i]["duration"];
            //std::cout << " x : " << frame["x"] << ", y: " << frame["y"] << ", w : " << frame["w"] << ",h : " << frame["h"] << std::endl;
            spriteAnimation.addFrame(name, sf::IntRect(frame["x"], frame["y"], frame["w"], frame["h"]), sf::milliseconds(duration));
        }
    }

    editableSpriteAnimationData->current_animation = spriteAnimation.getCurrAnimation();
    editableSpriteAnimationData->key_color = sf::Color(0, 0, 0, 0);
    spriteAnimation.setKeyColor(editableSpriteAnimationData->key_color);

    editableSpriteAnimationData->path = spriteSheetMetaDataPath;
    editableSpriteAnimationData->flipY = false;
    editableSpriteAnimationData->flipX = false;

    if (hasEmissionMap)
        spriteAnimation.setEmissionTexture(parent_path.parent_path().string() + "\\" + img_path.substr(0, img_path.size() - 4) + "emission.png");
    if (hasDepthMap)
        spriteAnimation.setDepthTexture(parent_path.parent_path().string() + "\\" + img_path.substr(0, img_path.size() - 4) + "depth.png");

    editableSpriteAnimationData->has_emission = spriteAnimation.hasEmissionTexture();
    editableSpriteAnimationData->has_depth = spriteAnimation.hasDepthTexture();

    isPhysicalObject = plant_data->current_state == PlantData::State::physical_item;
}

void PlantRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer)
{
    if (plant_data->current_state == PlantData::State::planted) {
        isPhysicalObject = false;

        if (editableSpriteAnimationData->current_animation != plant_animation_map[plant_data->current_stage])
        {
            editableSpriteAnimationData->current_animation = plant_animation_map[plant_data->current_stage];
            spriteAnimation.setCurrAnimation(editableSpriteAnimationData->current_animation);
        }

        if (spriteAnimation.getShininess() != editableSpriteAnimationData->shininess)
            spriteAnimation.setShininess(editableSpriteAnimationData->shininess);

        if (spriteAnimation.isFlipX() != editableSpriteAnimationData->flipX)
            spriteAnimation.flipX();

        if (spriteAnimation.isFlipY() != editableSpriteAnimationData->flipY)
            spriteAnimation.flipY();

        spriteAnimation.setKeyColor(editableSpriteAnimationData->key_color);

        if (spriteAnimation.getCurrAnimation() != editableSpriteAnimationData->current_animation)
            spriteAnimation.setCurrAnimation(editableSpriteAnimationData->current_animation);
        spriteAnimation.update(dt);

        Transform* transform = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
        glm::vec3 position = transform->position;
        glm::vec2 scale = transform->scale;
        float rotation = transform->rotation;
        position.x = int(position.x + 0.5);
        position.z = int(position.z + 0.5);

        spriteAnimation.setDepthFactor(editableSpriteAnimationData->depth_factor);

        renderer.add(&spriteAnimation, sf::Vector3f(position.x, position.y, position.z), sf::Vector2f(scale.x, scale.y), rotation);
    }
    else {
        Transform* transform = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

        if (plant_data->current_state == PlantData::State::physical_item && !isPhysicalObject) {
            
            float y = (spriteAnimation.getCurrFrameRect().height * transform->scale.y) / 2.0f;
            transform->position.y += y;
            isPhysicalObject = true;
        }

        glm::vec3 position = transform->position;
        glm::vec2 scale = transform->scale * 0.25f;
        float rotation = transform->rotation + 45.f;
        position.x = int(position.x + 0.5);
        position.z = int(position.z + 0.5);

        spriteAnimation.update(dt);

        renderer.add(&spriteAnimation, sf::Vector3f(position.x, position.y, position.z), sf::Vector2f(scale.x, scale.y), rotation);
    }

}
