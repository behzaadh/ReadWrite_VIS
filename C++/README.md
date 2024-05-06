# C++ Code Documentation

This C++ code consists of classes and functions for reading and writing VISAGE restart files  (`.VIS_T`).

## `read_vis.h` and `read_vis.cpp`

### `READ_VIS` Class

The `READ_VIS` class provides methods for reading VISAGE restart files and extracting various types of data from them.

#### Constructor

- `READ_VIS(const std::string &filename)`: Constructs a new instance of the `READ_VIS` class with the specified filename.

#### Public Methods

- `Double2D value(const std::string &key)`: Retrieves the value associated with the given key.
- `std::vector<double> value(const std::string &key, int col)`: Retrieves the value from the specified column for the given key.
- `std::string name(const std::string &key)`: Retrieves the name associated with the given key.
- `void printVector(const std::string &key)`: Prints the elements of a vector associated with the given key.
- `void printName(const std::string &key)`: Prints the elements of a string associated with the given key.

#### Static Methods

- `static void CreateInstance(const std::string &filename)`: Creates an instance of the `READ_VIS` class.
- `static std::shared_ptr<READ_VIS> getInstance()`: Returns a shared pointer to the singleton instance of the `READ_VIS` class.

## `write_vis.h` and `write_vis.cpp`

### `WRITE_VIS` Class

The `WRITE_VIS` class provides methods for writing values to VISAGE restart files.

#### Constructor

- `WRITE_VIS(const std::string &filename)`: Constructs a new instance of the `WRITE_VIS` class with the specified filename.

#### Public Methods

- `void write(const std::string &key, const SLB_DATA &data)`: Writes the values associated with the given key.

#### Static Methods

- `static void CreateInstance(const std::string &filename)`: Creates an instance of the `WRITE_VIS` class.
- `static std::shared_ptr<WRITE_VIS> getInstance()`: Returns a shared pointer to the singleton instance of the `WRITE_VIS` class.

## `main.cpp`

Contains the `main` function where you can test the functionality of the `READ_VIS` and `WRITE_VIS` classes. It demonstrates reading data from a VISAGE restart file, modifying the data, writing it back to the file, and reading it again.


Feel free to explore and modify the code as needed for your projects or research. If you have any questions or suggestions, please feel free to reach out (bdhosseinzadeh@gmail.com)!