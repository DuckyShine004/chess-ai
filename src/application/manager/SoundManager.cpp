#include "application/manager/SoundManager.hpp"

#include "sound/SoundBuffer.hpp"

using namespace sound;

namespace application::manager {

SoundManager::SoundManager() = default;

SoundManager::~SoundManager() = default;

SoundManager &SoundManager::getInstance() {
    static SoundManager instance;

    return instance;
}

void SoundManager::initialise() {
    int length = sizeof(EFFECTS) / sizeof(EFFECTS[0]);

    for (int index = 0; index < length; ++index) {
        std::string effectPath = SOUND_EFFECT_PATH + std::string(EFFECTS[index]);

        ALuint soundId = SoundBuffer::getInstance().addSound(effectPath.c_str());

        EffectType effectType = static_cast<EffectType>(index);

        this->_effects[effectType] = soundId;
    }
}

void SoundManager::playEffect(EffectType effectType) {
    SoundSource &source = this->getFreeSourceFromPool();

    ALuint soundId = this->_effects[effectType];

    source.play(soundId);
}

// TODO: Handle sound source better
SoundSource &SoundManager::getFreeSourceFromPool() {
    for (SoundSource &soundSource : this->_pool) {
        if (soundSource.isStopped()) {
            return soundSource;
        }
    }

    return this->_pool[0];
}

} // namespace application::manager
