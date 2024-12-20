#include "PlayerPhysicComponent.h"


#include <thsan/gameObject/data/DataType.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <thsan/ressourceManager/StateManager.h>
#include <thsan/CollisionEngine/CollisionEngine.h>
#include <glm/gtx/projection.hpp>
#include "states/mainMenu/MainMenuState.h"
#include "gameObjects/plant/PlantFactory.h"

PlayerPhysicComponent::PlayerPhysicComponent()
{
	//ctor
}

void PlayerPhysicComponent::init(Scene& scene)
{
	jump_timer = sf::Time::Zero;
	if (!parent->hasData(DATA_TYPE::PHYSIC_BODY)) {
		parent->setData(DATA_TYPE::PHYSIC_BODY, new physicBodyData());
	}

	HitBox* hitbox = nullptr;
	if (!parent->hasData(DATA_TYPE::HITBOX)) {
		hitbox = new HitBox();
		parent->setData(DATA_TYPE::HITBOX, hitbox);
	}
	else {
		hitbox = parent->getData<HitBox>(DATA_TYPE::HITBOX);
	}

	Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
	temp->position.y = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));
	hitbox->position = glm::vec3(0.f, 0.f, 0.f);
	hitbox->collision_object = Collision_object::real;
	
	//scene.enableDebugRendering(Debug_rendering::debug_normal);
}


