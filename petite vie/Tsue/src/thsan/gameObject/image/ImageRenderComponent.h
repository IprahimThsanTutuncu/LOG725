#pragma once

#include "thsan/graphic/SpriteAnimation.h"
#include "thsan/gameObject/component/RenderComponent.h"

#include <string>

class EditableImageData;

class ImageRenderComponent : public RenderComponent
{
public:
    ImageRenderComponent(const std::string& path, const std::string& emissionPath = "", const std::string& depthPath = ""); // "" -> mean that's here's no map

    void setDepthMap(const std::string& path);
    void setEmissionMap(const std::string& path);

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D) override;

    ~ImageRenderComponent();
private:
    EditableImageData* editableImageData;
    SpriteAnimation image;
    std::string path;
};