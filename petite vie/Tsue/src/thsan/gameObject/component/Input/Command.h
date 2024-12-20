#ifndef COMMAND_H
#define COMMAND_H

#include <SFML/System/Time.hpp>

#include "thsan/scene/Scene.h"
#include "thsan/gameObject/GameObject.h"


class GameObject;
class Scene;

class  Command
{
    public:
        Command() = default;
        virtual ~Command(){};

        virtual void execute(GameObject &parent, Scene &scene, const sf::Time& dt){};
        virtual void undo(GameObject &parent, Scene &scene, const sf::Time& dt){};

};

#endif // BASECOMMAND_H
