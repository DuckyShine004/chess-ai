#pragma once

#include <AL/alc.h>

namespace sound {

class SoundPlayer {
  public:
    SoundPlayer(const SoundPlayer &) = delete;

    SoundPlayer &operator=(const SoundPlayer &) = delete;

    static SoundPlayer &getInstance();

    void initialise();

  private:
    SoundPlayer();

    ~SoundPlayer();

    ALCdevice *_device;

    ALCcontext *_context;
};

} // namespace sound
