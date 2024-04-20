#include <gtest/gtest.h>

#include "../include/factory/NPCFactory.h"
#include "../include/mobs/Orc.h"
#include "../include/mobs/Outlaw.h"
#include "../include/mobs/Werewolf.h"
#include "../include/observers/ConsoleObserver.h"
#include "../include/observers/LogObserver.h"
#include "bits/stdc++.h"

TEST(NPCFactoryTest, Create_NPC) {
    NPCFactory factory;
    bool test = true;
    try {
        std::shared_ptr<NPC> Orc = factory.create_NPC(NPCType::Orc, 1, 2);
        std::shared_ptr<NPC> Outlaw = factory.create_NPC(NPCType::Outlaw, 3, 4);
        std::shared_ptr<NPC> Werewolf = factory.create_NPC(NPCType::Werewolf, 5, 6);
    } catch (...) {
        test = false;
    }
    EXPECT_TRUE(test);
}

TEST(NPCFactoryTest, Save) {
    NPCFactory factory;
    set_t s;
    bool test = true;
    try {
        s.insert(factory.create_NPC(NPCType::Orc, 1, 2));
        s.insert(factory.create_NPC(NPCType::Outlaw, 3, 4));
        s.insert(factory.create_NPC(NPCType::Werewolf, 5, 6));
        factory.save(s, "game_data/test.txt");
    } catch (...) {
        test = false;
    }
    EXPECT_TRUE(test);
}

TEST(NPCFactoryTest, Load) {
    NPCFactory factory;
    set_t s;
    bool test = true;
    try {
        s = factory.load("game_data/test.txt");
    } catch (...) {
        test = false;
    }
    EXPECT_TRUE(test);
}

TEST(NPCTest, OutputOperator) {
    std::shared_ptr<NPC> npc = std::make_shared<Orc>(1, 2);
    std::stringstream expected_output;
    expected_output << "Orc_9 {x : 1, y : 2}";

    std::stringstream actual_output;
    actual_output << *npc.get();

    EXPECT_EQ(expected_output.str(), actual_output.str());
}

TEST(NPCTest, Getters) {
    NPCFactory factory;
    std::shared_ptr<NPC> Orc = factory.create_NPC(NPCType::Orc, 1, 2);
    std::shared_ptr<NPC> Outlaw = factory.create_NPC(NPCType::Outlaw, 3, 4);
    std::shared_ptr<NPC> Werewolf = factory.create_NPC(NPCType::Werewolf, 5, 6);

    EXPECT_EQ(Orc->get_type(), "Orc");
    EXPECT_EQ(Outlaw->get_type(), "Outlaw");
    EXPECT_EQ(Werewolf->get_type(), "Werewolf");

    EXPECT_TRUE(Orc->is_alive());
    EXPECT_TRUE(Outlaw->is_alive());
    EXPECT_TRUE(Werewolf->is_alive());

    EXPECT_EQ(Orc->get_x(), 1);
    EXPECT_EQ(Outlaw->get_x(), 3);
    EXPECT_EQ(Werewolf->get_x(), 5);
    EXPECT_EQ(Orc->get_y(), 2);
    EXPECT_EQ(Outlaw->get_y(), 4);
    EXPECT_EQ(Werewolf->get_y(), 6);
}

TEST(NPCTest, Near) {
    NPCFactory factory;
    std::shared_ptr<NPC> Orc = factory.create_NPC(NPCType::Orc, 1, 1);
    std::shared_ptr<NPC> Outlaw = factory.create_NPC(NPCType::Outlaw, 50, 1);
    std::shared_ptr<NPC> Werewolf = factory.create_NPC(NPCType::Werewolf, 100, 1);

    EXPECT_FALSE(Orc->near(Outlaw, 10));
    EXPECT_TRUE(Orc->near(Outlaw, 60));
    EXPECT_FALSE(Orc->near(Werewolf, 90));
    EXPECT_TRUE(Orc->near(Werewolf, 110));
}

