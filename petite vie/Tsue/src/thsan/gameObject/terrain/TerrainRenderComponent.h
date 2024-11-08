#pragma once

#include "thsan/gameObject/component/RenderComponent.h"
#include "thsan/graphic/VoxelTerrain.h"

#include <string>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class TerrainRenderComponent : public RenderComponent
{
    public:
        TerrainRenderComponent(std::string height_map_path, std::string color_map_path, std::string collisionHeight_map_path, std::string collisionMask_map_path);

        void init(Scene& scene) override;
        void start(Scene& scene, RendererPseudo3D& rendererPseudo3D) override;
        void update(Scene& scene, const sf::Time& dt, sf::RenderTarget &target, RendererPseudo3D& rendererPseudo3D) override;
        ~TerrainRenderComponent();
    private:
        sf::Texture heightmap;
        sf::Texture colormap;
        VoxelTerrain m_VoxelTerrain;

        std::string color_map_path;
        std::string height_map_path;
        std::string collisionHeight_map_path;
        std::string collisionMask_map_path;

};

