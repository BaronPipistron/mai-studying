#include "../../include/mobs/Outlaw.h"

Outlaw::Outlaw(const int64_t __x, const int64_t __y) : 
    NPC(NPCType::Outlaw, "Outlaw_" + std::to_string(id++), __x, __y) 
{
}

std::string Outlaw::get_type() 
{
    return "Outlaw";
}

int64_t Outlaw::get_damage_range() const
{
    return 10;
}

void Outlaw::battle_cry() const
{
    std::cout << "Bar-rr-a!!!" << std::endl;
}

bool Outlaw::accept(std::shared_ptr<NPC> visitor) 
{
    if (visitor->fight(std::shared_ptr<Outlaw>(this, [](Outlaw*) {}))) {
        return true;
    }
    return false;
}

bool Outlaw::fight(std::shared_ptr<Outlaw> accepter) 
{
    return false;
}

bool Outlaw::fight(std::shared_ptr<Orc> accepter) 
{
    return false;
}

bool Outlaw::fight(std::shared_ptr<Werewolf> accepter) 
{
    if (this->get_energy() > accepter->get_energy()) {
        notify_killed(accepter);
        accepter->must_die();
        return true;
    }
    return true;
}

void Outlaw::move(const int64_t max_x, const int64_t max_y)
{
    std::lock_guard<std::shared_mutex> lck(_mutex);
    double angle = static_cast<double>(std::rand() % 100) / 100 * 2 * M_PI,
           dist = static_cast<double>(std::rand() % 100) / 100 * 10;

    int shift_x = static_cast<int>(dist * std::cos(angle)),
        shift_y = static_cast<int>(dist * std::sin(angle));

    if ((_x + shift_x >= 0) && (_x + shift_x <= max_x))
        _x += shift_x;
    if ((_y + shift_y >= 0) && (_y + shift_y <= max_y))
        _y += shift_y;
}