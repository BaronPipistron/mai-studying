#include "../../include/mobs/Werewolf.h"

Werewolf::Werewolf(const int64_t __x, const int64_t __y) : 
    NPC(NPCType::Werewolf, "Werewolf_" + std::to_string(id++), __x, __y) 
{
}

std::string Werewolf::get_type() 
{
    return "Werewolf";
}

int64_t Werewolf::get_damage_range() const
{
    return 5;
}

void Werewolf::battle_cry() const
{
    std::cout << "ROOAARRHH!!!" << std::endl;
}

bool Werewolf::accept(std::shared_ptr<NPC> visitor) 
{
    if (visitor->fight(std::shared_ptr<Werewolf>(this, [](Werewolf*) {}))) {
        return true;
    }
    return false;
}

bool Werewolf::fight(std::shared_ptr<Outlaw> accepter) 
{
    if (this->get_energy() > accepter->get_energy()) {
        notify_killed(accepter);
        accepter->must_die();
        return true;
    }
    return true;
}

bool Werewolf::fight(std::shared_ptr<Orc> accepter) 
{
    return false;
}

bool Werewolf::fight(std::shared_ptr<Werewolf> accepter) 
{
    return false;
}

void Werewolf::move(const int64_t max_x, const int64_t max_y)
{
    std::lock_guard<std::shared_mutex> lck(_mutex);
    double angle = static_cast<double>(std::rand() % 100) / 100 * 2 * M_PI,
           dist = static_cast<double>(std::rand() % 100) / 100 * 40;

    int shift_x = static_cast<int>(dist * std::cos(angle)),
        shift_y = static_cast<int>(dist * std::sin(angle));

    if ((_x + shift_x >= 0) && (_x + shift_x <= max_x))
        _x += shift_x;
    if ((_y + shift_y >= 0) && (_y + shift_y <= max_y))
        _y += shift_y;
}