#include "../../include/observers/LogObserver.h"

using namespace observers;

std::mutex file_mutex;

LogObserver::LogObserver() :
    _os(std::ofstream("logs/battle_logs.txt"))
{
}

void LogObserver::report_killed(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender) {
    std::lock_guard<std::mutex> lck(file_mutex);
    _os << *defender.get() << " | killed by | " << *attacker.get() << std::endl;
}