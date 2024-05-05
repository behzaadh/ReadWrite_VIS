#include "read_vis.h"
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

std::shared_ptr<READ_VIS> READ_VIS::_instance;

/**
 * @brief Constructs a new instance of the READ_VIS class.
 * 
 * @param filename The filename to read from.
 */
READ_VIS::READ_VIS(const std::string &filename) : _temp4(4), _temp8(8), _i(0)
{
    std::string T_SPEC_filename = filename;
    size_t pos = T_SPEC_filename.find(".VIS_T");
    if (pos != std::string::npos) {
        T_SPEC_filename.replace(pos, 6, ".VIS_TSPEC_");
    }

    // Open the file for reading
    std::ifstream file(T_SPEC_filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error while opening VIS_TSPEC file");
    }

    // Define the format of the data in the file
    std::string line;
    std::vector<std::string> names;
    std::vector<double> values;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        double value;
        if (!(iss >> name >> value)) {
            throw std::runtime_error("Error reading data from file");
        }
        names.push_back(name);
        values.push_back(value);
    }

    file.close();

    _byteVector = byteArray(filename.c_str());

    for (size_t i = 0; i < names.size(); ++i) {
        int     wsize = 0;
        size_t  cellSize = 0;
        _i = values[i] - 1;
        std::string keyword(keyword_b());
        std::string dtype(uChar2Str_b());
        std::string variable(uChar2Str_b());
        std::string VariableType(uChar2Str_b());
        skip();

        if (keyword == "END") {
            break;
        } else {
            cellSize = values[i + 1] - values[i] - 87;
            Data[keyword].Data_type = dtype;
            Data[keyword].Variable_class = variable;
            Data[keyword].Variable_type = VariableType;
        }

        std::vector<double>      Vec_data;     // Double data

        // Read data based on dtype
        if (dtype == "Integer") {
            while (_i < (values[i + 1]-1)) {
                Vec_data.push_back((double)uChar2Int_b());
            }
        } else if (dtype == "Real") {
            while (_i < (values[i + 1]-1)) {
                Vec_data.push_back((double)uChar2Real_b());
            }
        } else if (dtype == "Double") {
            while (_i < (values[i + 1]-1)) {
                Vec_data.push_back(uChar2Doub_b());
            }
        } else if (dtype == "Logic") {
            while (_i < (values[i + 1]-1)) {
                Vec_data.push_back((double)uChar2Int_b());
            }

        } else if (dtype == "String") {
            _i += cellSize;
            Data[keyword].HEADER = std::string(reinterpret_cast<const char *>(&_byteVector.data()[_i-cellSize]), cellSize);
        }

        if (dtype != "String") {
            if(VariableType == "GaussPoint") {
                size_t rows = Vec_data.size() / 8;
                for (size_t r = 0; r < 8; ++r) {
                    std::vector<double> reshaped_data;
                    for (size_t c = 0; c < rows; ++c) {
                        reshaped_data.push_back(Vec_data[r * rows + c]);
                    }
                    Data[keyword].DATA.push_back(reshaped_data);
                }
            } else {
                Data[keyword].DATA.push_back(Vec_data);
            }
        }
    }
}

/**
 * Retrieves the value associated with the given key.
 *
 * @param key The key to look up the value for.
 * @return The value associated with the given key.
 */
Double2D READ_VIS::value(const std::string &key)
{
    // Check if key exists
    if (Data.find(key) == Data.end()) {
        std::cout << "Could not find " << key << std::endl;
        return Double2D();
    }

    return Data.at(key).DATA;
}

/**
 * Retrieves the value from the specified column for the given key.
 *
 * @param key The key to search for.
 * @param col The column index to retrieve the value from.
 * @return A vector of double values from the specified column for the given key.
 */
std::vector<double> READ_VIS::value(const std::string &key, int col)
{
    // Check if key exists
    if (Data.find(key) == Data.end()) {
        std::cout << "Could not find " << key << std::endl;
        return std::vector<double>();
    }

    return Data.at(key).DATA[col];
}

/**
 * Retrieves the name associated with the given key.
 *
 * @param key The key for which to retrieve the name.
 * @return The name associated with the key.
 */
std::string READ_VIS::name(const std::string &key)
{
    // Check if key exists
    if (Data.find(key) == Data.end()) {
        std::cout << "Could not find " << key << std::endl;
        return std::string();
    }

    return Data.at(key).HEADER;
}

/**
 * Prints the elements of a vector associated with the given key.
 *
 * @param key The key associated with the vector.
 */
