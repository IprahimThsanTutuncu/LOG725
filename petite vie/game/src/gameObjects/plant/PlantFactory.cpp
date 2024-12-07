#include "PlantFactory.h"
#include "gameObjects/plant/PlantPhysicComponent.h"
#include "gameObjects/plant/PlantRenderComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/gameObject/component/NativeScriptComponent.h>

PlantFactory::PlantFactory(Scene* target):
    target(target)
{

}

GameObject* PlantFactory::createTrilleRouge()
{
    static int counter_id = 0;
    GameObject* go = target->createGameObject("trilleRouge" + std::to_string(counter_id));
    PlantData* plant_data{ nullptr };
    if (go->hasData(DATA_TYPE::PLANT))
        plant_data = go->getData<PlantData>(DATA_TYPE::PLANT);
    else {
        plant_data = new PlantData();
        go->setData(DATA_TYPE::PLANT, plant_data);
    }
    plant_data->name = "trille Rouge";
    plant_data->hp = 100.f;

    target->setGameObjectPhysicComponent<PlantPhysicComponent>(go);
    target->setGameObjectRenderComponent<PlantRenderComponent>(go, "media/image/plant/fleur/fleur_1_test.json");

    class FlowerBehaviour : public NativeScriptComponent {

        void init(Scene& scene) override
        {

        };
        void update(Scene& scene, const sf::Time& dt) override
        {

        };

        void onDelete(Scene& scene) override 
        {

        };
    };

    go->setNativeScript<FlowerBehaviour>();
    
    counter_id++;
    return go;
}
