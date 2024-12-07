#pragma once

#include <Thsan.h>

class DialogueState;

class MainMenuState : public State
{
public:
    MainMenuState(Game* parent, std::string begin_dialogue_path, int playAtParagrapheNum);

    std::string getType() override;
    void init() override;

    void init(std::string begin_dialogue_path, int playAtParagrapheNum);

    void start() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    ~MainMenuState() = default;

    //these are public for an hack, I am not proud
    int playAtParagrapheNum;
    std::string begin_dialogue_path;

private:
    bool startGameState{ false };

    Scene* m_scene;

    std::string id_dialogue;
    std::string id_dialogue_setting;
    std::shared_ptr<DialogueState> dialogue;
};
