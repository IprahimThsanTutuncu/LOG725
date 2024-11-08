#include "PointPhysicComponent.h"

#include <thsan/gameObject/data/DataType.h>
#include <thsan/gameObject/GameObject.h>

PointPhysicComponent::PointPhysicComponent(glm::vec3 position)
	:position(position)
{
}

void PointPhysicComponent::init( Scene& scene)
{
	if (!parent->hasData(DATA_TYPE::TRANSFORM))
		parent->setData(DATA_TYPE::TRANSFORM, new Transform());

	Transform* transform = m_parent->getData<Transform>(DATA_TYPE::TRANSFORM);
	transform->position = position;
}

void PointPhysicComponent::setPosition(glm::vec3 position)
{
	this->position = position;
	Transform* transform = m_parent->getData<Transform>(DATA_TYPE::TRANSFORM);
	transform->position = position;
}

void PointPhysicComponent::update( Scene& scene, const sf::Time& dt)
{

}

PointPhysicComponent::~PointPhysicComponent()
{
	//dtor
}
