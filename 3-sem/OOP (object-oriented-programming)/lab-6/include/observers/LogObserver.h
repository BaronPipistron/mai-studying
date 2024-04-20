#pragma once

#include <fstream>
#include <memory>

#include "../mobs/NPC.h"

namespace observers {

class LogObserver : public IObserver {
  private:
    std::ofstream _os;

  public:
    LogObserver();

    void report_killed(const NPC&, const NPC&) override;
};

}; // namespace observers