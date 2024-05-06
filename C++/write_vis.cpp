#include "write_vis.h"
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

std::shared_ptr<WRITE_VIS> WRITE_VIS::_instance;

/**
 * @brief Constructs a new instance of the WRITE_VIS class.
 * 
 * @param filename The filename to write to.
 */
WRITE_VIS::WRITE_VIS(const std::string &filename)
{
    _filename = filename;
    std::string T_SPEC_filename = filename;
    size_t pos = T_SPEC_filename.find(".VIS_T");
    if (pos != std::string::npos) {
        T_SPEC_filename.replace(pos, 6, ".VIS_TSPEC_");
    }

    // Open the file for writing
    std::ifstream file(T_SPEC_filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error while opening VIS_TSPEC file");
    }

    // Define the format of the data in the file
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        double value;
        if (!(iss >> name >> value)) {
            throw std::runtime_error("Error reading data from file");
        }
        _names.push_back(name);
        _values.push_back(value);
    }

    file.close();
}

/**
 * Writes the values associated with the given key.
 *
 * @param key The key to look up the value for.
 * @return The value associated with the given key.
 */
void WRITE_VIS::write(const std::string &key, const SLB_DATA &data)
{
    bool isUpdatedKeyworkAvailable = false;
    for (const auto& name : _names) {
        if (name == key) {
            isUpdatedKeyworkAvailable = true;
            break;
        }
    }

    if (!isUpdatedKeyworkAvailable) {
        throw std::runtime_error("The updated keyword does not exist in T files!");
    }

    // Open file
    std::fstream vis_file(_filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!vis_file.is_open()) {
        throw std::runtime_error("Error while opening VIS_T file");
    }

    int wsize = 0;
    // Read data based on dtype
    if (data.Data_type == "Integer") {
        wsize = 4;
    } else if (data.Data_type == "Real") {
        wsize = 4;
    } else if (data.Data_type == "Double") {
        wsize = 8;
    } else if (data.Data_type == "Logic") {
        wsize = 4;
    } else if (data.Data_type == "String") {
        wsize = 1;
    }
    std::vector<BYTE> tempByte;
    if (data.Data_type != "String") {
        tempByte.resize(data.DATA.size() * data.DATA.front().size() * wsize);
    } else {
        tempByte.resize(data.HEADER.size() * wsize);
    }

    // Write data
    for (size_t i = 0; i < _names.size(); ++i) {
        // Skip if the keyword is not the updated one
        if (_names[i] != key) {
            continue;
        }

        // Write data based on its type
        if (data.Data_type != "String") {
            size_t idx = 0;
            for (size_t i = 0; i < data.DATA.size(); ++i) {
                for (size_t j = 0; j < data.DATA[i].size(); ++j) {
                    unsigned char* temp = nullptr;
                    if (data.Data_type == "Integer") {
                        int val = data.DATA[i][j];
                        temp = reinterpret_cast<unsigned char*>(&val);
                    } else if (data.Data_type == "Real") {
                        float val = data.DATA[i][j];
                        temp = reinterpret_cast<unsigned char*>(&val);
                    } else if (data.Data_type == "Double") {
                        double val = data.DATA[i][j];
                        temp = reinterpret_cast<unsigned char*>(&val);
                    } else if (data.Data_type == "Logic") {
                        bool val = data.DATA[i][j];
                        temp = reinterpret_cast<unsigned char*>(&val);
                    }
                    // Convert to big endian
                    std::reverse(temp, temp + wsize);
                    // Copy to tempByte
                    std::copy(temp, temp + wsize, tempByte.data() + idx * wsize);
                    idx++;
                }
            }
        } else {
            size_t idx = 0;
            for (size_t i = 0; i < data.HEADER.size(); ++i) {
                char val = data.HEADER[i];
                unsigned char* temp = reinterpret_cast<unsigned char*>(&val);
                // Convert to big endian
                std::reverse(temp, temp + wsize);
                // Copy to tempByte
                std::copy(temp, temp + wsize, tempByte.data() + idx * wsize);
                idx++;
            }
        }


        if (tempByte.empty()) {
            throw std::runtime_error("No data provided to write");
        }

        // Move file pointer to the beginning of the data
        vis_file.seekp(_values[i] - 1 + 50 + 33 + 4, std::ios_base::beg);
        vis_file.write(reinterpret_cast<char*>(tempByte.data()), tempByte.size());
        break;
    }

    vis_file.close();
}

/**
 * @brief Creates an instance of the WRITE_VIS class.
 * 
 * This function creates an instance of the WRITE_VIS class and initializes it with the specified filename.
 * 
 * @param filename The name of the file to be read.
 */
void WRITE_VIS::CreateInstance(const std::string &filename)
{
    if (!WRITE_VIS::_instance) {
        WRITE_VIS::_instance = std::make_shared<WRITE_VIS>(filename);
    }
}

/**
 * @brief Returns a shared pointer to the singleton instance of the WRITE_VIS class.
 *
 * This function is used to obtain a shared pointer to the singleton instance of the WRITE_VIS class.
 * The singleton pattern ensures that only one instance of the class is created throughout the program's lifetime.
 *
 * @return A shared pointer to the singleton instance of the WRITE_VIS class.
 */
std::shared_ptr<WRITE_VIS> WRITE_VIS::getInstance()
{
    return WRITE_VIS::_instance;
}
