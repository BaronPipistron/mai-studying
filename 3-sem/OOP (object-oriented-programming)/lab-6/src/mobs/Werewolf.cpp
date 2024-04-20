#include "../../include/mobs/Werewolf.h"

Werewolf::Werewolf(const int64_t __x, const int64_t __y) : 
    NPC(NPCType::Werewolf, "Werewolf_" + std::to_string(id++), __x, __y) 
{
}

std::string Werewolf::get_type() 
{
    return "Werewolf";
}

void Werewolf::battle_cry() const
{
    std::cout << "ROOAARRHH!!!" << std::endl;
}

bool Werewolf::accept(std::shared_ptr<NPC> visitor) 
{
    if (visitor->fight(*this)) {
        this->_alive = false;
        return true;
    }

    return false;
}

bool Werewolf::fight(Outlaw& accepter) 
{
    notify_killed(accepter);
    return true;
}

bool Werewolf::fight(Orc& accepter) 
{
    return false;
}

bool Werewolf::fight(Werewolf& accepter) 
{
    return false;
}