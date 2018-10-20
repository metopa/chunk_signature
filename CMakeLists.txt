cmake_minimum_required(VERSION 3.12)
project(veeam)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest REQUIRED)

add_executable(veeam_calculate_checksum
        src/main.cpp
        src/program_config.cpp
        src/base_hasher.cpp
        src/md5_hasher.cpp
        src/crc32_hasher.cpp)
target_link_libraries(veeam_calculate_checksum ${Boost_LIBRARIES})

add_executable(veeam_calculate_checksum_tests
        test/program_config_test.cpp
        src/program_config.cpp)
target_link_libraries(veeam_calculate_checksum_tests
        ${Boost_LIBRARIES}
        GTest::GTest
        GTest::Main)
gtest_discover_tests(veeam_calculate_checksum_tests)



