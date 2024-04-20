#pragma once

#include <fstream>
#include <memory>

#include "../mobs/NPC.h"
#include "../mobs/Orc.h"
#include "../mobs/Outlaw.h"
#include "../mobs/Werewolf.h"

extern std::mutex print_mutex, file_mutex;

namespace observers {

class ConsoleObserver : public IObserver {
  public:
    ConsoleObserver() = default;

    void report_killed(const std::shared_ptr<NPC>, const std::shared_ptr<NPC>) override;
};

}; // namespace observers