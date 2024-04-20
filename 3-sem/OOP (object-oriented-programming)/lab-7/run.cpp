#include "bits/stdc++.h"
#include "include/factory/NPCFactory.h"
#include "include/fight/fight.h"

std::ostream& operator<<(std::ostream& out, const set_t& s) {
    for (auto npc : s) {
        out << *npc.get() << std::endl;
    }
    return out;
}

int main() {
    const int MAX_X{100}, MAX_Y{100}, npc_num{50};
    set_t npc_set;
    NPCFactory factory;
    std::shared_ptr<observers::IObserver> console_observer, log_observer;
    console_observer = std::make_shared<observers::ConsoleObserver>();
    log_observer = std::make_shared<observers::LogObserver>();
    std::srand(std::time(0));

    std::cout << "Generating ..." << std::endl
              << std::endl;

    for (size_t i = 0; i < npc_num; ++i) {
        std::shared_ptr<NPC> cur_npc = factory.create_NPC(NPCType(std::rand() % 3 + 1),
                                                          std::rand() % MAX_X,
                                                          std::rand() % MAX_Y);

        cur_npc->attach(console_observer);
        cur_npc->attach(log_observer);
        npc_set.insert(cur_npc);
    }

    std::cout << "Saving ..." << std::endl
              << std::endl;
    factory.save(npc_set, "game_data/npc.txt");

    std::cout << "Loading ..." << std::endl
              << std::endl;
    npc_set = factory.load("game_data/npc.txt");

    std::cout << "Warriors list:\n"
              << npc_set << std::endl;

    std::cout << "Fighting ..." << std::endl
              << std::endl;
    std::cout << "Battle stats ________________________________________________________________" << std::endl
              << std::endl;

    std::thread fight_thread(std::ref(FightManager::get()));

    std::thread move_thread([&npc_set, MAX_X, MAX_Y]() {
            while (!stop_flag) {
                for (std::shared_ptr<NPC> npc : npc_set) {
                        if(npc->is_alive()) {
                            npc->move(MAX_X - 1, MAX_Y - 1);
                        }
                }
                for (std::shared_ptr<NPC> defender : npc_set) {
                    for (std::shared_ptr<NPC> attacker : npc_set) {
                        if (defender != attacker && defender->is_alive() && attacker->is_alive() && defender->near(attacker, attacker->get_damage_range())) {
                            FightManager::get().add_event({defender, attacker});
                        }
                    }
                }
                std::this_thread::sleep_for(1s);
            } });

    int run_time{30};
    while (run_time--) {
        size_t grid{20}, survived{0};
        std::vector<std::vector<std::string>> map(grid, std::vector<std::string>(grid, "0"));
        {
            std::lock_guard<std::mutex> lck(print_mutex);

            for (std::shared_ptr<NPC> npc : npc_set) {
                std::string type = npc->get_type();
                int x = npc->get_x() / 5, y = npc->get_y() / 5;
                if (npc->is_alive()) {
                    if (map[x][y] == "0" || map[x][y] == ".") {
                        if (type == "Orc") {
                            map[x][y] = "Or";
                        } else if (type == "Outlaw") {
                            map[x][y] = "Ot";
                        } else if (type == "Werewolf") {
                            map[x][y] = "Wr";
                        }
                    } else if (map[x][y] == "Or" || map[x][y] == "Ot" || map[x][y] == "Wr") {
                        map[x][y] = "2";
                    } else {
                        map[x][y] = std::to_string(std::stoi(map[x][y]) + 1);
                    }
                    ++survived;
                } else {
                    if (map[x][y] == "0") {
                        map[x][y] = ".";
                    }
                }
            }

            std::cout << "Survived: " << survived << " | Killed: " << npc_num - survived << std::endl;

            for (size_t i = 0; i < grid; ++i) {
                for (size_t j = 0; j < grid; ++j) {
                    std::string str = map[i][j];
                    if (str != "0") {
                        if (str.size() == 1) {
                            std::cout << "[ " << str << "]";
                        } else {
                            std::cout << "[" << str << "]";
                        }
                    } else {
                        std::cout << "[  ]";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl
                      << "Timer: " << run_time << "s" << std::endl;
        }
        if (run_time == 1) {
            stop_flag = true;
        }
        std::this_thread::sleep_for(1s);
    }

    move_thread.join();
    fight_thread.join();

    std::cout << "______________________________________________________________________________" << std::endl
              << std::endl;

    std::cout << "Survivors:" << std::endl;

    for (std::shared_ptr<NPC> npc : npc_set) {
        if (npc->is_alive()) {
            std::cout << *npc.get() << std::endl;
        }
    }

    return 0;
}