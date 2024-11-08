#include "pch.h"

#include "VolumetricPostProcessingPhysicComponent.h"

#include "thsan/CollisionEngine/CollisionEngine.h"
#include "thsan/util/helperMath.h"
#include "thsan/graphic/PostProcess.h"

VolumetricPostProcessingPhysicComponent::VolumetricPostProcessingPhysicComponent(GameObject* target):
	target(target)
{
}

void VolumetricPostProcessingPhysicComponent::init(Scene& scene)
{
	if (!parent->hasData(DATA_TYPE::HITBOX)) {
		hitbox = new HitBox();
		parent->setData(DATA_TYPE::HITBOX, hitbox);
	}
	else
		hitbox = parent->getData<HitBox>(DATA_TYPE::HITBOX);

	if (!parent->hasData(DATA_TYPE::VOLUME_POST_PROCESS)) {
		vol_pp_data = new VolumetricPostProcessData();
		parent->setData(DATA_TYPE::VOLUME_POST_PROCESS, vol_pp_data);
	}
	else
		vol_pp_data = parent->getData<VolumetricPostProcessData>(DATA_TYPE::VOLUME_POST_PROCESS);

	transform = parent->getData<Transform>(DATA_TYPE::TRANSFORM);

	pp = scene.getPostProcess();
	CollisionEngine::add(parent, transform, hitbox);
}

void VolumetricPostProcessingPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
	if (!locked) {
		if (target) {
			if (CollisionEngine::overlap(parent, target) && !isOverlapping) {
				isOverlapping = true;
				for (const auto& pair : vol_pp_data->effects_and_opacity) {
					if (isFirstInterlapsing || isInterpolationFullyDone) {
						default_opacity[pair.first] = pp->getOpacity(pair.first);
						isFirstInterlapsing = false;
						isInterpolationFullyDone = false;
					}
					enable_then_init_interpolate(pair.first, pair.second,
								vol_pp_data->ease, vol_pp_data->interpolation,
								vol_pp_data->in_interpolation_duration);
				}
			}
			else if (!CollisionEngine::overlap(parent, target) && isOverlapping) {
				isOverlapping = false;
				for (const auto& pair : vol_pp_data->effects_and_opacity) {
					init_interpolate_then_disable(pair.first,
						vol_pp_data->ease, vol_pp_data->interpolation,
						vol_pp_data->out_interpolation_duration);
				}
			}
		}

		interpolate_effect(dt);
	}
}

void VolumetricPostProcessingPhysicComponent::interpolate_effect(const sf::Time& dt)
{
	static bool isLastWhereElapsedEqualDuration = false;

	if (!isInterpolating) {
		return;
	}
	
	if (elapsed_time > duration) {
		isLastWhereElapsedEqualDuration = true;
		elapsed_time = duration;
	}

	float t{1.0};

	if (duration != sf::Time::Zero)
		t = interpolate<float>(ease, interpolate_type, elapsed_time.asSeconds() / duration.asSeconds());
	else 
		isInterpolating = false;
	
	static int count = 0;
	count++;

	for (auto& val : curr_opacity) {
		val.second = (1.0f - t) * begin_opacity[val.first] + t * final_opacity[val.first];
		pp->setOpacity(val.first, val.second);

	}

	if (isLastWhereElapsedEqualDuration) {
		isLastWhereElapsedEqualDuration = false;
		isInterpolating = false;
		isInterpolationFullyDone = true;
		elapsed_time = sf::Time::Zero;
	}
	else
		this->elapsed_time += dt;
}

void VolumetricPostProcessingPhysicComponent::init_interpolate(PostProcess::Effect effect, float final_opacity, Ease e, InterpolationType it, sf::Time duration)
{
	begin_opacity[effect] = pp->getOpacity(effect);
	curr_opacity[effect] = pp->getOpacity(effect);
	this->final_opacity[effect] = final_opacity;
	this->ease = ease;
	this->elapsed_time = sf::Time::Zero;
	interpolate_type = it;
	this->duration = duration;
	isInterpolating = true;
}

void VolumetricPostProcessingPhysicComponent::enable_then_init_interpolate(PostProcess::Effect effect, float final_opacity, Ease e, InterpolationType it, sf::Time duration)
{
	init_interpolate(effect, final_opacity, e, it, duration);
	if (!pp->isEnable()) { 
		started_enable[effect] = false;
		pp->enable_effect(effect, curr_opacity[effect]);
	}else
		started_enable[effect] = true;

	isMarkedForDisabling = false;
}


void VolumetricPostProcessingPhysicComponent::init_interpolate_then_disable(PostProcess::Effect effect, Ease e, InterpolationType it, sf::Time duration)
{
	init_interpolate(effect, default_opacity[effect], e, it, duration);
	if(!started_enable[effect])
		isMarkedForDisabling = true;
	else
		isMarkedForDisabling = false;
}

bool VolumetricPostProcessingPhysicComponent::locked = false;
