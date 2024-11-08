#include <pch.h>

#include "GameObjectFactory.h"

#include "thsan/scene/Scene.h"
#include "thsan/gameObject/GameObject.h"
#include "image/ImageRenderComponent.h"
#include "spriteAnimation/SpriteAnimationRenderComponent.h"
#include "pointLight/PointLightRenderComponent.h"
#include "VolumetricPostProcessing/VolumetricPostProcessingPhysicComponent.h"

GameObjectFactory::GameObjectFactory(Scene* target):
	target(target)
{
}

GameObject* GameObjectFactory::createStaticImage(const std::string& name, const std::string& path, const std::string& emission_path, const std::string& depth_path)
{
	GameObject* go = target->createGameObject(name);
	target->setGameObjectRenderComponent<ImageRenderComponent>(go, path, emission_path, depth_path);
	return go;
}

GameObject* GameObjectFactory::createStaticSprite(const std::string& name, const std::string& spriteSheetMetaDataPath, bool hasEmissionMap, bool hasDepthMap)
{
	GameObject* go = target->createGameObject(name);
	target->setGameObjectRenderComponent<SpriteAnimationRenderComponent>(go, spriteSheetMetaDataPath, hasEmissionMap, hasDepthMap);
	return go;
}

GameObject* GameObjectFactory::createPointLight(const std::string& name, PointLight* pl)
{
	GameObject* go = target->createGameObject(name);
	target->setGameObjectRenderComponent<PointLightRenderComponent>(go, pl);
	return go;
}

GameObject* GameObjectFactory::createVolumetricPostProcess(const std::string& name, GameObject* targeted_go, const HitBox& hb, const VolumetricPostProcessData& vppd)
{
	GameObject* go = target->createGameObject(name);

	//	set les components avant de set un component
	//	pour s'assurer que la variable hb: Hitbox
	//  s'enregistre dans collisionEngine de target: Scene.
	if (!go->hasData(DATA_TYPE::HITBOX))
		go->setData(DATA_TYPE::HITBOX, new HitBox(hb));

	if (!go->hasData(DATA_TYPE::VOLUME_POST_PROCESS))
		go->setData(DATA_TYPE::VOLUME_POST_PROCESS, new VolumetricPostProcessData(vppd));

	target->setGameObjectPhysicComponent<VolumetricPostProcessingPhysicComponent>(go, targeted_go);
	return go;
}
