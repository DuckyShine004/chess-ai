#pragma once

#include <AL/al.h>

namespace sound {

class SoundSource {
  public:
    SoundSource();

    ~SoundSource();

    bool isStopped();

    void play(const ALuint soundBuffer);

    float getPosition();

  private:
    static constexpr float _GAIN = 1.0f;
    static constexpr float _PITCH = 1.0f;

    static constexpr float _POSITION[3] = { 0, 0, 0 };
    static constexpr float _VELOCITY[3] = { 0, 0, 0 };

    static constexpr bool _IS_LOOPING = false;

    ALuint _source;
    ALuint _buffer;
};

} // namespace sound
