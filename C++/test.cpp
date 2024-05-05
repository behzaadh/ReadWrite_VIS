#include <iostream>
#include <string>
#include "read_vis.h"

int main(int argc, char* argv[]) {
    std::string filename = "../../../Examples/2BLOCKS2WAY2022_IX.VIS_T0000";
    try {
        auto result = READ_VIS(filename);
        result.printVector("SATURATION");
        result.printVector("PORE");
        result.printName("CaseName");
        // Use the result as needed
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
