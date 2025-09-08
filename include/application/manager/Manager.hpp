#pragma once

namespace application::manager {

class Manager {
  public:
    Manager(const Manager &) = delete;

    Manager &operator=(const Manager &) = delete;

    virtual void initialise() = 0;

  protected:
    Manager();

    ~Manager();
};

} // namespace application::manager
