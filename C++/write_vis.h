#ifndef WRITE_VIS_H
#define WRITE_VIS_H

#include "read_vis.h"

/**
 * @class WRITE_VIS
 * @brief A class for writing VISAGE restart files.
 *
 * The WRITE_VIS class provides methods to write VISAGE restart files.
 * It supports writing values, names, and vectors to the VISAGE restart file.
 */
class WRITE_VIS {
public:
    WRITE_VIS(const std::string &filename);
    void write(const std::string &key, const SLB_DATA &data);

    // For c interface
    static void CreateInstance(const std::string &filename);
    static std::shared_ptr<WRITE_VIS> getInstance();

private:
    static std::shared_ptr<WRITE_VIS> _instance;
    std::vector<std::string> _names;
    std::vector<double> _values;
    std::string _filename;
};

#endif // WRITE_VIS_H
