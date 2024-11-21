#pragma once

#include <thsan/gameObject/component/Input/Command.h>

class MoveLeftCommand : public Command
{
    public:
        MoveLeftCommand();
        ~MoveLeftCommand();

        void execute(GameObject &parent, Scene &scene, const sf::Time& dt) override;
        void undo(GameObject &parent, Scene &scene, const sf::Time& dt) override;
};
