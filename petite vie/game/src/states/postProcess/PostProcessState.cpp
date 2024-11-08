#include "PostProcessState.h"
#include <thsan/ressourceManager/ShaderManager.h>


PostProcessState::PostProcessState(Game* parent, std::shared_ptr<State> previous, Scene* scene) :
    State(parent)
{
    window = &parent->getWindow();
    scene->setRenderTarget(&texture);
    target_state = previous;
    texture.create(parent->getWidth(), parent->getHeight(), parent->getContextSettings());

    s.setTexture(texture.getTexture());
    shader_pp = RessourceManager::ShaderManager::get("media/shader/postProcess/postprocess.vert", "media/shader/postProcess/postprocess.frag");

    this->scene = scene;
}

std::string PostProcessState::getType()
{
    return "postProcess";
}

void PostProcessState::init()
{
}

void PostProcessState::handleEvent(sf::Event event)
{

}

void PostProcessState::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) {
    target_state->input(deltaTime, inputActions);
}

void PostProcessState::update(const sf::Time& deltaTime)
{
    target_state->update(deltaTime);
}

void PostProcessState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    static float t = 0.f;
    t += deltaTime.asSeconds();
    target_state->draw(texture, deltaTime);
    texture.display();
    sf::Shader::bind(&*shader_pp);
    sf::Color ambient = scene->getDirectionalLightProperties().ambient;
    shader_pp->setUniform("texture_depth", scene->getDepthTexture());
    shader_pp->setUniform("directionalLight_ambient",sf::Glsl::Vec4(ambient));
    shader_pp->setUniform("wind_direction",sf::Glsl::Vec2(0.2,0.4));
    shader_pp->setUniform("speed",sf::Glsl::Vec2(0.2,0.5));
    shader_pp->setUniform("width", scene->getRenderResolution().x);
    shader_pp->setUniform("height", scene->getRenderResolution().y);
    shader_pp->setUniform("size_rate", 0.1f);
    shader_pp->setUniform("fg_layers", 2.f);
    shader_pp->setUniform("bg_layers", 2.f);
    shader_pp->setUniform("layer_gap",1.f);

    shader_pp->setUniform("time", t);
    target.draw(s);
    sf::Shader::bind(0);
}



