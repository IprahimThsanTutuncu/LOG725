#pragma once
#include <thsan/gameObject/component/Input/Command.h>

class AimCommand : public Command
{
public:
    AimCommand();
    ~AimCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:
    bool first_aim{true};
    void show_bar();
    void hide_bar();
    std::shared_ptr<PostProcess> postProcess;
};