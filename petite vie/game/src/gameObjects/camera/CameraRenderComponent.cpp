#include "CameraRenderComponent.h"

#include <thsan/scene/Scene.h>
#include <thsan/gameObject/GameObject.h>
#include <thsan/gameObject/data/DataType.h>
#include <thsan/graphic/RendererPseudo3D.h>

CameraRenderComponent::CameraRenderComponent()
{

}

void CameraRenderComponent::init(Scene& scene)
{
	cam_data = parent->getData<CameraData>(DATA_TYPE::CAMERA);
}

void CameraRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D)
{
	static auto renderer_tags = scene.getAllTags();
	renderer_tags = scene.getAllTags();
	PlayerData* player_data = scene.get_child("player")->getData<PlayerData>(DATA_TYPE::PLAYER);
	glm::vec3 player_position = scene.get_child("player")->getData<Transform>(DATA_TYPE::TRANSFORM)->position;
	float middle = scene.getRenderResolution().y / 2.f;



}


CameraRenderComponent::~CameraRenderComponent()
{

}
