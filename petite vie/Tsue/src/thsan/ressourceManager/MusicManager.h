#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <SFML/Audio.hpp>

namespace RessourceManager {

    class MusicManager {
    public:
        static void play(const std::string& musicPath, bool loop = false);

        static void pause();

        static void stop();

        static void setVolume(float volume);
        static float getVolume();

    private:
        static std::shared_ptr<sf::Music> get(const std::string& musicPath);

        static std::unordered_map<std::string, std::shared_ptr<sf::Music>> musicCache;

        static std::shared_ptr<sf::Music> currentMusic;
        static float volume;
    };

}
