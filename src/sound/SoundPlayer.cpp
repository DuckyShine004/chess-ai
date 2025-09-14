#include "sound/SoundPlayer.hpp"

#include <AL/al.h>

namespace sound {

SoundPlayer &SoundPlayer::getInstance() {
    static SoundPlayer instance;

    return instance;
}

SoundPlayer::SoundPlayer() = default;

SoundPlayer::~SoundPlayer() {
    alcMakeContextCurrent(nullptr);

    alcDestroyContext(this->_context);

    alcCloseDevice(this->_device);
};

void SoundPlayer::initialise() {
    const ALCchar *deviceName = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);

    this->_device = alcOpenDevice(deviceName);

    if (!this->_device) {
        throw("Failed to get sound device");
    }

    this->_context = alcCreateContext(this->_device, nullptr);

    if (!this->_context) {
        throw("Failed to set sound context");
    }

    ALCboolean isContextCurrent = alcMakeContextCurrent(this->_context);

    if (!isContextCurrent) {
        throw("Failed to make sound context current");
    }
}

} // namespace sound
