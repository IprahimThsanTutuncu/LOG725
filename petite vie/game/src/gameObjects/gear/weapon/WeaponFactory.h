#pragma once

class Scene;
class GameObject;

class WeaponFactory {

public:
	WeaponFactory(Scene* target);
	~WeaponFactory() = default;
	GameObject* createFaucille();


private:
	Scene* target;
};