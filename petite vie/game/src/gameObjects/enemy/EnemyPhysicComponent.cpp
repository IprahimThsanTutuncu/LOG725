#include "EnemyPhysicComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <thsan/CollisionEngine/CollisionEngine.h>

EnemyPhysicComponent::EnemyPhysicComponent()
{
    // Constructor initialization
}

void EnemyPhysicComponent::init(Scene& scene)
{
    if (!parent->hasData(DATA_TYPE::PHYSIC_BODY)) {
        parent->setData(DATA_TYPE::PHYSIC_BODY, new physicBodyData());
    }

    if (!parent->hasData(DATA_TYPE::HITBOX)) {
        parent->setData(DATA_TYPE::HITBOX, new HitBox());
    }

    charState = nullptr;
    if (!parent->hasData(DATA_TYPE::CHARACTER_STAT)) {
        charState = new CharacterStatData();
        parent->setData(DATA_TYPE::CHARACTER_STAT, charState);
    }
    else {
        charState = parent->getData<CharacterStatData>(DATA_TYPE::CHARACTER_STAT);
    }

    scene.addToGroups(parent, { "enemy" });

    hitbox = parent->getData<HitBox>(DATA_TYPE::HITBOX);
    hitbox->position = glm::vec3(0, 0, 0);
    hitbox->spheres.push_back(Sphere{10.f, glm::vec3()});
    hitbox->collision_object = Collision_object::real;

    charState->atk = 3.f;
    charState->def = 1.f;
    charState->hp = 100.f;
    charState->curr.hp = 100;
    charState->speed = 15.f;

    Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

    CollisionEngine::add(parent, temp, hitbox);

}

void EnemyPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
	Transform* temp = parent->getData<Transform>(DATA_TYPE::TRANSFORM);
	physicBodyData* phy = parent->getData<physicBodyData>(DATA_TYPE::PHYSIC_BODY);

	phy->friction = 0.95; // avoir une carte de terrain pour determiner les biomes
	const float g = 12.5f; // devrait être offert par la scene
	static float t = 0.f;
	static bool isMoving{ false };

	phy->acceleration.x = 0.1f;
	phy->acceleration.z = 0.1f;

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


	if (isMoving) {
		velocity.x += phy->acceleration.x * phy->force;
		velocity.z += phy->acceleration.z * phy->force;

		if (glm::length(velocity) > max_running_speed) 
		{
			velocity = glm::normalize(velocity) * max_running_speed;
		}
	}

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

	temp->position.x += r_force.x;
	temp->position.z += r_force.z;
	
	physical_height_min = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));


	phy->force *= phy->friction;

	if (phy->force <= 0.05) {
		phy->force = 0.f;
		phy->direction = glm::vec3(0, 0, 0);
	}

	temp->position.y = physical_height_min;


	if (temp->position.y < physical_height_min)
	{
		temp->position.y = physical_height_min;
		phy->onGround = true;
	}

    if (charState->curr.hp <= 0)
    {
        scene.delete_child(parent->getName());
    }
}

EnemyPhysicComponent::~EnemyPhysicComponent()
{
    // Destructor cleanup
}