TEST(NPCTest, Attach) {
    NPCFactory factory;
    std::shared_ptr<NPC> Orc = factory.create_NPC(NPCType::Orc, 1, 1);
    std::shared_ptr<observers::IObserver> console_observer, file_observer;
    console_observer = std::make_shared<observers::ConsoleObserver>();
    file_observer = std::make_shared<observers::LogObserver>();
    bool test = true;
    try {
        Orc->attach(console_observer);
        Orc->attach(file_observer);
    } catch (...) {
        test = false;
    }
    EXPECT_TRUE(test);
}

TEST(OrcTest, ConstructorAndGetters) {
    Orc Orc(1, 2);
    EXPECT_EQ(Orc.get_x(), 1);
    EXPECT_EQ(Orc.get_y(), 2);
    EXPECT_TRUE(Orc.is_alive());
    EXPECT_EQ(Orc.get_type(), "Orc");
}

TEST(OrcTest, Visitors) {
    NPCFactory factory;
    std::shared_ptr<NPC> Orc = factory.create_NPC(NPCType::Orc, 1, 2);
    std::shared_ptr<NPC> Outlaw = factory.create_NPC(NPCType::Outlaw, 3, 4);
    std::shared_ptr<NPC> Werewolf = factory.create_NPC(NPCType::Werewolf, 5, 6);

    EXPECT_FALSE(Orc->accept(Orc));
    EXPECT_FALSE(Orc->accept(Outlaw));
    EXPECT_FALSE(Orc->accept(Werewolf));
    EXPECT_TRUE(Orc->is_alive());
}

TEST(OutlawTest, ConstructorAndGetters) {
    Outlaw Outlaw(1, 2);
    EXPECT_EQ(Outlaw.get_x(), 1);
    EXPECT_EQ(Outlaw.get_y(), 2);
    EXPECT_TRUE(Outlaw.is_alive());
    EXPECT_EQ(Outlaw.get_type(), "Outlaw");
}

TEST(OutlawTest, Visitors) {
    NPCFactory factory;
    std::shared_ptr<NPC> Orc = factory.create_NPC(NPCType::Orc, 1, 2);
    std::shared_ptr<NPC> Outlaw = factory.create_NPC(NPCType::Outlaw, 3, 4);
    std::shared_ptr<NPC> Werewolf = factory.create_NPC(NPCType::Werewolf, 5, 6);

    EXPECT_FALSE(Outlaw->accept(Outlaw));
    EXPECT_TRUE(Outlaw->accept(Werewolf));
    EXPECT_FALSE(Outlaw->is_alive());
    EXPECT_TRUE(Outlaw->accept(Orc));
    EXPECT_FALSE(Outlaw->is_alive());
}

TEST(WerewolfTest, ConstructorAndGetters) {
    Werewolf Werewolf(1, 2);
    EXPECT_EQ(Werewolf.get_x(), 1);
    EXPECT_EQ(Werewolf.get_y(), 2);
    EXPECT_TRUE(Werewolf.is_alive());
    EXPECT_EQ(Werewolf.get_type(), "Werewolf");
}

TEST(WerewolfTest, Visitors) {
    NPCFactory factory;
    std::shared_ptr<NPC> Orc = factory.create_NPC(NPCType::Orc, 1, 2);
    std::shared_ptr<NPC> Outlaw = factory.create_NPC(NPCType::Outlaw, 3, 4);
    std::shared_ptr<NPC> Werewolf = factory.create_NPC(NPCType::Werewolf, 5, 6);
    std::shared_ptr<NPC> Werewolf_ = factory.create_NPC(NPCType::Werewolf, 5, 6);

    EXPECT_FALSE(Werewolf->accept(Orc));
    EXPECT_TRUE(Werewolf->is_alive());
    EXPECT_TRUE(Werewolf->accept(Outlaw));
    EXPECT_FALSE(Werewolf->is_alive());
    EXPECT_FALSE(Werewolf_->accept(Werewolf));
    EXPECT_TRUE(Werewolf_->is_alive());
    remove("game_data/battle_logs.txt");
    remove("game_data/test.txt");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}