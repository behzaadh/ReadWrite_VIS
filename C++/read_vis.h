#ifndef READ_VIS_H
#define READ_VIS_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
// More meaningful names for data type by aliasing existing data
// typedef was replaced with using to make the code more readable
// and consistent with the rest of the C++ language
// shorturl.at/qGT16
using BYTE = unsigned char;
using Int1D = std::vector<int>;
using Double1D = std::vector<double>;
using Double2D = std::vector<std::vector<double>>;
using String2D = std::vector<std::vector<std::string>>;

struct SLB_DATA{
    std::string Data_type;
    std::string Variable_class;
    std::string Variable_type;
    Double2D    DATA;     // Double data
    std::string HEADER;   // String data
};

/**
 * @class READ_VIS
 * @brief A class for reading VISAGE restart files and extracting data.
 *
 * The READ_VIS class provides methods to read VISAGE restart files and extract various types of data from them.
 * It supports reading values, names, and vectors from the VISAGE restart file.
 */
class READ_VIS {
public:
    READ_VIS(const std::string &filename);
    Double2D value(const std::string &key);
    std::vector<double> value(const std::string &key, int col);
    std::string name(const std::string &key);
    void printVector(const std::string &key);
    void printName(const std::string &key);
    std::unordered_map<std::string, SLB_DATA> Data;

    // For c interface
    static void CreateInstance(const std::string &filename);
    static std::shared_ptr<READ_VIS> getInstance();

private:
    std::vector<BYTE> byteArray(const char* filename);

    std::string keyword_b();

    std::string uChar2Str_b();

    // Big-endian ordering to integer
    int uChar2Int_b();

    // Big-endian ordering to double
    double uChar2Doub_b();

    // Big-endian ordering to integer
    float uChar2Real_b();

    // skip next word
    void skip();

    std::vector<BYTE> _byteVector;
    std::vector<BYTE> _temp4;
    std::vector<BYTE> _temp8;
    static std::shared_ptr<READ_VIS> _instance;
    int _i;
};

#endif // READ_VIS_H
