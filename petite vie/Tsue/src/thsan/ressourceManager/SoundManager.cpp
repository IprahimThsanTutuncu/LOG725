#include "pch.h"

#include "SoundManager.h"

namespace RessourceManager {

    std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> SoundManager::soundBuffers;
    std::unordered_map<std::string, std::shared_ptr<sf::Sound>> SoundManager::activeSounds;

    float SoundManager::volume = 0.f;

    std::shared_ptr<sf::SoundBuffer> SoundManager::get(const std::string& soundPath)
    {
        auto it = soundBuffers.find(soundPath);
        if (it != soundBuffers.end()) {
            return it->second;
        }

        auto buffer = std::make_shared<sf::SoundBuffer>();
        if (buffer->loadFromFile(soundPath)) {
            soundBuffers[soundPath] = buffer;
            return buffer;
        }
        return nullptr;
    }

    void SoundManager::play(const std::string& soundPath) {

        auto buffer = get(soundPath);
        if (!buffer) 
        {
            return;
        }

        auto sound = std::make_shared<sf::Sound>(*buffer);
        sound->setVolume(volume);
        sound->play();
        activeSounds[soundPath] = sound;
    }

    void SoundManager::pause() 
    {
        for (auto& pair : activeSounds)
        {
            pair.second->pause();
        }
    }

    void SoundManager::stop() 
    {
        for (auto& pair : activeSounds) 
        {
            pair.second->stop();
        }
        activeSounds.clear();
    }

    void SoundManager::setVolume(float vol) 
    {
        volume = vol;
    }

    float SoundManager::getVolume()
    {
        return volume;
    }
}
