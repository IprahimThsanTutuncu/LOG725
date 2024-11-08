#pragma once

#include <thsan/gameObject/component/Input/Command.h>

class Scene;
class GameObject;

class RotateLeftCommand : public Command
{
public:
    RotateLeftCommand();
    ~RotateLeftCommand();

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
};
