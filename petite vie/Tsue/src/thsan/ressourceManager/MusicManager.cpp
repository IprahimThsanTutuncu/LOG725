#include "pch.h"
#include "MusicManager.h"

namespace RessourceManager 
{
    std::unordered_map<std::string, std::shared_ptr<sf::Music>> MusicManager::musicCache;
    std::shared_ptr<sf::Music> MusicManager::currentMusic = nullptr;
    float MusicManager::volume = 100.0f;

    void MusicManager::play(const std::string& musicPath, bool loop) {
        currentMusic = get(musicPath);

        if (currentMusic) {
            currentMusic->setLoop(loop);
            currentMusic->setVolume(volume);
            currentMusic->play();
        }
    }

    void MusicManager::pause() {
        if (currentMusic && currentMusic->getStatus() == sf::Music::Playing) {
            currentMusic->pause();
        }
    }

    void MusicManager::stop() 
    {
        if (currentMusic)
        {
            currentMusic->stop();
            currentMusic.reset();
        }
    }

    void MusicManager::setVolume(float newVolume)
    {
        volume = newVolume;
        if (currentMusic) 
        {
            currentMusic->setVolume(volume);
        }
    }

    float MusicManager::getVolume()
    {
        return volume;
    }

    std::shared_ptr<sf::Music> MusicManager::get(const std::string& musicPath)
    {
        auto it = musicCache.find(musicPath);
        if (it != musicCache.end()) {
            return it->second;
        }

        auto music = std::make_shared<sf::Music>();
        if (music->openFromFile(musicPath))
        {
            musicCache[musicPath] = music;
            return music;
        }

        return nullptr;
    }

}
