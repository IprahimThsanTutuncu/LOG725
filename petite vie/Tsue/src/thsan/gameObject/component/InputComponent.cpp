#include "pch.h"

#include "InputComponent.h"

#include "thsan/config/ControlSetting.h"
#include "thsan/gameObject/GameObject.h"
#include "thsan/gameObject/component/Input/Command.h"


InputComponent::~InputComponent()
{
    for(Command* cmd: commandHistory)
        delete cmd;
    commandHistory.clear();
}

void InputComponent::undo(Scene& scene, const sf::Time& dt)
{
    commandHistory[currCommand]->undo(*parent, scene, dt);
    currCommand--;
}

void InputComponent::redo(Scene& scene,  const sf::Time& dt)
{
    commandHistory[currCommand]->execute(*parent, scene, dt);
    currCommand++;
}
