# Read_VIS
Read binary restart files (T files) from SLB's geomechanics simulators Visage.
# MATLAB Code Documentation

This MATLAB code consists of functions for reading and writing to/from Visage restart files (`.VIS_T`). These functions are designed to manipulate data stored in these files.

## Functions

### `read_vis`

This function reads data from a `.VIS_T` file and extracts information based on predefined formats. It then organizes this data into a structured format.

#### Input Parameters:

- `filename`: The name of the `.VIS_T` file to be read.

#### Output:

- `out`: A structured array containing the extracted data.

### `write_vis`

This function updates specific keywords within a `.VIS_T` file with new data.

#### Input Parameters:

- `filename`: The name of the `.VIS_T` file to be updated.
- `updatedKeyword`: The keyword to be updated within the file.
- `dataIn`: The new data to be written into the file for the specified keyword.

## Usage Example

```matlab
clc;clear;
out = read_vis('..\Examples\2BLOCKS2WAY2022_IX.VIS_T0000');
out.SATURATION.Data(:,1) = 0.5;
write_vis('..\Examples\2BLOCKS2WAY2022_IX.VIS_T0000', 'SATURATION', out.SATURATION.Data(:,1));
out = read_vis('..\Examples\2BLOCKS2WAY2022_IX.VIS_T0000');
```

Feel free to explore and modify the code as needed for your projects or research. If you have any questions or suggestions, please feel free to reach out (bdhosseinzadeh@gmail.com)!