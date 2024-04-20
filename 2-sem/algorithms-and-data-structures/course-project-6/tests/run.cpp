#include <iostream>
#include <iomanip>
#include <string>

#include "../include/DataBase.h"

int main(int argc, char* argv[]){
    DataBase db("MyBDFile");

    RowTable comp1("Alekseev", 2, "Pentium", 1400, "PCI", 16020, "SCSI", 1, 14751, 5, 4, "Ubuntu");
    RowTable comp2("Garibyan", 1, "AMD", 600, "AGP", 4096, "IDE", 2, 14789, 2, 1, "Windows");
    RowTable comp3("Nefedov", 3, "Baikal", 256, "Embedded", 14855, "ATA", 2, 18662, 3, 3, "Arch");
    RowTable comp4("Smerchinskaya", 4, "Intel", 1024, "PCI", 4665, "SATA", 2, 25641, 1, 6, "Windows");
    RowTable comp5("Bitukov", 1, "AMD", 2048, "PCI", 7888, "IDE", 3, 26465, 6, 7, "MacOS");
    RowTable comp6("Bartakovski", 2, "Intel", 1240, "Embedded", 14096, "SATA", 3, 14751, 3, 5, "Windows");
    RowTable comp7("Martushova", 1, "Intel", 1349, "Embedded", 13096, "IDE", 1, 18462, 5, 3, "Ubuntu");
    RowTable comp8("Pegachkova", 1, "AMD", 1491, "External", 2048, "SCSI", 1, 85616, 2, 9, "MacOS");
    RowTable comp9("Zaitsev", 1, "Intel", 41941, "PCI", 7895, "ATA", 2, 15445, 4, 7, "Ubuntu");
    RowTable comp10("Burdin", 4, "AMD", 14141, "AGP", 4096, "SCSI", 2, 26462, 5, 3, "Arch");
    RowTable comp11("Bobkov", 3, "AMD", 41414, "Embedded", 7889, "SCSI", 2, 15452, 1, 1, "MacOS");
    RowTable comp12("Krylov", 1, "AMD", 1656, "PCI", 1532, "IDE", 1, 78232, 2, 0, "Ubuntu");
    RowTable comp13("Bulakina", 1, "Intel", 784, "External", 1514, "ATA", 2, 7812, 4, 5, "Arch");
    RowTable comp14("Kondaratsev", 2, "Intel", 4151, "AGP", 7851, "SCSI", 1, 13228, 4, 4, "Windows");
    RowTable comp15("Soshnikov", 2, "AMD", 17885, "External", 3214, "SATA", 2, 79512, 2, 6, "Arch");

    db.addData(comp1);
    db.addData(comp2);
    db.addData(comp3);
    db.addData(comp4);
    db.addData(comp5);
    db.addData(comp6);
    db.addData(comp7);
    db.addData(comp8);
    db.addData(comp9);
    db.addData(comp10);
    db.addData(comp11);
    db.addData(comp12);
    db.addData(comp13);
    db.addData(comp14);
    db.addData(comp15);

    int p_val = 0;

    for (size_t i = 1; i < argc; ++i){
        if (argv[i] == "-f"){
            db.print();
        } else if (argv[i] == "-p" && (i + 1) < argc){
            p_val = std::atoi(argv[i + 1]);
        }
    }

    db.findOwners(p_val);

    return 0;
}