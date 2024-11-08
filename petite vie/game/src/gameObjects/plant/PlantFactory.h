#pragma once

class Scene;
class GameObject;

class PlantFactory {

public:
	PlantFactory(Scene* target);
	~PlantFactory() = default;
	GameObject* createTrilleRouge();
	
private:
	Scene* target;
};