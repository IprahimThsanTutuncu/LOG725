#pragma once
#include <thsan/gameObject/component/Input/Command.h>

class JumpCommand : public Command
{
public:
    JumpCommand();
    ~JumpCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:
    bool isJumping;
};