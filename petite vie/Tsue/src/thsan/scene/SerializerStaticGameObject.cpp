#include <pch.h>
#include "serializerStaticGameObject.h"

#include "thsan/scene/Scene.h"
#include "thsan/ressourceManager/MapDataManager.h"
#include "thsan/gameObject/GameObject.h"
#include "thsan/gameObject/data/DataType.h"
#include "thsan/gameObject/data/DataType.h"
#include "thsan/graphic/light/Light.h"
#include "thsan/graphic/light/PointLight.h"
#include "thsan/config/ControlSetting.h"

SerializerStaticGameObject::SerializerStaticGameObject(PassKey key, Scene* target):
    target(target)
{
    
}

data::MapData SerializerStaticGameObject::serialize()
{
    data::MapData mapData;

    //VOXEL TERRAIN
    VoxelTerrainData* voxel_terrain = target->get_child("terrain")->getData<VoxelTerrainData>(DATA_TYPE::VOXEL_TERRAIN);
    mapData.voxel_terrain = serialize(voxel_terrain);

    //CLEAR FOR INIT
    mapData.image_static.clear();
    mapData.sprite_static.clear();
    mapData.lights.clear();

    // EDITABLE IMAGE & SPRITE
    for (GameObject* go : target->get_children()) {
        if (go->hasData(DATA_TYPE::EDITABLE_IMAGE)) {
            EditableImageData* editable_image = go->getData<EditableImageData>(DATA_TYPE::EDITABLE_IMAGE);
            Transform* transform = go->getData<Transform>(DATA_TYPE::TRANSFORM);
            mapData.image_static.push_back(serialize(editable_image, transform, go->getName()));
        }
        else if (go->hasData(DATA_TYPE::EDITABLE_SPRITE_ANIMATION)) {
            if (go->hasData(DATA_TYPE::EDITABLE_SPRITE_ANIMATION)) {
                EditableSpriteAnimationData* editable_sprite = go->getData<EditableSpriteAnimationData>(DATA_TYPE::EDITABLE_SPRITE_ANIMATION);
                Transform* transform = go->getData<Transform>(DATA_TYPE::TRANSFORM);
                mapData.sprite_static.push_back(serialize(editable_sprite, transform, go->getName()));
            }
        }

    }

    //DIRECTIONAL LIGHT
    LightProperties light = target->getDirectionalLightProperties();
    mapData.lights.push_back(serialize(&light, "directional"));

    //POINT LIGHTS
    unsigned counter = 0;
    for (PointLight* pl : target->getPointLights()) {
        light = pl->getLightProperties();
        mapData.lights.push_back(serialize(&light, "pointLight" + counter++));
    }

    return mapData;
}

data::VoxelTerrain SerializerStaticGameObject::serialize(const VoxelTerrainData* voxelTerrainData)
{
	data::VoxelTerrain vt;

    vt.angle = voxelTerrainData->angle;
    vt.height = voxelTerrainData->height;
    vt.horizon = voxelTerrainData->horizon;
    vt.lod = voxelTerrainData->LOD;
    vt.lod_start_at = voxelTerrainData->LOD_start_at;
    vt.normal_blurr.x = voxelTerrainData->normal_blurr.x;
    vt.normal_blurr.y = voxelTerrainData->normal_blurr.y;
    vt.normal_blur_iteration = voxelTerrainData->normal_blur_iteration;
    vt.offset_camera_position.x = voxelTerrainData->offset_camera_position.x;
    vt.offset_camera_position.y = voxelTerrainData->offset_camera_position.y;
    vt.offset_camera_position.z = voxelTerrainData->offset_camera_position.z;
    vt.position.x = voxelTerrainData->position.x;
    vt.position.y = voxelTerrainData->position.y;
    vt.position.z = voxelTerrainData->position.z;
    vt.scale_height = voxelTerrainData->scale_height;
    vt.screen_height = voxelTerrainData->screen_height;
    vt.screen_width = voxelTerrainData->screen_width;
    vt.sprite_render_distance = voxelTerrainData->sprite_render_distance;
    vt.terrain_collision_height_path = voxelTerrainData->collision_height_map_path;
    vt.terrain_collision_mask_path = voxelTerrainData->collision_mask_map_path;
    vt.terrain_color_path = voxelTerrainData->color_map_path;
    vt.terrain_height_path = voxelTerrainData->height_map_path;
    vt.terrain_render_distance = voxelTerrainData->terrain_render_distance;
    vt.shininess = voxelTerrainData->shininess;
    vt.FOV = voxelTerrainData->FOV;
    vt.blur_type = voxelTerrainData->blur_type;

    return vt;
}

data::ImageStatic SerializerStaticGameObject::serialize(const EditableImageData* editableImageData, Transform* transform, const std::string& name)
{
    data::ImageStatic image_static;

    image_static.flip_x = editableImageData->flipX;
    image_static.flip_y = editableImageData->flipY;
    image_static.depth_path = editableImageData->depth_path;
    image_static.emission_path = editableImageData->emission_path;
    image_static.key_color.r = editableImageData->key_color.r;
    image_static.key_color.g = editableImageData->key_color.g;
    image_static.key_color.b = editableImageData->key_color.b;
    image_static.name = name;
    image_static.path = editableImageData->path;
    image_static.position.x = transform->position.x;
    image_static.position.y = transform->position.y;
    image_static.position.z = transform->position.z;
    image_static.rotation = transform->rotation;
    image_static.scale = transform->scale.x;
    image_static.depth_factor = editableImageData->depth_factor;

    return image_static;
}

