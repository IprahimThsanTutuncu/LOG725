#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H

#include <thsan/gameObject/component/InputComponent.h>
#include "command/MoveDownCommand.h"
#include "command/MoveLeftCommand.h"
#include "command/MoveRightCommand.h"
#include "command/MoveUpCommand.h"
#include "command/AimCommand.h"
#include "command/StopAimCommand.h"
#include "command/RotateLeftCommand.h"
#include "command/RotateRightCommand.h"
#include "command/jumpCommand.h"
#include "command/PlantCommand.h"
#include "command/ActionCommand.h"
#include "command/AttackCommand.h"
#include "command/RunCommand.h"
#include "command/StopRunCommand.h"

class PlayerInputComponent : public InputComponent
{
    public:
        PlayerInputComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, std::vector<Config::InputAction> inputActions) override;
        virtual std::vector<Command*> handleInput(std::vector<Config::InputAction> inputActions) override;

        ~PlayerInputComponent();

    private:
        Command* left{new MoveLeftCommand()};
        Command* down{new MoveDownCommand()};
        Command* up{new MoveUpCommand()};
        Command* right{new MoveRightCommand()};
        Command* plant{new PlantCommand()};
        Command* jump{new JumpCommand()};
        Command* aim{ new AimCommand()};
        Command* stop_aim{ new StopAimCommand() };
        Command* rotateLeft{ new RotateLeftCommand() };
        Command* rotateRight{ new RotateRightCommand() };
        Command* run{ new RunCommand() };
        Command* stop_run{ new StopRunCommand() };
        Command* action{ new ActionCommand() };
        Command* attack{ new AttackCommand() };
        std::vector<Command*> historyInput;

        PlayerData* player_data;
        BagData* bag_data;
        CharacterStatData* character_stat_data;
        GameObject* healthBar;
};

#endif // PLAYERINPUTCOMPONENT_H
