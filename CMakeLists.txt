cmake_minimum_required(VERSION 3.10)

project(ALGORITHM VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Python
find_package(Python3 3.12 COMPONENTS Interpreter Development NumPy REQUIRED)

# Include directories for Python and NumPy
include_directories(
    {Python3_INCLUDE_DIRS}
    includeS
)
add_executable(
    main
    main.cpp
    ./include/checker.hpp
    ./include/mergeFiles.hpp
    ./include/readWriteData.hpp
    ./include/matplotlibcpp.hpp
    ./include/chart.hpp
    ./include/statistic.hpp
    ./model/KMP.hpp
    ./model/KMP.cpp
    ./model/Trie.hpp
    ./model/Trie.cpp
    ./model/CompressedTrie.hpp
    ./model/CompressedTrie.cpp
)

# Link the Psapi library for Windows
if(WIN32)
    target_link_libraries(main PRIVATE psapi)
endif()

# Link Python libraries
target_link_libraries(main PRIVATE Python3::NumPy)
