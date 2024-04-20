#include "../../include/observers/ConsoleObserver.h"

std::mutex print_mutex;

using namespace observers;

void ConsoleObserver::report_killed(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender) {
    std::lock_guard<std::mutex> lck(print_mutex);
    std::cout << *defender.get() << " | killed by | " << *attacker.get() << std::endl;
}