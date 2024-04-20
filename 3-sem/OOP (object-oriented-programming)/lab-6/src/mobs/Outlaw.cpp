#include "../../include/mobs/Outlaw.h"

Outlaw::Outlaw(const int64_t __x, const int64_t __y) : 
    NPC(NPCType::Outlaw, "Outlaw_" + std::to_string(id++), __x, __y) 
{
}

std::string Outlaw::get_type() 
{
    return "Outlaw";
}

void Outlaw::battle_cry() const
{
    std::cout << "Bar-rr-a!!!" << std::endl;
}

bool Outlaw::accept(std::shared_ptr<NPC> visitor) 
{
    if (visitor->fight(*this)) {
        this->_alive = false;
        return true;
    }

    return false;
}

bool Outlaw::fight(Outlaw& accepter) 
{
    return false;
}

bool Outlaw::fight(Orc& accepter) 
{
    return false;
}

bool Outlaw::fight(Werewolf& accepter) 
{
    notify_killed(accepter);
    return true;
}