void PlayerPhysicComponent::update(Scene& scene,  const sf::Time& dt)
{
	Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
	physicBodyData* phy = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);
	PlayerData* pd = parent->getData<PlayerData>(DATA_TYPE::PLAYER);
	auto* p = parent->getData<PlayerData>(DATA_TYPE::PLAYER);

	phy->friction = 0.95; // avoir une carte de terrain pour determiner les biomes
	const float g = 12.5f; // devrait �tre offert par la scene
	static float t = 0.f;
	static bool isMoving{ false };

	phy->acceleration.x = 0.1f;
	phy->acceleration.z = 0.1f;

	if (p->curr_state != PlayerData::State::attack && p->curr_state != PlayerData::State::charge)
	{
		std::vector<GameObject*> overlapsed_go = CollisionEngine::getAllOverlapingGameObjectWithGroups("player_hitbox", { "enemy" }, true);
		if (overlapsed_go.size() > 0)
		{
			Transform* player_position = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
			Transform* enemy_position = overlapsed_go[0]->getData<Transform>(DATA_TYPE::TRANSFORM);

			glm::vec3 distance = player_position->position - enemy_position->position;
			CharacterStatData* character_stat_data = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
			CharacterStatData* enemy_stat_data = overlapsed_go[0]->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);

			float dammage = enemy_stat_data->atk - character_stat_data->def;
			if (dammage < 1.f)
				dammage = 1.f;

			character_stat_data->curr.hp -= dammage;
			glm::vec3 force_direction = glm::normalize(distance);

			float knockback_strength = glm::length(distance) * 100.f * ( 1 / (1 - phy->friction));

			phy->force = knockback_strength;
			phy->direction = force_direction;
		}

	}

	float weight;
	phy->masse = 0.29f;

	if (!onGround) {
		if (t < sf::seconds(3.f).asSeconds())
			t += dt.asSeconds();
		weight = phy->masse * g * t;
	}
	else 
	{
		t = 0.f;
		weight = phy->masse * g;
	}

	float physical_height_min = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));

	glm::vec3 f_dir = phy->direction;

	if (glm::length(f_dir) != 0) {}

	glm::vec3 f_direction = glm::vec3(0.f);
	if (glm::length(f_dir) != 0) {
		isMoving = true;
		f_dir = glm::normalize(f_dir);
		f_direction = f_dir;
		f_dir *= velocity;
	}
	else
		isMoving = false;

	if (isJumping)
		isMoving = false;

	if (isMoving) {
		velocity.x += phy->acceleration.x * phy->force;
		velocity.z += phy->acceleration.z * phy->force;
		float max_speed = pd->curr_state == PlayerData::State::run ? max_running_speed : max_walking_speed;
		if (glm::length(velocity) > max_speed) {
			velocity = glm::normalize(velocity) * max_speed;
		}
	}
	else if (!isJumping)
		velocity *= phy->friction;

	// verify if the next position isn't
	// a wall, that is his his normal isn't near one
	// and significantly higher.
	// usually would workout, but if it moves by more than 1 pixel
	// it'll just walk on a roof (flat surface)
	// so that's a fix. O(n) btw.

	float next_height = 0;
	float curr_height = 0;

	if (glm::length(f_dir) != 0.f) {
		bool isNextWayHigher = true;
		float tolerated_height_gap = 5.f; // should be specified by the player_data
		do {
			next_height = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x + f_dir.x, temp->position.z + f_dir.z));
			curr_height = temp->position.y;

			if (next_height > curr_height + tolerated_height_gap) {
				f_dir -= f_direction;
				isNextWayHigher = true;
			}
			else
				isNextWayHigher = false;

		} while (isNextWayHigher);

	}

	sf::Vector3f terrain_normal_1 = scene.getNormalOnPosition(sf::Vector2f(temp->position.x + f_dir.x, temp->position.z + f_dir.z));
	sf::Vector3f terrain_normal_2 = scene.getNormalOnPosition(sf::Vector2f(temp->position.x + f_dir.x + 2, temp->position.z + f_dir.z));
	sf::Vector3f terrain_normal_3 = scene.getNormalOnPosition(sf::Vector2f(temp->position.x + f_dir.x - 2, temp->position.z + f_dir.z));
	sf::Vector3f terrain_normal_4 = scene.getNormalOnPosition(sf::Vector2f(temp->position.x + f_dir.x, temp->position.z + f_dir.z + 2));
	sf::Vector3f terrain_normal_5 = scene.getNormalOnPosition(sf::Vector2f(temp->position.x + f_dir.x, temp->position.z + f_dir.z - 2));
	sf::Vector3f terrain_normal =
		terrain_normal_1 * 0.2f
		+ terrain_normal_2 * 0.2f
		+ terrain_normal_3 * 0.2f
		+ terrain_normal_4 * 0.2f
		+ terrain_normal_5 * 0.2f;

	glm::vec3 n = glm::normalize(glm::vec3(terrain_normal.x, terrain_normal.y, terrain_normal.z));

	glm::vec3 f_w = glm::vec3(0.f, -1.f, 0.f) * weight;
	glm::vec3 f = f_dir + f_w;

	glm::vec3 r = glm::cross(glm::cross(n, f), n);
	//haha, r_force comme dans vrak tv :D
	glm::vec3 r_force;

	if (temp->position.y > physical_height_min) {
		r_force = f_w + f_dir * (1.f / glm::length((f_w + 1.f)));
		onGround = false;
	}
	else {
		onGround = true;
		r_force = f_w - glm::proj(f, n) + f_dir; // ;
	}

	if (!isJumping) {
		if (glm::length(r_force) > 0.f) {
			if (pd->curr_state != PlayerData::State::run)
				pd->curr_state = PlayerData::State::walk;
		}
		temp->position += r_force;
	}
	else {
		temp->position.x += r_force.x;
		temp->position.z += r_force.z;
	}


	physical_height_min = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));


	phy->force *= phy->friction;

	if (phy->force <= 0.05) {
		phy->force = 0.f;
		phy->direction = glm::vec3(0, 0, 0);
	}



	if (isDoubleJumping) {
		double_jump_timer += dt;
		float t = double_jump_timer.asSeconds();

		if (t >= sf::seconds(th / 2.f).asSeconds())
			pd->curr_state = PlayerData::State::fall;
		float vel = 2 * h / th;
		float g1 = -(2 * h) / (th * th);
		temp->position.y += (g1 * t + vel);

	}

	else if (phy->isDoubleJump) {
		isDoubleJumping = true;
		double_jump_timer = sf::Time::Zero;
		y01 = temp->position.y;
		phy->isDoubleJump = false;
		//there is no vx2 = glm::length... double jump height is constant in velocity. Formula is respect to time
		pd->curr_state = PlayerData::State::double_jump;
	}

	if (isJumping && !isDoubleJumping) {
		jump_timer += dt;
		float t = jump_timer.asSeconds();

		if (t >= sf::seconds(th / 2.f).asSeconds())
			pd->curr_state = PlayerData::State::fall;
		float vel = 2 * h / th;
		float g1 = -(2 * h) / (th * th);
		temp->position.y += (g1 * t * vx / 2.f + vel * vx / 2.f) * wall_normal.y;
		temp->position.x += wall_normal.x * t / 2.f * vx;
		temp->position.z += wall_normal.z * t / 2.f * vx;
	}
	else if (phy->isJump && !isDoubleJumping) {
		isJumping = true;
		jump_timer = sf::Time::Zero;
		y0 = temp->position.y;
		phy->isJump = false;
		wall_normal = n;
		vx = glm::length(glm::vec2(r_force.x, r_force.z)) + 1.f; // 1.f parce que sinon 0.f et � 0.f y jump po :((
		pd->curr_state = PlayerData::State::jump;
	}

	if (temp->position.y < physical_height_min) {
		temp->position.y = physical_height_min;

		phy->onGround = true;
		isDoubleJumping = false;
		isJumping = false;
		pd->curr_state = PlayerData::State::land;
	}

	if (pd->isAiming) {
		isAiming = true;
		target_aim = scene.get_child(pd->target_name);
		aim_timer = sf::Time::Zero;
		y0 = temp->position.y;
		x0 = temp->position.x;
		z0 = temp->position.z;
	}
	else if (isAiming)
	{
		glm::vec3 target_position = target_aim->getData<Transform>(DATA_TYPE::TRANSFORM)->position;
		glm::vec3 dir = glm::normalize(temp->position - target_position);
		float longeur = glm::length(temp->position - target_position);

		aim_timer += dt;
		float t = aim_timer.asSeconds();

		if (temp->position.y < physical_height_min) {
			temp->position.y = physical_height_min;
		}

		temp->position.x += -dir.x * t * 20.f;
		temp->position.z += -dir.z * t * 20.f;

		if (longeur <= 20.f) 
		{
			isAiming = false;
			phy->isJump = true;

			if (target_aim->hasData(DATA_TYPE::CHARACTER_STAT)) 
			{
				CharacterStatData* pcd = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
				CharacterStatData* cd = target_aim->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
				cd->curr.hp -= pcd->atk * 18.f - cd->def;
			}


			if (target_aim->hasData(DATA_TYPE::PLANT))
			{
				PlantData* plant = target_aim->getData<PlantData>(DATA_TYPE::PLANT);
				plant->current_state = PlantData::State::physical_item;
			}
		}
	}

	///
	/// this part could essentially be put elsewhere.
	/// the code seen on top can be re-usable.
	/// maybe some form of inheritance might help out.
	///

	if (p->curr_state == PlayerData::State::attack) {
		std::vector<GameObject*> overlapsed_go = CollisionEngine::getAllOverlapingGameObjectWithGroups("player_attack_hitbox", { "plant" });

		if (overlapsed_go.size() > 0)
		{
			for (GameObject* go : overlapsed_go)
			{
				if (go->hasData(DATA_TYPE::PLANT)) 
				{
					PlantData* data = go->getData<PlantData>(DATA_TYPE::PLANT);

					if (data->current_state != PlantData::State::physical_item && data->current_stage == PlantData::Stage::ripping)
					{
						PlantFactory pf(&scene);
						GameObject* p = pf.createTrilleRouge();
						PlantData* plantData = p->getData<PlantData>(DATA_TYPE::PLANT);
						plantData->current_state = PlantData::State::physical_item;

						Transform* transformData = p->getData<Transform>(DATA_TYPE::TRANSFORM);
						Transform* goTransformData = go->getData<Transform>(DATA_TYPE::TRANSFORM);
						transformData->position = goTransformData->position + glm::vec3(0.5, 0.0, 0.5);
						transformData->scale = goTransformData->scale;
					}

					data->current_state = PlantData::State::physical_item;
				}
			}
		}

	}

	if (p->curr_state == PlayerData::State::attack) {
		std::vector<GameObject*> overlapsed_go = CollisionEngine::getAllOverlapingGameObjectWithGroups("player_attack_hitbox", { "enemy" });

		if (overlapsed_go.size() > 0) {
			for (GameObject* go : overlapsed_go) {
				if (go->hasData(DATA_TYPE::CHARACTER_STAT)) {
					CharacterStatData* data = go->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
					CharacterStatData* player_data = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
					data->curr.hp -= player_data->atk;
				}
			}
		}

	}

	if (pd->isAiming)
		pd->curr_state = PlayerData::State::charge;


	CharacterStatData* data = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
	if (data->curr.hp <= 0)
	{
		pd->curr_state = PlayerData::State::death;
	}

}

PlayerPhysicComponent::~PlayerPhysicComponent()
{
	//dtor
}
