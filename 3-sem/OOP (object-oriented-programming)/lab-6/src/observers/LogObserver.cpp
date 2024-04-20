#include "../../include/observers/LogObserver.h"

using namespace observers;

LogObserver::LogObserver() :
    _os(std::ofstream("logs/battle_logs.txt"))
{
}

void LogObserver::report_killed(const NPC& attacker, const NPC& defender) {
    _os << defender << " | killed by | " << attacker << std::endl;
}