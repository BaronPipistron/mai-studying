#include "../../include/mobs/NPC.h"

using namespace observers;

NPC::NPC(NPCType __type, 
         const std::string& __name, 
         const int64_t __x, 
         const int64_t __y) :
    _type(__type), _name(__name), _x(__x), _y(__y), _alive(true)
{
}

const int64_t NPC::get_x() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return _x;
}

const int64_t NPC::get_y() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return _y;
}

int64_t NPC::get_energy() const 
{
    return std::rand() % 6 + 1;
}

const std::string& NPC::get_name() const
{
    return _name;
}

const bool NPC::is_alive() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return _alive;
}

void NPC::attach(std::shared_ptr<observers::IObserver> observer)
{
    std::lock_guard<std::shared_mutex> lck(_mutex);
    _observers.push_back(observer);
}

void NPC::notify_killed(const std::shared_ptr<NPC> defender)
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (auto& obs : _observers) {
        obs->report_killed(std::shared_ptr<NPC>(this, [](NPC*) {}), defender);
    }
}

bool NPC::near(const std::shared_ptr<NPC> enemy, const int64_t distance) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return (this->_x - enemy->_x) * (this->_x - enemy->_x) +
           (this->_y - enemy->_y) * (this->_y - enemy->_y) <= (distance * distance);
}

void NPC::must_die() 
{
    std::lock_guard<std::shared_mutex> lck(_mutex);
    _alive = false;
}

std::ostream& operator<<(std::ostream& os, const NPC& npc)
{
    os << npc._name << " {x : " << npc._x << ", y : " << npc._y << "}";
    return os;
}

uint16_t NPC::id = 0;