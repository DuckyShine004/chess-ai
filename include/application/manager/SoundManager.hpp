#pragma once

#include <map>

#include <AL/al.h>

#include "application/manager/Manager.hpp"

#include "sound/SoundSource.hpp"

#include "engine/Engine.hpp"

namespace application::manager {

enum EffectType : int {
    CAPTURE = 0,
    CASTLE,
    CHECK,
    CHECKMATE,
    MOVE,
    PROMOTION,
    STALEMATE,
    CAT,
};

inline constexpr int POOL_SIZE = 8;

inline constexpr const char *SOUND_EFFECT_PATH = "resources/sounds/effects/";

// clang-format off
inline constexpr const char *EFFECTS[] = {
    "capture.wav",
    "castle.wav",
    "check.wav",
    "checkmate.wav",
    "move.wav",
    "promotion.wav",
    "stalemate.wav",
    "cat.mp3"
};
// clang-format on

class SoundManager final : public Manager {
  public:
    static SoundManager &getInstance();

    void initialise() override;

    void playEffect(EffectType effectType);

    void playMoveEffect(engine::Engine &engine, uint16_t move);

  private:
    SoundManager();

    ~SoundManager();

    sound::SoundSource &getFreeSourceFromPool();

    std::map<EffectType, ALuint> _effects;

    sound::SoundSource _pool[POOL_SIZE];
};

} // namespace application::manager
