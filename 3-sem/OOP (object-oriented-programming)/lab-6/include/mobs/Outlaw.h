#pragma once

#include "NPC.h"
#include "Orc.h"
#include "Werewolf.h"

class Outlaw : public NPC {
  public:
    Outlaw(const int64_t, const int64_t);

    std::string get_type() override;

    void battle_cry() const override;

    bool accept(std::shared_ptr<NPC>) override;
    bool fight(Outlaw&) override;
    bool fight(Orc&) override;
    bool fight(Werewolf&) override;
};
