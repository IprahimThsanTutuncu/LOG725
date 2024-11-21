#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <SFML/Audio.hpp>

namespace RessourceManager {

    class SoundManager {
    public:
        static void play(const std::string& soundPath);
        static void pause();
        static void stop();
        static void setVolume(float volume);

    private:
        static std::shared_ptr<sf::SoundBuffer> get(const std::string& soundPath);

        static std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
        static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> activeSounds;
    };
}
