cmake_minimum_required(VERSION 3.5)

project(Read_VIS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Read_VIS test.cpp read_vis.h read_vis.cpp)

include(GNUInstallDirs)
install(TARGETS Read_VIS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
