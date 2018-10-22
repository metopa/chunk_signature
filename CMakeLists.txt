cmake_minimum_required(VERSION 3.12)
project(veeam)

set(CMAKE_CXX_STANDARD 14)


find_package(Boost COMPONENTS program_options iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest REQUIRED)

add_executable(veeam_calculate_checksum
        src/main.cpp
        src/program_config.cpp
        src/hasher/base_hasher.cpp
        src/hasher/md5_hasher.cpp
        src/hasher/crc32_hasher.cpp
        src/result_writer/base_result_writer.cpp
        src/result_writer/m_mapped_result_writer.cpp
        src/file_chunker.cpp
        )
target_link_libraries(veeam_calculate_checksum ${Boost_LIBRARIES})

add_executable(veeam_calculate_checksum_tests
        test/program_config_test.cpp
        src/program_config.cpp)
target_link_libraries(veeam_calculate_checksum_tests
        ${Boost_LIBRARIES}
        GTest::GTest
        GTest::Main)
gtest_discover_tests(veeam_calculate_checksum_tests)



