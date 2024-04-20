#pragma once

#include "NPC.h"
#include "Outlaw.h"
#include "Orc.h"

class Werewolf : public NPC {
  public:
    Werewolf(const int64_t, const int64_t);

    std::string get_type() override;

    void battle_cry() const override;

    bool accept(std::shared_ptr<NPC>) override;
    bool fight(Outlaw&) override;
    bool fight(Orc&) override;
    bool fight(Werewolf&) override;
};
