#pragma once

#include <fstream>

#include "../mobs/NPC.h"
#include "../mobs/Orc.h"
#include "../mobs/Outlaw.h"
#include "../mobs/Werewolf.h"
#include "../observers/ConsoleObserver.h"
#include "../observers/LogObserver.h"

class NPCFactory final {
  public:
    NPCFactory() = default;
    virtual ~NPCFactory() = default;

    std::shared_ptr<NPC> create_NPC(NPCType, const int64_t, const int64_t);

    void save(const set_t&, const std::string&);
    set_t load(const std::string&);
};