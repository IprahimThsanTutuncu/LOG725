#include "TestMapState.h"
#include <thsan/ressourceManager/ShaderManager.h>
#include <thsan/gameObject/component/NativeScriptComponent.h>

#include "gameObjects/camera/FollowCameraPhysicComponent.h"
#include "gameObjects/camera/CameraRenderComponent.h"

#include "gameObjects/player/PlayerInputComponent.h"
#include "gameObjects/player/PlayerPhysicComponent.h"
#include "gameObjects/player/PlayerSpriteRenderComponent.h"

#include "gameObjects/NPC/OrangeCuteRenderComponent.h"
#include "gameObjects/NPC/FixedOnGroundPhysicComponent.h"
#include "gameObjects/NPC/SimpleDialogNPCInputComponent.h"

#include "gameObjects/enemy/EnemyRenderComponent.h"
#include "gameObjects/enemy/EnemyPhysicComponent.h"

#include "gameObjects/bar/BarRenderComponent.h"

#include <thsan/gameObject/GameObjectFactory.h>
#include <glm/gtx/string_cast.hpp>

TestMapState::TestMapState(Game* parent) :
    State(parent),
    scene(this, "media/data/map/test_roll.json", VIEW_TYPE::DYNAMIC_VIEW)
{
}

std::string TestMapState::getType()
{
    return "hxh";
}

