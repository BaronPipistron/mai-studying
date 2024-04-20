#include "../../include/mobs/Orc.h"

Orc::Orc(const int64_t __x, const int64_t __y) : 
    NPC(NPCType::Orc, "Orc_" + std::to_string(id++), __x, __y) 
{
}

std::string Orc::get_type() 
{
    return "Orc";
}

int64_t Orc::get_damage_range() const
{
    return 10;
}

void Orc::battle_cry() const
{
    std::cout << "lok'tar Ogar!!!" << std::endl;
}

bool Orc::accept(std::shared_ptr<NPC> visitor) 
{
    if (visitor->fight(std::shared_ptr<Orc>(this, [](Orc*) {}))) {
        return true;
    }
    return false;
}

bool Orc::fight(std::shared_ptr<Outlaw> accepter) 
{
    if (this->get_energy() > accepter->get_energy()) {
        notify_killed(accepter);
        accepter->must_die();
        return true;
    }
    return true;
}

bool Orc::fight(std::shared_ptr<Orc> accepter) 
{
    return false;
}

bool Orc::fight(std::shared_ptr<Werewolf> accepter) 
{
    return false;
}

void Orc::move(const int64_t max_x, const int64_t max_y)
{
    std::lock_guard<std::shared_mutex> lck(_mutex);
    double angle = static_cast<double>(std::rand() % 100) / 100 * 2 * M_PI,
           dist = static_cast<double>(std::rand() % 100) / 100 * 20;

    int shift_x = static_cast<int>(dist * std::cos(angle)),
        shift_y = static_cast<int>(dist * std::sin(angle));

    if ((_x + shift_x >= 0) && (_x + shift_x <= max_x))
        _x += shift_x;
    if ((_y + shift_y >= 0) && (_y + shift_y <= max_y))
        _y += shift_y;
}