#include <iostream>
#include <string>
#include "read_vis.h"
#include "write_vis.h"

int main(int argc, char* argv[]) {
    std::string filename = "../../../Examples/2BLOCKS2WAY2022_IX.VIS_T0000";
    try {
        auto result = READ_VIS(filename);
        result.printVector("SATURATION");
        result.printVector("PORE");
        result.printName("CaseName");
        for (int i = 0; i < result.Data.at("SATURATION").DATA.front().size(); ++i) {
            result.Data.at("SATURATION").DATA.front().at(i) = 0.5;
        }
        result.Data.at("CaseName").HEADER = "TestCaseName       ";
        auto input = WRITE_VIS(filename);
        input.write("SATURATION", result.Data.at("SATURATION"));
        input.write("CaseName", result.Data.at("CaseName"));
        result = READ_VIS(filename);
        result.printVector("SATURATION");
        result.printName("CaseName");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
