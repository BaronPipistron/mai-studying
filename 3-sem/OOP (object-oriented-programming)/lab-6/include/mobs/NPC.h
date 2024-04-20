#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../types/NPCType.h"

class NPC;
class Outlaw;
class Orc;
class Werewolf;

using set_t = std::set<std::shared_ptr<NPC>>;

namespace observers {

class IObserver {
  public:
    IObserver() = default;
    virtual void report_killed(const NPC&, const NPC&) = 0;
};

}; // namespace observers

class NPC {
  protected:
    NPCType _type;
    std::string _name;
    int64_t _x { 0 }; 
    int64_t _y { 0 };
    bool _alive;
    std::vector<std::shared_ptr<observers::IObserver>> _observers;

    static uint16_t id;
  
    NPC(NPCType,
        const std::string&,
        const int64_t, 
        const int64_t);
  
  public:
    virtual ~NPC() = default;

    virtual std::string get_type() = 0;
    const int64_t get_x() const;
    const int64_t get_y() const;
    const std::string& get_name() const;
    const bool is_alive() const;

    virtual void battle_cry() const = 0;

    virtual bool accept(std::shared_ptr<NPC>) = 0;
    virtual bool fight(Outlaw&) = 0;
    virtual bool fight(Orc&) = 0;
    virtual bool fight(Werewolf&) = 0;

    void attach(std::shared_ptr<observers::IObserver>);
    void notify_killed(const NPC&) const;

    bool near(const std::shared_ptr<NPC>, const int64_t) const;

    friend std::ostream& operator<<(std::ostream&, const NPC&);
};

