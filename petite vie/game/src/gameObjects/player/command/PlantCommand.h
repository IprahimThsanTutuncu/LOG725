#pragma once

#include <thsan/gameObject/component/Input/Command.h>
#include "../../plant/PlantFactory.h"

class PlantCommand : public Command
{
public:
    PlantCommand();
    ~PlantCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:
    PlantFactory* plantFactory;
    PlayerData* data;
};