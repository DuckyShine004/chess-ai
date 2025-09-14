#pragma once

#include <vector>

#include <AL/al.h>

#include <sndfile.h>

namespace sound {

class SoundBuffer {
  public:
    SoundBuffer(const SoundBuffer &) = delete;

    SoundBuffer &operator=(const SoundBuffer &) = delete;

    static SoundBuffer &getInstance();

    ALuint addSound(const char *soundPath);

    bool removeSound(const ALuint &sound);

  private:
    SoundBuffer();

    ~SoundBuffer();

    float *getMemoryBuffer(const SF_INFO &soundFileInfo);

    bool setSoundFile(const char *soundPath, SNDFILE *&soundFile, SF_INFO &soundFileInfo);

    bool setSoundFormat(ALenum &format, SNDFILE *soundFile, const SF_INFO &soundFileInfo);

    bool setNumberOfFrames(sf_count_t &numberOfFrames, SNDFILE *soundFile, const SF_INFO &soundFileInfo, float *memoryBuffer);

    bool setSoundBuffer(ALuint &soundBuffer, SNDFILE *soundFile, const SF_INFO &soundFileInfo, ALenum &format, sf_count_t &numberOfFrames, float *memoryBuffer);

    bool isSoundError(ALuint soundBuffer);

    std::vector<ALuint> _sounds;
};

} // namespace sound
