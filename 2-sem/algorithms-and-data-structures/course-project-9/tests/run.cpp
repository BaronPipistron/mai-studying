#include <iostream>
#include <string>

#include "../include/Table.h"
#include "../src/benchmark.cpp"

int main(){
    Table<std::string> t;

    t.push_back("zzzz0012", "fdrkr");
    t.push_back("ttttt45g", "glpdsg");
    t.push_back("qqqqrer4", "gldg");
    t.push_back("bgag3431", "gfggf");
    t.push_back("aaaare45", "fdlafdl");

    t.sort("zzzzzzzzzzzzzzzzzzzzzz");
    t.print();

    benchmark();
}