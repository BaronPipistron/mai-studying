#include "../../include/factory/NPCFactory.h"

std::shared_ptr<NPC> NPCFactory::create_NPC(NPCType type, const int64_t x, const int64_t y) 
{
    std::shared_ptr<NPC> result;
    switch (type) {
    case NPCType::Orc :
        result = std::make_shared<Orc>(x, y);
        break;
    case NPCType::Outlaw :
        result = std::make_shared<Outlaw>(x, y);
        break;
    case NPCType::Werewolf :
        result = std::make_shared<Werewolf>(x, y);
        break;

    default:
        break;
    }

    return result;
}

void NPCFactory::save(const set_t& s, const std::string& file_name) {
    std::ofstream out(file_name);
    out << s.size() << std::endl;
    for (auto& npc : s) {
        out << npc->get_type() << std::endl
            << npc->get_x() << std::endl
            << npc->get_y() << std::endl;
    }

    out.flush();
    out.close();
}

set_t NPCFactory::load(const std::string& file_name) {
    set_t result;
    std::ifstream in(file_name);
    std::shared_ptr<observers::IObserver> console_observer, file_observer;
    console_observer = std::make_shared<observers::ConsoleObserver>();
    file_observer = std::make_shared<observers::LogObserver>();

    if (in.good() && in.is_open()) {
        int count;
        in >> count;
        
        while (count--) {
            std::shared_ptr<NPC> npc;
            int64_t x, y;
            std::string type;

            in >> type;
            if (type == "Orc") {
                in >> x >> y;
                npc = std::make_shared<Orc>(x, y);
                result.insert(npc);
            } else if (type == "Outlaw") {
                in >> x >> y;
                npc = std::make_shared<Outlaw>(x, y);
                result.insert(npc);
            } else if (type == "Werewolf") {
                in >> x >> y;
                npc = std::make_shared<Werewolf>(x, y);
                result.insert(npc);
            }
            npc->attach(console_observer);
            npc->attach(file_observer);
        }
    }

    return result;
}