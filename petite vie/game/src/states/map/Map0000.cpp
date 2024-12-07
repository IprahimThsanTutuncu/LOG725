#include "Map0000.h"
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

#include "gameObjects/plant/PlantFactory.h"
#include "gameObjects/enemy/EnemyFactory.h"

#include "gameObjects/gameCleared/GameClearedRenderComponent.h"

#include <cstdlib> 
#include <ctime> 

void Map0000::generatePlantsInRange(int n, float range, GameObject* player) {
    PlantFactory pf(m_scene);

    Transform* playerTransform = player->getData<Transform>(DATA_TYPE::TRANSFORM);
    if (!playerTransform) return;

    glm::vec3 playerPosition = playerTransform->position;

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(0)));
        seeded = true;
    }

    for (int i = 0; i < n; ++i)
    {
        float randomX = playerPosition.x + (rand() % static_cast<int>(range * 2)) - range;
        float randomZ = playerPosition.z + (rand() % static_cast<int>(range * 2)) - range;

        GameObject* plant = pf.createTrilleRouge();
        Transform* plantTransform = plant->getData<Transform>(DATA_TYPE::TRANSFORM);
        if (plantTransform) {
            plantTransform->position = glm::vec3(randomX, plantTransform->position.y, randomZ);
            plantTransform->scale = glm::vec2(0.1, 0.1);
        }

        PlantData* plant_data = plant->getData<PlantData>(DATA_TYPE::PLANT);
        plant_data->hp = 100;
        if (plant_data)
            plant_data->current_state = PlantData::State::planted;
    }
}

void Map0000::spawnTrilleRougeWithPosition(float x, float z) {
    PlantFactory pf(m_scene);
    GameObject* plant = pf.createTrilleRouge();

    Transform* transform = plant->getData<Transform>(DATA_TYPE::TRANSFORM);
    if (transform)
    {
        transform->position.x = x;
        transform->position.z = z;

        transform->scale = glm::vec2(0.1f, 0.1f);
    }

    PlantData* plant_data = plant->getData<PlantData>(DATA_TYPE::PLANT);
    plant_data->hp = 100;
    if (plant_data)
        plant_data->current_state = PlantData::State::planted;
}


Map0000::Map0000(Game* parent) :
    MapState(parent)
{
    m_scene = new Scene(this, "media/data/map/map0000.json", VIEW_TYPE::DYNAMIC_VIEW);
}


void Map0000::init()
{
    PlantFactory pf(m_scene);
    GameObject* plant = pf.createTrilleRouge();

    GameObject* player = m_scene->createGameObject("player");
    m_scene->setGameObjectInputComponent<PlayerInputComponent>(player);
    m_scene->setGameObjectPhysicComponent<PlayerPhysicComponent>(player);
    m_scene->setGameObjectRenderComponent<PlayerSpriteRenderComponent>(player);

    Transform* player_t = player->getData<Transform>(DATA_TYPE::TRANSFORM);
    player_t->position = glm::vec3(0.f, player_t->position.y, 0.f);

    GameObject* truc = m_scene->createGameObject("victory sign");
    m_scene->setGameObjectRenderComponent<GameClearedRenderComponent>(truc);

    generatePlantsInRange(20, 200, player);

    //put this in a methode where I can generate mechants object with a position on x and z
    EnemyFactory ef(m_scene);

    for (int i = 0; i < 20; i++) 
    {
        GameObject* mechant = ef.spawner(player, 100.f, 800.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1024.f, 1000.f, 1024.f));
    }
    GameObject* go = m_scene->createGameObject("camera");
    m_scene->setGameObjectPhysicComponent<FollowCameraPhysicComponent>(go, player);
    m_scene->setGameObjectRenderComponent<CameraRenderComponent>(go);

    GameObject* bars = m_scene->createGameObject("bars");
    m_scene->setGameObjectRenderComponent<BarRenderComponent>(bars);

    GameObjectFactory go_fact(m_scene);

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

	std::shared_ptr<PostProcess> pp = m_scene->getPostProcess();
	pp->enable();
	pp->snowfall_param.bg_layers = 2.f;
	pp->snowfall_param.fg_layers = 1.f;
	pp->snowfall_param.layer_gap = 1.f;
	pp->snowfall_param.size_rate = 0.1f;
	pp->snowfall_param.speed = sf::Glsl::Vec2(-2.f, -2.f);
	pp->snowfall_param.wind_direction = sf::Glsl::Vec2(-0.2, -0.4);

	pp->letterbox_param.bar_screen_ratio_top = 0.0f;
	pp->letterbox_param.bar_screen_ratio_bottom = 0.0f;

	pp->letterbox_param.fading_range_top = 0.1f;
	pp->letterbox_param.fading_range_bottom = 0.1f;

	pp->letterbox_param.type = PostProcess::Letterbox_param::Type::smoothstep;
	pp->letterbox_param.top_bar_color = m_scene->getDirectionalLightProperties().ambient;
	pp->letterbox_param.bottom_bar_color = m_scene->getDirectionalLightProperties().ambient;

	pp->letterbox_param.blend = PostProcess::Blend::additive;

	pp->enable_effect(PostProcess::Effect::snowfall);
	pp->bind_effect_to_layer(PostProcess::Effect::snowfall, 0);

	pp->enable_effect(PostProcess::Effect::letterbox);
	pp->bind_effect_to_layer(PostProcess::Effect::letterbox, 1);

    //m_scene->setDirLight_ambient(sf::Color(64, 64, 64));
    //m_scene->setDirLight_direction(sf::Vector3f(0, 0, -1));
    //m_scene->setDirLight_diffuse(sf::Color(128, 64, 32));

}

void Map0000::handleEvent(sf::Event event)
{

}

void Map0000::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) {
    m_scene->input(deltaTime, inputActions);
}

void Map0000::update(const sf::Time& deltaTime)
{
    glm::vec3 position = m_scene->get_child("player")->getData<Transform>(DATA_TYPE::TRANSFORM)->position;

    static float t = 0;
    t += deltaTime.asSeconds();
    float d = sqrt(cos(t) * cos(t) + -sin(t) * sin(t));
    //m_scene->setDirLight_direction(sf::Vector3f(cos(t)/d, -0.5/d, sin(t)/d));
    m_scene->update(deltaTime);

}

void Map0000::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    //m_scene->enableDebugRendering(Debug_rendering::debug_normal);
    LightProperties dir = m_scene->getDirectionalLightProperties();
    target.clear(dir.ambient);
    m_scene->setRenderTarget(&target);
    m_scene->render(deltaTime);
}
