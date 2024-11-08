#pragma once

#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <glm/glm.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <unordered_map>
#include "thsan/graphic/light/Light.h"
#include "thsan/util/helperMath.h"
#include "thsan/graphic/PostProcess.h"

enum class DATA_TYPE: unsigned int {
    NONE          = 0,
    TRANSFORM     = 1,
    UI            = 2,
    TILE_MAP      = 3,
    VOXEL_TERRAIN = 4,
	CAMERA		  = 5,
    HITBOX        = 6,
	PHYSIC_BODY   = 7,
    TREES         = 8,
    PLAYER        = 9,
    EDITABLE_SPRITE_ANIMATION = 10,
    EDITABLE_IMAGE = 11,
    LIGHT          = 12,
    VOLUME_POST_PROCESS = 13,
    BARS           = 14,
    PLANT          = 15,
    GEAR           = 16,
    CHARACTER_STAT = 17,
    BAG            = 18
};


struct DataType{};

struct Transform: public DataType{
    Transform(){
        this->position = glm::vec3(0.f, 0.f, 0.f);
        this->scale = glm::vec2(1.f, 1.f);
        this->rotation = 0.f;
        this->parent = nullptr;
    }

    glm::vec3 position;
    glm::vec2 scale;
    float rotation;

    Transform* parent;
};


// note: position is in the center of the box.
//      In fact, every single position in shape
//      in hitBox is intended that way.

struct Sphere {
    float radius;
    glm::vec3 position;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 position;
};

struct Capsule {
    float radius;
    float height;
    glm::vec3 position;
};

enum class Collision_object {
    phantoms,
    real
};

struct HitBox: public DataType {
    std::vector<Sphere> spheres;
    std::vector<AABB> AABBs;
    std::vector<Capsule> capsules;
    glm::vec3 position{0.f, 0.f, 0.f};
    Collision_object collision_object{ Collision_object::phantoms};
};

struct UI_data: public DataType{
    UI_data(){
        isHover = false;
        isActive = false;
        isVisited = false;
        isLink = false;

        isText = false;
        isImage = false;
        isPanelAlert = true;
        isPanelDecoration = true;

        position = sf::Vector2f(0.f, 0.f);
        size = sf::Vector2f(100.f, 10.f);
        color = sf::Color(0, 0, 0, 255);

        text = "";
        path = "media/images/default.png";
    }

    bool isHover;
    bool isActive;
    bool isVisited;
    bool isLink;

    bool isText;
    bool isImage;
    bool isPanelAlert;
    bool isPanelDecoration;
	bool isPanelSelection;

    sf::Vector2f position;
    sf::Vector2f size;
	sf::Vector2f margin;
	sf::Vector2f padding;
    sf::Color color;

    std::string text;
    std::string path;

    std::vector<UI_data> childs;
};

struct TileMapData: public DataType{
    std::vector<uint8_t> tileRender;
    std::vector<uint8_t> tileCollision;
    std::vector<uint8_t> tileEvent;

    uint64_t tile_size_render;
    uint64_t tile_size_collision;
    uint64_t tile_size_event;

    uint64_t tile_grid_render;
    uint64_t tile_grid_collision;
    uint64_t tile_grid_event;
};

struct VoxelTerrainData: public DataType
{
    std::string height_map_path;
    std::string color_map_path;
    std::string collision_height_map_path;
    std::string collision_mask_map_path;
    std::string event_map_path;

    sf::Vector3f offset_camera_position;
    sf::Vector3f position;
    sf::Vector2f normal_blurr;
    float screen_width;
    float screen_height;

    int normal_blur_iteration;
    float LOD;
    float LOD_start_at;
    float angle;
    float height;
    float horizon;
    float scale_height;
    float sprite_render_distance;
    float terrain_render_distance;
    float shininess;
    int FOV;
    int blur_type;
};

struct CameraData : public DataType {
    bool isPolar{false};
    std::string target_name{};
	glm::vec3 offset;
    glm::vec2 screen_position;
    sf::FloatRect soft_zone;
    sf::FloatRect dead_zone;
    sf::FloatRect on_screen_target_rect;
    float offset_distance;
    float angle;
    float Damping;
};

struct physicBodyData : public DataType {
	physicBodyData() {
		force = 0.f;
		friction = 0.99f;
		direction = glm::vec3(0.f, 0.f, 0.f);
        masse = 0.f;
	}
	float force;
	float friction;
    float masse;
    glm::vec3 acceleration;
    glm::vec3 velocity;
	glm::vec3 direction;

    bool isJump{false};
    bool isDoubleJump{false};
    bool onGround{true};

};

struct PlayerData : public DataType {
    enum class State {
        idle,
        walk,
        run,
        attack,
        forward_attack,
        plant,
        damage,
        death,
        jump,
        double_jump,
        fall,
        land,
        charge
    };
    bool isAiming{false};
    State curr_state;
    std::unordered_map<std::string, int> items;
    Transform* target_transform;
    std::string target_name;

    float atk;
    float def;
    float hp;
};

struct EditableSpriteAnimationData : public DataType {
    EditableSpriteAnimationData():
        key_color(sf::Color::Transparent),
        shininess(32),
        depth_factor(1.f)
    {
    }
    sf::Color key_color;
    std::vector<std::string> list_of_animations;
    std::string current_animation;
    std::string path;
    float depth_factor;
    int shininess;
    bool has_depth;
    bool has_emission;
    bool flipX;
    bool flipY;
    

};

