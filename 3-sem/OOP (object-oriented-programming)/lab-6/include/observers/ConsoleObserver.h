#pragma once

#include <fstream>
#include <memory>

#include "../mobs/NPC.h"

namespace observers {

class ConsoleObserver : public IObserver {
  public:
    ConsoleObserver() = default;

    void report_killed(const NPC&, const NPC&) override;
};

}; // namespace observers