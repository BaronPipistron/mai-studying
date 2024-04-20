#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <set>
#include <shared_mutex>
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
    virtual void report_killed(const std::shared_ptr<NPC>, const std::shared_ptr<NPC>) = 0;
};

}; // namespace observers

class NPC {
  protected:
    NPCType _type;
    std::string _name;
    int64_t _x { 0 }; 
    int64_t _y { 0 };
    bool _alive { true };
    std::vector<std::shared_ptr<observers::IObserver>> _observers;
    mutable std::shared_mutex _mutex;

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
    virtual int64_t get_damage_range() const = 0;
    int64_t get_energy() const;
    const std::string& get_name() const;
    const bool is_alive() const;

    virtual void battle_cry() const = 0;

    virtual bool accept(std::shared_ptr<NPC>) = 0;
    virtual bool fight(std::shared_ptr<Outlaw>) = 0;
    virtual bool fight(std::shared_ptr<Orc>) = 0;
    virtual bool fight(std::shared_ptr<Werewolf>) = 0;

    void attach(std::shared_ptr<observers::IObserver>);
    void notify_killed(const std::shared_ptr<NPC>);

    bool near(const std::shared_ptr<NPC>, const int64_t) const;
    virtual void move(const int64_t, const int64_t) = 0;
    void must_die();

    friend std::ostream& operator<<(std::ostream&, const NPC&);
};