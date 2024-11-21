#include "PlayerSpriteRenderComponent.h"
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <thsan/CollisionEngine/CollisionEngine.h>

#include <iostream>
#include <SFML/Graphics/Glsl.hpp>
#include <glm/glm.hpp>

PlayerSpriteRenderComponent::PlayerSpriteRenderComponent():
    playerSprite("media/image/character/sonic.png"),
    sparkleSprite("media/image/effect/sparkle_1.png"),
	isLookingLeft(true)
{

}

void PlayerSpriteRenderComponent::init(Scene& scene)
{
    if (!parent->hasData(DATA_TYPE::PHYSIC_BODY)) {
        parent->setData(DATA_TYPE::PHYSIC_BODY, new physicBodyData());
    }

    if (!parent->hasData(DATA_TYPE::PLAYER)) {
        parent->setData(DATA_TYPE::PLAYER, new PlayerData());
    }

    if (!parent->hasData(DATA_TYPE::BAG)) {
        parent->setData(DATA_TYPE::BAG, new BagData());
    }

    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
    temp->scale.x = 0.45f;
    temp->scale.y = 0.45f;

    playerSprite.setShininess(32.f);

	playerSprite.setKeyColor(sf::Color(0, 112, 112, 255));
    playerSprite.setCurrAnimation("idle");
    playerSprite.addFrame("idle", sf::IntRect(11, 14, 22, 32), sf::seconds(0.1f));
    playerSprite.addFrame("idle", sf::IntRect(38, 12, 22, 34), sf::seconds(0.1f));
    playerSprite.addFrame("idle", sf::IntRect(65, 11, 24, 35), sf::seconds(0.1f));
    playerSprite.addFrame("idle", sf::IntRect(123, 14, 26, 32), sf::seconds(0.1f));
    playerSprite.addFrame("idle", sf::IntRect(154, 15, 24, 31), sf::seconds(0.1f));

    playerSprite.setCurrAnimation("jumping");
    playerSprite.addFrame("jumping", sf::IntRect(9, 272, 22, 40), sf::seconds(0.1f));
    playerSprite.addFrame("jumping", sf::IntRect(36, 272, 22, 40), sf::seconds(0.1f));
    playerSprite.addFrame("jumping", sf::IntRect(63, 274, 24, 36), sf::seconds(0.1f));
    playerSprite.addFrame("jumping", sf::IntRect(92, 277, 23, 34), sf::seconds(0.1f));
    playerSprite.addFrame("jumping", sf::IntRect(120, 280, 22, 31), sf::seconds(0.1f));

    playerSprite.setCurrAnimation("double-jumping");
    playerSprite.addFrame("double-jumping", sf::IntRect(14, 413, 33, 33), sf::seconds(0.1f));
    playerSprite.addFrame("double-jumping", sf::IntRect(52, 413, 33, 33), sf::seconds(0.1f));
    playerSprite.addFrame("double-jumping", sf::IntRect(90, 414, 31, 31), sf::seconds(0.1f));
    playerSprite.addFrame("double-jumping", sf::IntRect(126, 416, 29, 27), sf::seconds(0.1f));
    playerSprite.addFrame("double-jumping", sf::IntRect(162, 414, 26, 31), sf::seconds(0.1f));
    playerSprite.addFrame("double-jumping", sf::IntRect(194, 411, 26, 36), sf::seconds(0.1f));
    playerSprite.addFrame("double-jumping", sf::IntRect(226, 410, 26, 36), sf::seconds(0.1f));

    playerSprite.setCurrAnimation("idle-to-run");
    playerSprite.addFrame("idle-to-run", sf::IntRect(10, 101, 24, 33), sf::seconds(0.1f));
    playerSprite.setCurrAnimation("run");
    playerSprite.addFrame("run", sf::IntRect(45, 100, 31, 32), sf::seconds(0.1f));
    playerSprite.addFrame("run", sf::IntRect(81, 101, 30, 32), sf::seconds(0.1f));
    playerSprite.addFrame("run", sf::IntRect(116, 100, 23, 34), sf::seconds(0.1f));
    playerSprite.addFrame("run", sf::IntRect(144, 99, 28, 35), sf::seconds(0.1f));
    playerSprite.addFrame("run", sf::IntRect(177, 101, 30, 33), sf::seconds(0.1f));
    playerSprite.addFrame("run", sf::IntRect(212, 101, 28, 33), sf::seconds(0.1f));
    playerSprite.addFrame("run", sf::IntRect(245, 100, 23, 34), sf::seconds(0.1f));
    playerSprite.addFrame("run", sf::IntRect(273, 99, 28, 35), sf::seconds(0.1f));

    playerSprite.setCurrAnimation("landing");
    playerSprite.addFrame("landing", sf::IntRect(11, 370, 27, 30), sf::seconds(0.1f));
    playerSprite.addFrame("landing", sf::IntRect(43, 370, 26, 30), sf::seconds(0.1f));
    playerSprite.addFrame("landing", sf::IntRect(74, 370, 22, 30), sf::seconds(0.1f));

    playerSprite.setCurrAnimation("falling");
    playerSprite.addFrame("falling", sf::IntRect(12, 325, 24, 34), sf::seconds(0.1f));
    playerSprite.addFrame("falling", sf::IntRect(42, 323, 30, 36), sf::seconds(0.1f));
    playerSprite.addFrame("falling", sf::IntRect(77, 321, 35, 39), sf::seconds(0.1f));
    playerSprite.addFrame("falling", sf::IntRect(117, 320, 35, 40), sf::seconds(0.1f));

    playerSprite.setCurrAnimation("charging");
    playerSprite.addFrame("charging", sf::IntRect(165, 1066, 20, 24), sf::seconds(0.05f));
    playerSprite.addFrame("charging", sf::IntRect(191, 1065, 20, 25), sf::seconds(0.05f));
    playerSprite.addFrame("charging", sf::IntRect(218, 1067, 20, 22), sf::seconds(0.05f));
    playerSprite.addFrame("charging", sf::IntRect(246, 1068, 20, 20), sf::seconds(0.05f));

	//playerSprite.setDepthTexture("media/image/base0.png");

    plantSprite = createSpriteAnimation("media/image/effect/slash_fire.png", "plantSprite");

    plantSprite->enableLoop(false);
    plantSprite->setKeyColor(sf::Color(0, 0, 0, 0));
    plantSprite->setCurrAnimation("plant");
    plantSprite->addFrame("plant", sf::IntRect(0, 0, 1, 1), sf::seconds(0.1f));

    ////////////////////////////////////////////
    // set up hitbox and animation of attacks //
    //////////////////////////////////////////// 

    slashSprite = createSpriteAnimation("media/image/effect/slash_fire.png", "slashSprite");
    attack_hitbox_start.spheres.push_back(Sphere{ 50.f, glm::vec3(0.f) });
    attack_hitbox_mid.spheres.push_back(Sphere{ 200.f, glm::vec3(0.f) });
    player_hitbox.spheres.push_back(Sphere{ 24.f, glm::vec3(0.f) });
    slashSprite->enableLoop(false);
    slashSprite->setKeyColor(sf::Color(0, 0, 0, 0));
    slashSprite->setCurrAnimation("slash");
    slashSprite->addFrame("slash", sf::IntRect(215, 201, 127, 119), sf::seconds(0.1f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 1, 172 * 1, 170 * 1, 172 * 1), sf::seconds(0.05f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 2, 172 * 1, 170 * 1, 172 * 1), sf::seconds(0.05f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 3, 172 * 1, 170 * 1, 172 * 1), sf::seconds(0.05f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 4, 172 * 1, 170 * 1, 172 * 1), sf::seconds(0.05f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 0, 172 * 2, 170 * 1, 172 * 1), sf::seconds(0.05f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 1, 172 * 2, 170 * 1, 172 * 1), sf::seconds(0.05f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 2, 172 * 2, 170 * 1, 172 * 1), sf::seconds(0.05f));
    slashSprite->addFrame("slash", sf::IntRect(170 * 3, 172 * 2, 170 * 1, 172 * 1), sf::seconds(0.05f));

    CollisionEngine::add("player_hitbox", temp, &player_hitbox);

    //make these line of code exist after the animation
    parent->addOnSpriteAnimationRenderEvent("slash", 0.2f, false, [&](GameObject& go, Scene& scene, const sf::Time& at) {

        Transform* tr_player = go.getData<Transform>(DATA_TYPE::TRANSFORM);

        GameObject* cam_go = scene.get_child("camera");
        float camera_angle{ 0.f };
        if (cam_go) {
            camera_angle = cam_go->getData<CameraData>(DATA_TYPE::CAMERA)->angle;
        }
        const double pi = 3.14159265358979323846;

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite.isFlipX())
            n = -n;
        n *= 10.0f;

        attack_hitbox_start.position = n;
        
        CollisionEngine::add("player_attack_hitbox", tr_player, &attack_hitbox_start);
    });

    parent->addOnSpriteAnimationRenderEvent("slash", 0.7f, false, [&](GameObject& go, Scene& scene, const sf::Time& at) {

        Transform* tr_player = go.getData<Transform>(DATA_TYPE::TRANSFORM);

        GameObject* cam_go = scene.get_child("camera");
        float camera_angle{ 0.f };
        if (cam_go) {
            camera_angle = cam_go->getData<CameraData>(DATA_TYPE::CAMERA)->angle;
        }
        const double pi = 3.14159265358979323846;

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite.isFlipX())
            n = -n;
        n *= 10.0f;

        attack_hitbox_mid.position = n;
        CollisionEngine::remove("player_attack_hitbox");
        CollisionEngine::add("player_attack_hitbox", tr_player, &attack_hitbox_mid);
    });
}

void PlayerSpriteRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer)
{
    //renderer.enableDebugRendering(Debug_rendering::debug_normal);
    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
    PlayerData* player_data = parent->getData<PlayerData>(DATA_TYPE::PLAYER);

	glm::vec3 direction = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY)->direction;
    glm::vec3 position = temp->position;
    
	static float t = 0;
	t += dt.asSeconds();

    playerSprite.update(dt);


    //set player spirte animation
	glm::vec3 dir = glm::vec3(direction.x, direction.y, direction.z);

    if (player_data->curr_state == PlayerData::State::charge) {
        playerSprite.setCurrAnimation("charging");
        playerSprite.enableLoop(true);
    }
    else if (player_data->curr_state == PlayerData::State::jump) {
        if (playerSprite.getCurrAnimation() != "jumping") {
            playerSprite.setCurrAnimation("jumping");
            playerSprite.resetCurrAnimation();
        }
        playerSprite.enableLoop(false);
    }
    else if (player_data->curr_state == PlayerData::State::double_jump) {
        if (playerSprite.getCurrAnimation() != "double-jumping") {
            playerSprite.setCurrAnimation("double-jumping");
            playerSprite.resetCurrAnimation();
        }
        playerSprite.enableLoop(false);
    }

    else if (player_data->curr_state == PlayerData::State::fall) {
        if (playerSprite.getCurrAnimation() != "falling") {
            playerSprite.setCurrAnimation("falling");
            playerSprite.resetCurrAnimation();
        }
        playerSprite.enableLoop(false);
    }

    else if (player_data->curr_state == PlayerData::State::land) {
        playerSprite.setCurrAnimation("landing");
        playerSprite.enableLoop(true);
        float t = playerSprite.getCurrTimeReached().asSeconds() / playerSprite.getCurrTimeDuration().asSeconds();

        if (t >= 1.0f)
            player_data->curr_state = PlayerData::State::idle;
    }else if (glm::length(dir) != 0) {
        playerSprite.setCurrAnimation("run");
    }
    else {
        playerSprite.setCurrAnimation("idle");
    }



    //set player look direction
    GameObject* cam_go = scene.get_child("camera");
    float camera_angle{0.f};
    if (cam_go) {
        camera_angle = cam_go->getData<CameraData>(DATA_TYPE::CAMERA)->angle;
    }
    const double pi = 3.14159265358979323846;
    float angle_under_two_pi = static_cast<float>(static_cast<int>((camera_angle * 180 / pi)) % 360) * (pi / 180);

    glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi/2),0.f, cos(camera_angle + pi/2)));
    if (glm::dot(dir, n) < -std::numeric_limits<float>::epsilon() && isLookingLeft)
    {
        playerSprite.flipX();
        isLookingLeft = false;
    }
    else if (glm::dot(dir, n) > std::numeric_limits<float>::epsilon() && !isLookingLeft) {
        playerSprite.flipX();
        isLookingLeft = true;
    }
    
    renderer.add(&playerSprite, sf::Vector3f(position.x, position.y, position.z), sf::Vector2f(temp->scale.x, temp->scale.y), 0);

    ///////////////////////////////
    // Render planting Animation //
    ///////////////////////////////

    static bool show_plant_animation = false;

    if (player_data->curr_state == PlayerData::State::plant) {
        show_plant_animation = true;
        if (plantSprite->getCurrAnimation() != "plant") {
            plantSprite->setCurrAnimation("plant");
            plantSprite->resetCurrAnimation();
        }
    }

    if (show_plant_animation) {

        plantSprite->update(dt);

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite.isFlipX())
            n = -n;
        n *= 10.0f;
        renderer.add(&*plantSprite, sf::Vector3f(position.x + n.x, position.y + n.y, position.z + n.z), sf::Vector2f(temp->scale.x, temp->scale.y), 0);
    }

    /////////////////////////////
    // Render attack animation //
    /////////////////////////////

    static bool show_attack_animation = false;

    if (player_data->curr_state == PlayerData::State::attack) {
        slashSprite->setCurrAnimation("slash");
        show_attack_animation = true;
    }

    if (show_attack_animation) {

        slashSprite->update(dt);

        float t = slashSprite->getCurrTimeReached().asSeconds() / slashSprite->getCurrTimeDuration().asSeconds();
        if (t >= 1.0f) 
        {
            show_attack_animation = false;
            player_data->curr_state = PlayerData::State::idle;
            slashSprite->resetCurrAnimation();
        }

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite.isFlipX())
            n = -n;
        n *= 10.0f;
        renderer.add(&*slashSprite, sf::Vector3f(position.x + n.x, position.y + n.y, position.z + n.z), sf::Vector2f(temp->scale.x, temp->scale.y), 0);
    }

    renderer.trackSpriteWithTag(&playerSprite, parent->getName());
}
