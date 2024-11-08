#include "WeaponFactory.h"

#include "faucille/FaucillePhysicComponent.h"
#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>

WeaponFactory::WeaponFactory(Scene* target)
{
}

GameObject* WeaponFactory::createFaucille()
{
    static int faucille_id = 0;
    GameObject* faucille = target->createGameObject("faucille" + faucille_id++);

    target->setGameObjectPhysicComponent<FaucillePhysicComponent>(
        faucille,
        [](GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat) {
            stat.atk = 5.0;
            stat.speed = -1.0;
        },
        [](GameObject& parent, GameObject& target, Scene& scene, CharacterStatData& stat) {

        }
    );

    return faucille;
}
