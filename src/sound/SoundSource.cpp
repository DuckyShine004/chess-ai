#include "sound/SoundSource.hpp"

namespace sound {

SoundSource::SoundSource() : _buffer(0) {
    alGenSources(1, &this->_source);

    alSourcef(this->_source, AL_GAIN, this->_GAIN);
    alSourcef(this->_source, AL_PITCH, this->_PITCH);

    alSource3f(this->_source, AL_POSITION, this->_POSITION[0], this->_POSITION[1], this->_POSITION[2]);
    alSource3f(this->_source, AL_VELOCITY, this->_VELOCITY[0], this->_VELOCITY[1], this->_VELOCITY[2]);

    alSourcei(this->_source, AL_LOOPING, this->_IS_LOOPING);
    alSourcei(this->_source, AL_BUFFER, this->_buffer);
}

SoundSource::~SoundSource() {
    alDeleteSources(1, &this->_source);
}

bool SoundSource::isStopped() {
    ALint state = AL_STOPPED;

    alGetSourcei(_source, AL_SOURCE_STATE, &state);

    return state == AL_STOPPED || state == AL_INITIAL;
}

void SoundSource::play(const ALuint buffer) {
    if (buffer != this->_buffer) {
        this->_buffer = buffer;

        alSourcei(this->_source, AL_BUFFER, (ALint)this->_buffer);
    }

    alSourcePlay(this->_source);
}

float SoundSource::getPosition() {
    ALfloat position = 0.0f;

    alGetSourcef(this->_source, AL_SEC_OFFSET, &position);

    return position * 1000.0f;
}

} // namespace sound
