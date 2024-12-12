#pragma once

#include <thsan/gameObject/component/Input/Command.h>

class PauseCommand : public Command
{
public:
    PauseCommand();
    ~PauseCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:

};