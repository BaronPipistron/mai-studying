#include "../../include/observers/ConsoleObserver.h"

using namespace observers;

void ConsoleObserver::report_killed(const NPC& attacker, const NPC& defender) {
    std::cout << defender << " | killed by | " << attacker << std::endl;
}