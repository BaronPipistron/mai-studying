#include "../../include/mobs/Orc.h"

Orc::Orc(const int64_t __x, const int64_t __y) : 
    NPC(NPCType::Orc, "Orc_" + std::to_string(id++), __x, __y) 
{
}

std::string Orc::get_type() 
{
    return "Orc";
}

void Orc::battle_cry() const
{
    std::cout << "lok'tar Ogar!!!" << std::endl;
}

bool Orc::accept(std::shared_ptr<NPC> visitor) 
{
    if (visitor->fight(*this)) {
        this->_alive = false;
        return true;
    }

    return false;
}

bool Orc::fight(Outlaw& accepter) 
{
    notify_killed(accepter);
    return true;
}

bool Orc::fight(Orc& accepter) 
{
    return false;
}

bool Orc::fight(Werewolf& accepter) 
{
    return false;
}