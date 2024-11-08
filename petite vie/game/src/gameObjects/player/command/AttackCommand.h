#pragma once

#include <thsan/gameObject/component/Input/Command.h>

class AttackCommand : public Command
{
public:
    AttackCommand();
    ~AttackCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:

};