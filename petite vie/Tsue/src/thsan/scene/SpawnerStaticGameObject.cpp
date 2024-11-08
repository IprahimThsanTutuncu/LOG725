#include <pch.h>

#include "SpawnerStaticGameObject.h"
#include "thsan/gameObject/image/ImageRenderComponent.h"
#include "thsan/gameObject/spriteAnimation/SpriteAnimationRenderComponent.h"

SpawnerStaticGameObject::SpawnerStaticGameObject(PassKey key, Scene* target):
    target(target)
{
    gameObjectFactory = std::make_unique<GameObjectFactory>(target);
}

void SpawnerStaticGameObject::spawn(data::MapData mapData)
{
    spawn(mapData.voxel_terrain);
    spawnMultiple(mapData.image_static);
    spawnMultiple(mapData.sprite_static);
    spawnMultiple(mapData.lights);
}

void SpawnerStaticGameObject::spawn(data::ImageStatic imageStatic)
{
    GameObject* go = gameObjectFactory->createStaticImage(imageStatic.name, imageStatic.path, imageStatic.emission_path, imageStatic.depth_path);

    EditableImageData* editable_image = go->getData<EditableImageData>(DATA_TYPE::EDITABLE_IMAGE);
    Transform* transform = go->getData<Transform>(DATA_TYPE::TRANSFORM);

    editable_image->flipX = imageStatic.flip_x;
    editable_image->flipY = imageStatic.flip_y;
    editable_image->depth_path = imageStatic.depth_path;
    editable_image->emission_path = imageStatic.emission_path;
    editable_image->key_color.r = imageStatic.key_color.r;
    editable_image->key_color.g = imageStatic.key_color.g;
    editable_image->key_color.b = imageStatic.key_color.b;
    editable_image->depth_factor = imageStatic.depth_factor;
    go->setName(imageStatic.name);
    editable_image->path = imageStatic.path;
    transform->position.x = imageStatic.position.x;
    transform->position.y = imageStatic.position.y;
    transform->position.z = imageStatic.position.z;
    transform->rotation = imageStatic.rotation;
    transform->scale.x = imageStatic.scale;
    transform->scale.y = imageStatic.scale;
}

void SpawnerStaticGameObject::spawn(data::SpriteStatic spriteStatic)
{
    //create go
    GameObject* go = gameObjectFactory->createStaticSprite(spriteStatic.name, spriteStatic.path, spriteStatic.has_emission, spriteStatic.has_depth);

    //enit le spriteAnimation Component
    EditableSpriteAnimationData* editable_sprite_animation_data = go->getData<EditableSpriteAnimationData>(DATA_TYPE::EDITABLE_SPRITE_ANIMATION);

    editable_sprite_animation_data->flipX = spriteStatic.flip_x;
    editable_sprite_animation_data->flipY = spriteStatic.flip_y;
    editable_sprite_animation_data->has_depth = spriteStatic.has_depth;
    editable_sprite_animation_data->has_emission = spriteStatic.has_emission;
    editable_sprite_animation_data->key_color.r = spriteStatic.key_color.r;
    editable_sprite_animation_data->key_color.g = spriteStatic.key_color.g;
    editable_sprite_animation_data->key_color.b = spriteStatic.key_color.b;
    editable_sprite_animation_data->path = spriteStatic.path;
    editable_sprite_animation_data->current_animation = spriteStatic.current_animation;
    editable_sprite_animation_data->depth_factor = spriteStatic.depth_factor;


    //init transform comp.
    Transform* transform = go->getData<Transform>(DATA_TYPE::TRANSFORM);

    transform->position.x = spriteStatic.position.x;
    transform->position.y = spriteStatic.position.y;
    transform->position.z = spriteStatic.position.z;
    transform->rotation = spriteStatic.rotation;
    transform->scale.x = spriteStatic.scale;
    transform->scale.y = spriteStatic.scale;
}

