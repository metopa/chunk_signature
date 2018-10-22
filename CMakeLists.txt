cmake_minimum_required(VERSION 3.12)
project(veeam_signature)

set(CMAKE_CXX_STANDARD 14)


find_package(Boost COMPONENTS program_options iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest REQUIRED)

include_directories(include)

set(HASHER_SRC
        lib/hasher/base_hasher.cpp
        lib/hasher/md5_hasher.cpp
        lib/hasher/crc32_hasher.cpp
        )
add_library(hasher ${HASHER_SRC})

add_executable(signature
        src/main.cpp
        src/program_config.cpp
        src/result_writer/base_result_writer.cpp
        src/result_writer/m_mapped_result_writer.cpp
        src/file_chunker.cpp
        )
target_link_libraries(signature ${Boost_LIBRARIES} hasher)

add_executable(signature_test
        test/program_config_test.cpp
        src/program_config.cpp)
target_link_libraries(signature_test
        ${Boost_LIBRARIES}
        GTest::GTest
        GTest::Main)
gtest_discover_tests(signature_test)



