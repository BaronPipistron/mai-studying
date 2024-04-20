#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <iostream>
#include <string>

class DataBase;

class RowTable{
    friend class DataBase;
private:
    std::string owner_surname;
    size_t num_processors;
    std::string processor_type;
    size_t memory_sz;
    std::string video_controller_type;
    size_t video_memory_sz;
    std::string hard_driver_type;
    size_t num_hard_drivers;
    size_t hard_driver_memory;
    size_t num_integrated_controllers;
    size_t num_external_devices;
    std::string operating_system;

public:
    RowTable() = default;
    RowTable(const std::string& owner_surname, const size_t& num_processors, const std::string& processor_type, const size_t& memory_sz,
            const std::string& video_controller_type, const size_t& video_memory_sz, const std::string& hard_driver_type, 
            const size_t& num_hard_drivers, const size_t& hard_driver_memory, const size_t& num_integrated_controllers, 
            const size_t& num_external_devices, const std::string& operating_system);
    ~RowTable() = default;

    void print() const;
};

#include "../src/RowTable.cpp"

#endif