#include "HealthBarRenderComponent.h"
#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <thsan/graphic/SpriteAnimation.h>

HealthBarRenderComponent::HealthBarRenderComponent(std::string target, sf::Color color):
	background("media/image/decoration/rect.png"),
	foreground("media/image/decoration/rect.png")

{
	targetName = target;
	this->fg_color = color;
}

void HealthBarRenderComponent::init(Scene& scene)
{
	GameObject* go_target = scene.get_child(targetName);

	if (!go_target)
		return;

	CharacterStatData* stat = go_target->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);

	if (!stat)
		return;

	float hp = stat->hp;

	background.setEmissionTexture("media/image/decoration/rect.png");
	foreground.setEmissionTexture("media/image/decoration/rect.png");

	background.setCurrAnimation("100");
	background.addFrame("100", sf::IntRect(0, 0, (hp / 100.f) * 32.f, 4), sf::seconds(0.1f));

	foreground.setCurrAnimation("100");
	foreground.addFrame("100", sf::IntRect(0, 0, (hp / 100.f) * 32.f, 4), sf::seconds(0.1f));

	background.setShininess(2);
	foreground.setShininess(2);


	for (int i = 0; i < 100; i++)
	{
		std::string id = std::to_string(i);
		foreground.setCurrAnimation(id);
		float curr = i;
		foreground.addFrame(id, sf::IntRect(0, 0, (i / 100.f) * 32.f, 4), sf::seconds(0.1f));
	}

	bg_color = sf::Color(fg_color.r / 2, fg_color.g / 2, fg_color.b / 2);
}
void HealthBarRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer)
{
    GameObject* go_target = scene.get_child(targetName);
    if (!go_target)
        return;

    CharacterStatData* stat = go_target->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
    if (!stat)
        return;

    Transform* trans = go_target->getData<Transform>(DATA_TYPE::TRANSFORM);
    if (!trans)
        return;

    SpriteAnimation* sa = renderer.querySpriteAssociateWith(targetName);
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

    background.setCurrAnimation("100");
    int curr_hp = static_cast<int>(stat->curr.hp);
    if (curr_hp < 0)
        curr_hp = 0;
    if (curr_hp > 100)
        curr_hp = 100;
    foreground.setCurrAnimation(std::to_string(curr_hp));

    background.setColor(bg_color);
    foreground.setColor(fg_color);

    background.update(dt);
    foreground.update(dt);

    //renderer.add(&background, position);
    int alive = 0;
    auto objects = scene.getAllGameObjectFromGroup("enemy");
    if (objects.size() > 0) {
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
    if(alive > 0)
        renderer.add(&foreground, position);
}

