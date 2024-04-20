#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "RowTable.h"

#include <iostream>
#include <string>

class DataBase{
private:
    FILE* file = nullptr;
    std::string_view file_path;

public:
    DataBase() = default;
    DataBase(const std::string& file_name);
    ~DataBase();

    std::string createFile(const std::string& file_name = "DBFile");
    void deleteFile();
    void addData(const RowTable& computer);
    void print() const;
    void findOwners(const uint64_t& num_of_external_devices) const;
};

#include "../src/DataBase.cpp"

#endif