data::SpriteStatic SerializerStaticGameObject::serialize(const EditableSpriteAnimationData* editableSpriteAnimationData, Transform* transform, const std::string& name)
{
    data::SpriteStatic sprite_static;

    sprite_static.flip_x = editableSpriteAnimationData->flipX;
    sprite_static.flip_y = editableSpriteAnimationData->flipY;
    sprite_static.has_depth = editableSpriteAnimationData->has_depth;
    sprite_static.has_emission = editableSpriteAnimationData->has_emission;
    sprite_static.key_color.r = editableSpriteAnimationData->key_color.r;
    sprite_static.key_color.g = editableSpriteAnimationData->key_color.g;
    sprite_static.key_color.b = editableSpriteAnimationData->key_color.b;
    sprite_static.name = name;
    sprite_static.path = editableSpriteAnimationData->path;
    sprite_static.position.x = transform->position.x;
    sprite_static.position.y = transform->position.y;
    sprite_static.position.z = transform->position.z;
    sprite_static.rotation = transform->rotation;
    sprite_static.scale = transform->scale.x;
    sprite_static.current_animation = editableSpriteAnimationData->current_animation;
    sprite_static.depth_factor = editableSpriteAnimationData->depth_factor;

    return sprite_static;
}

data::LightProperties SerializerStaticGameObject::serialize(const LightData* lightData, const std::string& name)
{
    data::LightProperties light_data;
    light_data.name = name;
    light_data.ambient.r = lightData->lightProperties.ambient.r;
    light_data.ambient.g = lightData->lightProperties.ambient.g;
    light_data.ambient.b = lightData->lightProperties.ambient.b;
    light_data.ambient.a = lightData->lightProperties.ambient.a;

    light_data.diffuse.r = lightData->lightProperties.diffuse.r;
    light_data.diffuse.g = lightData->lightProperties.diffuse.g;
    light_data.diffuse.b = lightData->lightProperties.diffuse.b;
    light_data.diffuse.a = lightData->lightProperties.diffuse.a;

    light_data.specular.r = lightData->lightProperties.specular.r;
    light_data.specular.g = lightData->lightProperties.specular.g;
    light_data.specular.b = lightData->lightProperties.specular.b;
    light_data.specular.a = lightData->lightProperties.specular.a;

    light_data.attenuationConstant = lightData->lightProperties.attenuationConstant;
    light_data.attenuationLinear = lightData->lightProperties.attenuationLinear;
    light_data.attenuationQuadratic = lightData->lightProperties.attenuationQuadratic;

    light_data.isCursor = lightData->lightProperties.isCursor;
    light_data.isDirection = lightData->lightProperties.isDirection;
    light_data.isEnable = lightData->lightProperties.isEnable;
    light_data.isPoint = lightData->lightProperties.isPoint;
    light_data.isSpot = lightData->lightProperties.isSpot;

    light_data.position.x = lightData->lightProperties.position.x;
    light_data.position.y = lightData->lightProperties.position.y;
    light_data.position.z = lightData->lightProperties.position.z;

    light_data.direction.x = lightData->lightProperties.direction.x;
    light_data.direction.y = lightData->lightProperties.direction.y;
    light_data.direction.z = lightData->lightProperties.direction.z;

    return light_data;
}

data::LightProperties SerializerStaticGameObject::serialize(const LightProperties* light, const std::string& name)
{
    data::LightProperties light_data;
    light_data.name = name;
    light_data.ambient.r = light->ambient.r;
    light_data.ambient.g = light->ambient.g;
    light_data.ambient.b = light->ambient.b;
    light_data.ambient.a = light->ambient.a;

    light_data.diffuse.r = light->diffuse.r;
    light_data.diffuse.g = light->diffuse.g;
    light_data.diffuse.b = light->diffuse.b;
    light_data.diffuse.a = light->diffuse.a;

    light_data.specular.r = light->specular.r;
    light_data.specular.g = light->specular.g;
    light_data.specular.b = light->specular.b;
    light_data.specular.a = light->specular.a;

    light_data.attenuationConstant = light->attenuationConstant;
    light_data.attenuationLinear = light->attenuationLinear;
    light_data.attenuationQuadratic = light->attenuationQuadratic;

    light_data.isCursor = light->isCursor;
    light_data.isDirection = light->isDirection;
    light_data.isEnable = light->isEnable;
    light_data.isPoint = light->isPoint;
    light_data.isSpot = light->isSpot;

    light_data.position.x = light->position.x;
    light_data.position.y = light->position.y;
    light_data.position.z = light->position.z;

    light_data.direction.x = light->direction.x;
    light_data.direction.y = light->direction.y;
    light_data.direction.z = light->direction.z;

    return light_data;
}
