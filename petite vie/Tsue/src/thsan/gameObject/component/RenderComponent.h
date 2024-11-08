#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <functional>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

class GameObject;
class RendererPseudo3D;
class SpriteAnimation;
class Scene;

class RenderComponent
{
public:
    RenderComponent() = default;
    virtual ~RenderComponent() = default;

    virtual void init(Scene& scene){};
    virtual void start(Scene& scene, RendererPseudo3D& renderer){};
    virtual void update(Scene& scene, const sf::Time& dt, sf::RenderTarget &target, RendererPseudo3D& renderer){};
    virtual void onDelete(Scene& scene) {};

    bool isActive() { return enabled; }
    void disable() { enabled = false; }
    void enable() { enabled = true; }

protected:
    struct SpriteAnimationEvents {
        std::string animation;
        float at; // 0 < at < 1
        bool call_once;
        bool was_called;
        std::function<void(GameObject&, Scene&, const sf::Time&)> func;
    };

    GameObject* parent;

    void addSpriteAnimationEvent(const std::string& animation, float at, bool call_once, std::function<void(GameObject&, Scene&, const sf::Time&)> func);
    void addSpriteAnimationEvent(const std::string& sprite_id, const std::string& animation, float at, bool call_once, std::function<void(GameObject&, Scene&, const sf::Time&)> func);
    std::shared_ptr<SpriteAnimation> createSpriteAnimation(const std::string& path, const std::string& sprite_id);
    void callEvent(Scene& scene, const sf::Time& dt);
    std::unordered_map<std::string, std::vector<SpriteAnimationEvents>> spriteAnimationEventContainer;
    std::unordered_map<std::string, std::shared_ptr<SpriteAnimation>> spriteAnimations;

    friend GameObject;
    friend Scene;
private:
    bool enabled{ true };
};