void TestMapState::init()
{
    GameObject* player = scene.createGameObject("player");
    scene.setGameObjectInputComponent<PlayerInputComponent>(player);
    scene.setGameObjectPhysicComponent<PlayerPhysicComponent>(player);
    scene.setGameObjectRenderComponent<PlayerSpriteRenderComponent>(player);

    PlantData plant;
    plant.name = "rouge fleur";
    plant.hp = 105.f;

    PlantData plant2;
    plant2.name = "bleu fleur";
    plant2.hp = 65.f;

    PlantData plant3;
    plant3.name = "vert fleur";
    plant3.hp = 65.f;

    PlantData plant4;
    plant4.name = "mauve fleur";
    plant4.hp = 65.f;

    PlantData plant5;
    plant5.name = "ma fleur";
    plant5.hp = 65.f;

    PlantData plant6;
    plant6.name = "me fleur";
    plant6.hp = 65.f;

    PlantData plant7;
    plant7.name = "mi fleur";
    plant7.hp = 65.f;

    player->getData<BagData>(DATA_TYPE::BAG)->addPlant(plant);
    player->getData<BagData>(DATA_TYPE::BAG)->addPlant(plant2);
    player->getData<BagData>(DATA_TYPE::BAG)->addPlant(plant3);
    player->getData<BagData>(DATA_TYPE::BAG)->addPlant(plant4);
    player->getData<BagData>(DATA_TYPE::BAG)->addPlant(plant5);
    player->getData<BagData>(DATA_TYPE::BAG)->addPlant(plant6);
    player->getData<BagData>(DATA_TYPE::BAG)->addPlant(plant7);

    Transform* player_t = player->getData<Transform>(DATA_TYPE::TRANSFORM);
    player_t->position = glm::vec3(0.f, player_t->position.y, 0.f);

    GameObject* truc = scene.createGameObject("orange :)");
    scene.setGameObjectInputComponent<SimpleDialogNPCInputComponent>(truc);
    scene.setGameObjectPhysicComponent<FixedOnGroundPhysicComponent>(truc);
    scene.setGameObjectRenderComponent<OrangeCuteRenderComponent>(truc);

    GameObject* mechant = scene.createGameObject("motobug");
    scene.setGameObjectPhysicComponent<EnemyPhysicComponent>(mechant);
    scene.setGameObjectRenderComponent<EnemyRenderComponent>(mechant);


    class BasicEnemyController : public NativeScriptComponent {
    private:
        float speed = 0.0f;   
        glm::vec3 targetDirection; 
        float changeDirectionInterval = 2.0f;
        float timeSinceLastDirectionChange = 0.0f;

        glm::vec3 getRandomDirection() 
        {
            float angle = static_cast<float>(rand()) / RAND_MAX * glm::two_pi<float>();
            return glm::normalize(glm::vec3(cos(angle), 0.0f, sin(angle)));
        }

    public:
        void init(Scene& scene) override {
            targetDirection = getRandomDirection();
        }

        void update(Scene& scene, const sf::Time& dt) override {
            physicBodyData* physics = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
            Transform* transform = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

            if (physics && transform) 
            {
                timeSinceLastDirectionChange += dt.asSeconds();
                if (timeSinceLastDirectionChange >= changeDirectionInterval) {
                    targetDirection = getRandomDirection();
                    timeSinceLastDirectionChange = 0.0f;
                }

                // Update physics data
                physics->force = speed;
                physics->direction = targetDirection;


               
            }
        }
    };


    mechant->setNativeScript<BasicEnemyController>();

    Transform* et = mechant->getData<Transform>(DATA_TYPE::TRANSFORM);
    et->position.z = 100;

    GameObject* go = scene.createGameObject("camera");
    scene.setGameObjectPhysicComponent<FollowCameraPhysicComponent>(go, player);
    scene.setGameObjectRenderComponent<CameraRenderComponent>(go);

    GameObject* bars = scene.createGameObject("bars");
    scene.setGameObjectRenderComponent<BarRenderComponent>(bars);

    GameObjectFactory go_fact(&scene);

    HitBox hb;
    AABB box;
    box.max = glm::vec3(100.f, 2000.f, 2000.f);
    box.min = glm::vec3(-100.f, -2000.f, -2000.f);
    box.position = glm::vec3(-100.f, 0.f, 0.f);
    hb.position = glm::vec3(-100.f, 0.f, 0.f);
    hb.AABBs.push_back(box);

    VolumetricPostProcessData vppd;
    vppd.ease = Ease::None;
    vppd.interpolation = InterpolationType::linear;
    vppd.in_interpolation_duration = sf::seconds(0.75);
    vppd.out_interpolation_duration = sf::seconds(0.75f);

    vppd.effects_and_opacity[PostProcess::Effect::letterbox] = 1.0f;
    vppd.effects_and_opacity[PostProcess::Effect::snowfall] = 1.0f;

    go_volume_pp = go_fact.createVolumetricPostProcess("volume_pp", truc, hb, vppd);

    class CameraController : public NativeScriptComponent {

        void init(Scene& scene) override {
            CameraData* cd = parent->getData<CameraData>(DATA_TYPE::CAMERA);

            cd->offset.y = 15.f;
            cd->angle = 0.f;
            cd->offset_distance = 70.f;
            cd->isPolar = true;
        };
        void update(Scene& scene, const sf::Time& dt) override {
        };

        void onDelete(Scene& scene) override {
        
        };
    };

    go->setNativeScript<CameraController>();

    pp = scene.getPostProcess();
    pp->enable();
    pp->snowfall_param.bg_layers = 2.f;
    pp->snowfall_param.fg_layers = 2.f;
    pp->snowfall_param.layer_gap = 5.f;
    pp->snowfall_param.size_rate = 0.1f;
    pp->snowfall_param.speed = sf::Glsl::Vec2(2.f, -3.f);
    pp->snowfall_param.wind_direction = sf::Glsl::Vec2(0.2, -0.4);

    pp->letterbox_param.top_bar_color = sf::Color::Black;
    pp->letterbox_param.bottom_bar_color = sf::Color::Black;
    pp->letterbox_param.left_bar_color = sf::Color::Cyan;
    pp->letterbox_param.right_bar_color = sf::Color(128,64,0);

    pp->letterbox_param.bar_screen_ratio_top = 0.1f;
    pp->letterbox_param.bar_screen_ratio_bottom = 0.1f;
    pp->letterbox_param.bar_screen_ratio_left = 0.0f;
    pp->letterbox_param.bar_screen_ratio_right = 0.0f;

    pp->letterbox_param.priority_letterbox[3] = PostProcess::Letterbox_param::Bar::top;
    pp->letterbox_param.priority_letterbox[2] = PostProcess::Letterbox_param::Bar::bottom;
    pp->letterbox_param.priority_letterbox[1] = PostProcess::Letterbox_param::Bar::right;
    pp->letterbox_param.priority_letterbox[0] = PostProcess::Letterbox_param::Bar::left;

    pp->letterbox_param.blend = PostProcess::Blend::multiply;
    pp->letterbox_param.type = PostProcess::Letterbox_param::Type::smoothstep;

    pp->enable_effect(PostProcess::Effect::snowfall, 0.0f);
    pp->bind_effect_to_layer(PostProcess::Effect::snowfall, 0);

    pp->enable_effect(PostProcess::Effect::letterbox, 0.0f);
    pp->bind_effect_to_layer(PostProcess::Effect::letterbox, 1);

    scene.setDirLight_ambient(sf::Color(64, 64, 64));
    scene.setDirLight_direction(sf::Vector3f(0, 0, -1));
    scene.setDirLight_diffuse(sf::Color(128, 64, 32));

}

void TestMapState::handleEvent(sf::Event event)
{

}

void TestMapState::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) {
    scene.input(deltaTime, inputActions);
}

void TestMapState::update(const sf::Time& deltaTime)
{
    glm::vec3 position = scene.get_child("player")->getData<Transform>(DATA_TYPE::TRANSFORM)->position;

    static float t = 0; 
    t += deltaTime.asSeconds();
    float d = sqrt(cos(t) * cos(t) + - sin(t) * sin(t));
    //scene.setDirLight_direction(sf::Vector3f(cos(t)/d, -0.5/d, sin(t)/d));
    scene.update(deltaTime);

    pp->letterbox_param.fading_range_top = 0.01f;
    pp->letterbox_param.fading_range_bottom = 0.01f;
    pp->letterbox_param.fading_range_left = 1.0;
    pp->letterbox_param.fading_range_right = 1.0;
}

void TestMapState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    //scene.enableDebugRendering(Debug_rendering::debug_normal);
    LightProperties dir = scene.getDirectionalLightProperties();
    target.clear(dir.ambient);
    scene.setRenderTarget(&target);
    scene.render(deltaTime);
}