struct EditableImageData : public DataType {
    EditableImageData():
        flipX(false),
        flipY(false),
        shininess(32),
        depth_factor(1.f){
    }
    bool flipX;
    bool flipY;
    int shininess;
    float depth_factor;
    std::string path;
    std::string depth_path;
    std::string emission_path;
    sf::Color key_color;
};

struct LightData : public DataType {
    LightProperties lightProperties;
    bool isDecoration{false};
};

struct EnumClassEffectHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

struct VolumetricPostProcessData : public DataType {
    Ease ease{ Ease::in };
    InterpolationType interpolation{ InterpolationType::linear };
    sf::Time in_interpolation_duration{ sf::seconds(1.0f) };
    sf::Time out_interpolation_duration{ sf::seconds(1.0f) };
    std::unordered_map<PostProcess::Effect, float, EnumClassEffectHash> effects_and_opacity;
};

struct BarsData : public DataType {
    bool show_bar{ false };
    sf::Time duration{ sf::seconds(0.5f) };
    float end_speed{ 3.f }; // bars will hide at en_speed faster -> elapsed_time -= dt * end_speed
    float bar_screen_ratio_top{0.f};
    float bar_screen_ratio_bottom{0.f};
};



struct PlantData : public DataType {
public:
    enum class Stage {
        seed = 0,
        sprout = 1,
        seeding = 2,
        vegetative = 3,
        budding = 4,
        flowering = 5,
        ripping = 6,
        LAST = 7
    };

    enum class State {
        planted = 0,
        eaten = 1,
        thrown = 2,
        wore = 3,
        physical_item = 4,
        inventory_item = 5,
        LAST = 6
    };

    PlantData() {
        growth_val[Stage::seed] = 0.f;
        growth_val[Stage::sprout] = 0.1f;
        growth_val[Stage::seeding] = 0.2f;
        growth_val[Stage::vegetative] = 0.3f;
        growth_val[Stage::budding] = 0.5f;
        growth_val[Stage::flowering] = 0.8f;
        growth_val[Stage::ripping] = 0.9f;

        static int id_increment = 0;
        name_id = "id__" + std::to_string(id_increment);
        id_increment++;
    }           

    /*
        at: value of the point where the plant
            will become the specified stage. Usually 
            Handled automatically by the plantPhysicComponent
     */
    void setGrowth(Stage stage, float at) {
        if (at > 1.0f)
            at = 1.0f;
        else if (at < 0.f)
            at = 0.f;

        growth_val[stage] = at;
    }

    float getGrowth(Stage stage) {
        return growth_val[stage];
    }

    std::string getId() const{
        return name_id;
    }

    Stage current_stage = Stage::seed;
    State current_state = State::inventory_item;
    std::string owner = "";

    sf::Time growth_time{sf::seconds(5.f)};
    std::string name = "";

    float hp{ 100 };

private:
    std::string name_id = "";
    std::unordered_map<Stage, float> growth_val;

};

struct GearData: public DataType {
    enum class Type {
        weapon = 0,
        boot = 1,
        cloth = 2,
        LAST = 3
    };

    Type type;
    std::string name;
    std::string desc;
    bool isEquiped;
    std::string owner_name;
};

struct CharacterStatData : public DataType {

    struct Current {
        int hp;
        int mp;
    };

    Current curr;

    int hp;
    int mp;
    int atk;
    int def;
    int speed;
};

struct BagData : public DataType {
    BagData() = default;

    void addPlant(const PlantData& plantData) {
        if (std::none_of(plants.begin(), plants.end(), [&](const PlantData& plant) {
            return plant.getId() == plantData.getId();
            })) {
            plants.push_back(plantData);
        }
    }

    void addItem(const std::string& name, const std::string& desc) {
        auto& item = items[name];
        if (item.second > 0) {
            item.second++;
        }
        else {
            item.first = Item{ name, desc };
            item.second = 1;
        }
    }

    void addGear(const std::string& name, const std::string& desc) {
        auto& gear = gears[name];
        if (gear.second > 0) {
            gear.second++;
        }
        else {
            gear.first = Gear{ name, desc };
            gear.second = 1;
        }
    }

    void removePlant(const std::string& name_id) {
        plants.erase(std::remove_if(plants.begin(), plants.end(), [&name_id](const PlantData& plant) {
            return plant.getId() == name_id;
            }), plants.end());
    }

    void removeOneItem(const std::string& name) {
        if (items.count(name)) {
            if (--items[name].second == 0) {
                items.erase(name);
            }
        }
    }

    void removeOneGear(const std::string& name) {
        if (gears.count(name)) {
            if (--gears[name].second == 0) {
                gears.erase(name);
            }
        }
    }

    struct Item {
        std::string name;
        std::string desc;
    };

    struct Gear {
        std::string name;
        std::string desc;
    };

    std::vector<PlantData> getPlants() const {
        return plants;
    }

    std::vector<std::pair<Item, int>> getItems() const {
        return convertToVector(items);
    }

    std::vector<std::pair<Gear, int>> getGears() const {
        return convertToVector(gears);
    }

private:
    template<typename T>
    std::vector<std::pair<T, int>> convertToVector(const std::unordered_map<std::string, std::pair<T, int>>& map) const {
        std::vector<std::pair<T, int>> vec;
        for (const auto& elem : map) {
            vec.push_back(elem.second);
        }
        return vec;
    }

    std::vector<PlantData> plants;
    std::unordered_map<std::string, std::pair<Item, int>> items;
    std::unordered_map<std::string, std::pair<Gear, int>> gears;
};
