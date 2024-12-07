#include "GameClearedRenderComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <thsan/graphic/SpriteAnimation.h>
#include <thsan/Game.h>
#include <thsan/ressourceManager/StateManager.h>
#include "states/mainMenu/MainMenuState.h"

GameClearedRenderComponent::GameClearedRenderComponent()

{
}

void GameClearedRenderComponent::init(Scene& scene)
{
    ssbm_fx = this->createSpriteAnimation("media/image/effect/ssbm_fx.png", "ssbm_fx");

    ssbm_fx->setEmissionTexture("media/image/effect/ssbm_fx.png");

    ssbm_fx->setCurrAnimation("complete");
    ssbm_fx->addFrame("complete", sf::IntRect(6, 640, 533, 183), sf::seconds(2.f));
    ssbm_fx->setShininess(32);
    ssbm_fx->setKeyColor(sf::Color(0,255,0, 255));

    parent->addOnSpriteAnimationRenderEvent("complete", 1.f, true, [&](GameObject& go, Scene& scene, const sf::Time& at)
        {
            Game* game = scene.getParent()->getParent();
            game->changeState(RessourceManager::StateManager::create<MainMenuState>(game, "media/data/dialogue/mainmenu_won.txt", 3), true);
        });

}
void GameClearedRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer)
{

    GameObject* go_target = scene.get_child("player");
    if (!go_target)
        return;

    Transform* trans = go_target->getData<Transform>(DATA_TYPE::TRANSFORM);
    if (!trans)
        return;

    SpriteAnimation* sa = renderer.querySpriteAssociateWith("player");
    if (!sa)
        return;

    glm::vec3 position = trans->position;

    constexpr float min_height_offset = 15.0f;
    constexpr float max_height_offset = 100.0f;
    constexpr float height_increment = 5.0f;

    float sprite_height = min_height_offset + (sa->getCurrFrameRect().height * trans->scale.y);

    float height_adjustment = std::round(sprite_height / height_increment) * height_increment;
    height_adjustment = glm::clamp(height_adjustment, min_height_offset, max_height_offset);

    position.y += height_adjustment;

    int alive = 0;
    auto objects = scene.getAllGameObjectFromGroup("enemy");
    if(objects.size() > 0){
    for (auto go : objects)
    {   
        if (go) {
            auto stat = go->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
            if (stat->curr.hp > 0)
            {
                alive++;
            }
        }
    }
    }
    if (alive < 1)
    {
        ssbm_fx->setCurrAnimation("complete");
        ssbm_fx->update(dt);

        renderer.add(ssbm_fx.get(), position, glm::vec2(0.1), 0.f);
        renderer.trackSpriteWithTag(ssbm_fx.get(), parent->getName());
    }
    else {

        ssbm_fx->setCurrAnimation("");
    }
}

