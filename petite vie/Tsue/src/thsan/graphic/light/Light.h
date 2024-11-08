#pragma once

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

struct LightProperties{
    bool isEnable;
    bool isDirection;
    bool isPoint;
    bool isSpot;
    bool isCursor;

    sf::Vector3f position;
    sf::Vector3f direction;

    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    sf::Color ambient;
    sf::Color diffuse;
    sf::Color specular;
};