void READ_VIS::printVector(const std::string &key)
{
    // Check if key exists
    if (Data.find(key) == Data.end()) {
        std::cout << "Could not find " << key << std::endl;
        return;
    }
    std::cout << key << ": " << std::endl;

    Double2D vect = Data.at(key).DATA;

    for (int i = 0; i < vect.size(); i++) {
        for (int j = 0; j < vect[i].size(); j++) {
            std::cout << vect[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


/**
 * Prints the elements of a string associated with the given key.
 *
 * @param key The key associated with the string.
 */
void READ_VIS::printName(const std::string &key)
{
    // Check if key exists
    if (Data.find(key) == Data.end()) {
        std::cout << "Could not find " << key << std::endl;
        return;
    }

    std::cout << key << ": " << Data.at(key).HEADER << " " << std::endl;
}

/**
 * @brief Creates an instance of the READ_VIS class.
 * 
 * This function creates an instance of the READ_VIS class and initializes it with the specified filename.
 * 
 * @param filename The name of the file to be read.
 */
void READ_VIS::CreateInstance(const std::string &filename)
{
    if (!READ_VIS::_instance) {
        READ_VIS::_instance = std::make_shared<READ_VIS>(filename);
    }
}

/**
 * @brief Returns a shared pointer to the singleton instance of the READ_VIS class.
 *
 * This function is used to obtain a shared pointer to the singleton instance of the READ_VIS class.
 * The singleton pattern ensures that only one instance of the class is created throughout the program's lifetime.
 *
 * @return A shared pointer to the singleton instance of the READ_VIS class.
 */
std::shared_ptr<READ_VIS> READ_VIS::getInstance()
{
    return READ_VIS::_instance;
}


/**
 * @brief Reads the contents of a file and returns them as a vector of bytes. 
 * adopted from https://stackoverflow.com/a/21802936/4027652
 * 
 * @param filename The name of the file to read.
 * @return std::vector<BYTE> The contents of the file as a vector of bytes.
 */
std::vector<BYTE> READ_VIS::byteArray(const char *filename)
{
    std::vector<BYTE> vec;
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "failed to open " << filename << '\n';
    } else {

        // Stop eating new lines in binary mode!!!
        file.unsetf(std::ios::skipws);

        // get its size:
        std::streampos fileSize;

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // reserve capacity
        vec.reserve(fileSize);

        // read the data:
        vec.insert(vec.begin(),
                   std::istream_iterator<BYTE>(file),
                   std::istream_iterator<BYTE>());
    }
    return vec;
}

/**
 * Converts an unsigned char to a string representation.
 *
 * @return The string representation of the unsigned char.
 */
std::string READ_VIS::keyword_b()
{
    _i += 50;
    std::string keyword(reinterpret_cast<const char *>(&_byteVector.data()[_i-50]), 50);
    keyword.erase(std::remove(keyword.begin(), keyword.end(), ' '), keyword.end());
    std::replace(keyword.begin(), keyword.end(), '+', '_');
    std::replace(keyword.begin(), keyword.end(), '-', '_');

    return keyword;
}

/**
 * Converts an unsigned char to a string representation.
 *
 * @return The string representation of the unsigned char.
 */
std::string READ_VIS::uChar2Str_b()
{
    _i += 11;
    std::string string(reinterpret_cast<const char *>(&_byteVector.data()[_i-11]), 11);
    string.erase(std::remove(string.begin(), string.end(), ' '), string.end());
    std::replace(string.begin(), string.end(), '+', '_');
    std::replace(string.begin(), string.end(), '-', '_');

    return string;
}

// TODO: find a way to not construct a new vector
/**
 * Converts an unsigned char to an integer.
 *
 * @return The converted integer value.
 */
int READ_VIS::uChar2Int_b()
{
    _temp4[3] = _byteVector[_i];
    _temp4[2] = _byteVector[_i+1];
    _temp4[1] = _byteVector[_i+2];
    _temp4[0] = _byteVector[_i+3];
    _i += 4;

    return *reinterpret_cast<const int32_t*>(&_temp4[0]);
}

/**
 * Converts an unsigned char value to a double value.
 *
 * @return The converted double value.
 */
double READ_VIS::uChar2Doub_b()
{
    _temp8[7] = _byteVector[_i];
    _temp8[6] = _byteVector[_i+1];
    _temp8[5] = _byteVector[_i+2];
    _temp8[4] = _byteVector[_i+3];
    _temp8[3] = _byteVector[_i+4];
    _temp8[2] = _byteVector[_i+5];
    _temp8[1] = _byteVector[_i+6];
    _temp8[0] = _byteVector[_i+7];
    _i += 8;

    return *reinterpret_cast<const double*>(&_temp8[0]);
}

/**
 * Converts an unsigned char value to a float value.
 *
 * @return The converted float value.
 */
float READ_VIS::uChar2Real_b()
{
    _temp4[3] = _byteVector[_i];
    _temp4[2] = _byteVector[_i+1];
    _temp4[1] = _byteVector[_i+2];
    _temp4[0] = _byteVector[_i+3];
    _i += 4;

    return *reinterpret_cast<const float*>(&_temp4[0]);
}

void READ_VIS::skip()
{
    _i += 4;
}
