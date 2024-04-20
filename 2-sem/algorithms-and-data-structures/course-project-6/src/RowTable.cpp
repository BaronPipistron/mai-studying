#include "../include/RowTable.h"

#include <iostream>
#include <iomanip>

RowTable::RowTable(const std::string& owner_surname, const size_t& num_processors, const std::string& processor_type, 
                const size_t& memory_sz, const std::string& video_controller_type, const size_t& video_memory_sz, 
                const std::string& hard_driver_type, const size_t& num_hard_drivers, const size_t& hard_driver_memory, 
                const size_t& num_integrated_controllers, const size_t& num_external_devices, const std::string& operating_system):
                owner_surname(owner_surname), num_processors(num_processors), processor_type(processor_type), memory_sz(memory_sz),
                video_controller_type(video_controller_type), video_memory_sz(video_memory_sz), hard_driver_type(hard_driver_type),
                num_hard_drivers(num_hard_drivers), hard_driver_memory(hard_driver_memory), 
                num_integrated_controllers(num_integrated_controllers), num_external_devices(num_external_devices), 
                operating_system(operating_system) {}

void RowTable::print() const{
    std::cout << "| " << std::setw(14) << std::left << owner_surname << "| " << std::setw(18) << std::left << num_processors <<
                "| " << std::setw(16) << std::left << processor_type << "| " << std::setw(12) << std::left << memory_sz <<
                "| " << std::setw(22) << std::left << video_controller_type << "| " << std::setw(13) << std::left << video_memory_sz <<
                "| " << std::setw(17) << std::left << hard_driver_type << "| " << std::setw(20) << std::left << num_hard_drivers <<
                "| " << std::setw(19) << std::left << hard_driver_memory << "| " << std::setw(30) << std::left <<
                num_integrated_controllers << "| " << std::setw(24) << std::left << num_external_devices << "| " <<
                std::setw(17) << std::left << operating_system << "|\n";
}