void SpawnerStaticGameObject::spawn(data::LightProperties light)
{
    if (light.isPoint) {
        PointLight* pl = target->createPointLight();
        pl->setAmbientColor(sf::Color(
            static_cast<sf::Uint8>(light.ambient.r),
            static_cast<sf::Uint8>(light.ambient.g),
            static_cast<sf::Uint8>(light.ambient.b)
        ));

        pl->setDiffuseColor(sf::Color(
            static_cast<sf::Uint8>(light.diffuse.r),
            static_cast<sf::Uint8>(light.diffuse.g),
            static_cast<sf::Uint8>(light.diffuse.b)
        ));

        pl->setSpecularColor(sf::Color(
            static_cast<sf::Uint8>(light.specular.r),
            static_cast<sf::Uint8>(light.specular.g),
            static_cast<sf::Uint8>(light.specular.b)
        ));

        pl->setPosition(sf::Vector3f(light.position.x, light.position.y, light.position.z));
        pl->enable(light.isEnable);

        pl->setAttenuation(light.attenuationConstant, light.attenuationLinear, light.attenuationQuadratic);
    }
    else if (light.isDirection) {
        target->setDirLight_ambient(sf::Color(
            static_cast<sf::Uint8>(light.ambient.r),
            static_cast<sf::Uint8>(light.ambient.g),
            static_cast<sf::Uint8>(light.ambient.b)
        ));

        target->setDirLight_diffuse(sf::Color(
            static_cast<sf::Uint8>(light.diffuse.r),
            static_cast<sf::Uint8>(light.diffuse.g),
            static_cast<sf::Uint8>(light.diffuse.b)
        ));


        target->setDirLight_specular(sf::Color(
            static_cast<sf::Uint8>(light.specular.r),
            static_cast<sf::Uint8>(light.specular.g),
            static_cast<sf::Uint8>(light.specular.b)
        ));

        float dir_l = light.direction.x * light.direction.x + light.direction.y * light.direction.y + light.direction.z * light.direction.z;
        dir_l = sqrt(dir_l);

        target->setDirLight_direction(sf::Vector3f(static_cast<float>(light.direction.x/ dir_l), static_cast<float>(light.direction.y/ dir_l), static_cast<float>(light.direction.z/ dir_l)));

        if (light.isEnable)
            target->enableDirectionalLight();
        else
            target->disableDirectionalLight();
    }
}

void SpawnerStaticGameObject::spawn(data::VoxelTerrain VoxelTerrain)
{
    VoxelTerrainData* vt = target->get_child("terrain")->getData<VoxelTerrainData>(DATA_TYPE::VOXEL_TERRAIN);

    vt->angle = static_cast<float>(VoxelTerrain.angle);
    vt->height = static_cast<float>(VoxelTerrain.height);
    vt->horizon = static_cast<float>(VoxelTerrain.horizon);
    vt->LOD = static_cast<float>(VoxelTerrain.lod);
    vt->LOD_start_at = static_cast<float>(VoxelTerrain.lod_start_at);
    vt->normal_blurr.x = static_cast<float>(VoxelTerrain.normal_blurr.x);
    vt->normal_blurr.y = static_cast<float>(VoxelTerrain.normal_blurr.y);
    vt->normal_blur_iteration = static_cast<int>(VoxelTerrain.normal_blur_iteration);
    vt->offset_camera_position.x = static_cast<float>(VoxelTerrain.offset_camera_position.x);
    vt->offset_camera_position.y = static_cast<float>(VoxelTerrain.offset_camera_position.y);
    vt->offset_camera_position.z = static_cast<float>(VoxelTerrain.offset_camera_position.z);
    vt->position.x = static_cast<float>(VoxelTerrain.position.x);
    vt->position.y = static_cast<float>(VoxelTerrain.position.y);
    vt->position.z = static_cast<float>(VoxelTerrain.position.z);
    vt->scale_height = static_cast<float>(VoxelTerrain.scale_height);
    vt->screen_height = static_cast<float>(VoxelTerrain.screen_height);
    vt->screen_width = static_cast<float>(VoxelTerrain.screen_width);
    vt->sprite_render_distance = static_cast<float>(VoxelTerrain.sprite_render_distance);
    vt->collision_height_map_path = VoxelTerrain.terrain_collision_height_path;
    vt->collision_mask_map_path = VoxelTerrain.terrain_collision_mask_path;
    vt->color_map_path = VoxelTerrain.terrain_color_path;
    vt->height_map_path = VoxelTerrain.terrain_height_path;
    vt->terrain_render_distance = static_cast<float>(VoxelTerrain.terrain_render_distance);
    vt->shininess = static_cast<float>(VoxelTerrain.shininess);
    vt->FOV = static_cast<int>(VoxelTerrain.FOV);
    vt->blur_type = static_cast<int>(VoxelTerrain.blur_type);
}

void SpawnerStaticGameObject::spawnMultiple(std::vector<data::ImageStatic> images)
{
    for (data::ImageStatic image_static : images)
        spawn(image_static);
}

void SpawnerStaticGameObject::spawnMultiple(std::vector<data::SpriteStatic> sprites)
{
    for (data::SpriteStatic sprite_static : sprites)
        spawn(sprite_static);
}

void SpawnerStaticGameObject::spawnMultiple(std::vector<data::LightProperties> lights)
{
    for (data::LightProperties light_data : lights)
        spawn(light_data);
}