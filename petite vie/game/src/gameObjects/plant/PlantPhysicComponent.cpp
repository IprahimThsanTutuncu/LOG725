#include "PlantPhysicComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/CollisionEngine/CollisionEngine.h>

#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

PlantPhysicComponent::PlantPhysicComponent()
{
}

void PlantPhysicComponent::init(Scene& scene)
{
	if (parent->hasData(DATA_TYPE::PLANT))
		plant_data = parent->getData<PlantData>(DATA_TYPE::PLANT);
	else {
		plant_data = new PlantData();
		parent->setData(DATA_TYPE::PLANT, plant_data);
	}

	if (!parent->hasData(DATA_TYPE::PHYSIC_BODY))
		parent->setData(DATA_TYPE::PHYSIC_BODY, new physicBodyData());

	if (!parent->hasData(DATA_TYPE::HITBOX))
		parent->setData(DATA_TYPE::HITBOX, new HitBox());

	timer = sf::Time::Zero;

	isPhysicalObject = plant_data->current_state == PlantData::State::physical_item;

	Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

	float physical_height_min = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));
	onGround = temp->position.y > physical_height_min;

	random_val = glm::linearRand<float>(0.5f, 1.0f);
	random_sign = glm::linearRand<float>(-1.f, 1.0f);
	random_sign = random_sign < 0.f ? -1.f : 1.f;

	scene.addToGroups(parent, { "plant" });

	hitbox = parent->getData<HitBox>(DATA_TYPE::HITBOX);
	hitbox->position = glm::vec3(0, 0, 0);
	hitbox->spheres.push_back(Sphere{ 10.f, glm::vec3() });
	hitbox->collision_object = Collision_object::real;

	CollisionEngine::add(parent, temp, hitbox);
}

void PlantPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
	/////////////////////////
	/// Physic on falling ///
	/////////////////////////

	if (plant_data->current_state == PlantData::State::physical_item) 
	{

		if (!isPhysicalObject) {
			isPhysicalObject = true;
			onGround = false;
			timer = sf::Time::Zero;
		}

		timer += dt;
		Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
		physicBodyData* phy = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
		HitBox* hitbox = parent->getData<HitBox>(DATA_TYPE::HITBOX);

		phy->friction = 0.95; // avoir une carte de terrain pour determiner les biomes
		const float g = 9.81; // devrait être offert par la scene
		static float t = 0.f;

		float weight = 0.f;
		phy->masse = 0.02f;
		float physical_height_min = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));

		if (!onGround && !isNearPlayer) {
			if (firstFrameNotOnGround) {
				original.x = temp->position.z;
				original.y = temp->position.y;
				firstFrameNotOnGround = false;
			}

			if (t < sf::seconds(3.f).asSeconds())
				t += dt.asSeconds();
			weight = phy->masse * 9.81f * t;

			std::function<float(float)> func = [](float t) {
				return t - 2.f * t * t;
			};

			sf::Time duration = sf::seconds(1.f);
			float delta_s = timer.asSeconds() / duration.asSeconds();
			delta_s > 1.0f ? delta_s = 1.0f : delta_s;
			delta_s < 0.f ? delta_s = 0.f: delta_s;
			float s = interpolate(Ease::out, InterpolationType::bounce, delta_s);

			//set new position
			// 1.25 arc length de la courbe parametric déf. par y(t)t - 2.f * t * t et x(t) = 0.f
			float position_y = original.y + ((original.y - physical_height_min) * func(s));
			temp->position.y = position_y;			
		}
		else {
			firstFrameNotOnGround = true;

			if (!isNearPlayer) {
				std::vector<GameObject*> all_go = CollisionEngine::getAllClosestGameObjectWithinRadius(temp, 100.f, -10.f, 10.f, Collision_object::real);
				player = nullptr;

				for (GameObject* go : all_go)
					if (go->getName() == "player") {
						player = go;
						break;
					}

				if (player) {
					isNearPlayer = true;
					timer = sf::Time::Zero;
					original = temp->position;
				}
			}
			
			if (isNearPlayer) 
			{
				Transform* transform_player = player->getData<Transform>(DATA_TYPE::TRANSFORM);
				glm::vec3 position = player->getData<Transform>(DATA_TYPE::TRANSFORM)->position;
				position.y += (scene.querySpriteRectAssociateWith("player").height / 2.f) * transform_player->scale.y;
				sf::Time duration = sf::seconds(0.2f);
				float delta_s = timer.asSeconds() / duration.asSeconds();
				delta_s > 1.0f ? delta_s = 1.0f : delta_s;
				delta_s < 0.f ? delta_s = 0.f : delta_s;
				float s = interpolate(Ease::in, InterpolationType::cubic, delta_s);

				if(delta_s != 1.0f)
					temp->position = original * (1.f-s) + position * s;
				else
				{
					BagData* bd = player->getData<BagData>(DATA_TYPE::BAG);
					bd->addPlant(*plant_data);
					scene.delete_child(parent->getName());
				}
			}

		}

		onGround = temp->position.y <= physical_height_min;

		if (temp->position.y < physical_height_min)
			temp->position.y = physical_height_min;
	}

	else if (plant_data->current_state == PlantData::State::planted)
	{
		Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

		float physical_height_min = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));
		temp->position.y = physical_height_min;

		if (plant_data->current_stage == PlantData::Stage::seed)
			plant_data->current_stage == PlantData::Stage::sprout;

		int next = static_cast<int>(plant_data->current_stage) + 1;
		if (next == static_cast<int>(PlantData::Stage::LAST))
			next -= 1;
		else {
			timer += dt;
		}
		PlantData::Stage next_stage = static_cast<PlantData::Stage>(next);

		float t = timer.asSeconds() / plant_data->growth_time.asSeconds();
		if (plant_data->getGrowth(next_stage) < t)
			plant_data->current_stage = next_stage;
	}

}

PlantPhysicComponent::~PlantPhysicComponent()
{
}
