#pragma once

#include "application/manager/Manager.hpp"

namespace application::manager {

class MouseManager final : public Manager {
  public:
    static MouseManager &getInstance();

    void initialise() override;

    bool isClick();

  private:
    MouseManager();

    ~MouseManager();

    bool _isClicking;
};

} // namespace application::manager
