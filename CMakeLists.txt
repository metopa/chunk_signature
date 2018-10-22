cmake_minimum_required(VERSION 3.12)
project(veeam_signature)

set(CMAKE_CXX_STANDARD 14)

# External dependencies
find_package(Boost COMPONENTS program_options iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest REQUIRED)

find_package(OpenSSL)

include_directories(include)

set(HASHER_SRC
        lib/hasher/base_hasher.cpp
        lib/hasher/crc32_hasher.cpp
        )

if (OpenSSL_FOUND)
    message(STATUS "OpenSSL found. Additional hashers are available.")
    set(HASHER_SRC
            ${HASHER_SRC}
            lib/hasher/md5_hasher.cpp
            )
else (OpenSSL_FOUND)
    message(WARNING "OpenSSL not found. Additional hashers are not available.")
endif (OpenSSL_FOUND)

add_library(hasher ${HASHER_SRC})
SET(hasher_LIBRARY -Wl,--whole-archive hasher -Wl,--no-whole-archive)
if (OpenSSL_FOUND)
    target_link_libraries(hasher OpenSSL::SSL)
endif (OpenSSL_FOUND)

add_executable(signature
        src/main.cpp
        src/program_config.cpp
        src/result_writer/base_result_writer.cpp
        src/result_writer/m_mapped_result_writer.cpp
        src/file_chunker.cpp
        )
target_link_libraries(signature ${Boost_LIBRARIES} ${hasher_LIBRARY})

add_executable(signature_test
        test/program_config_test.cpp
        src/program_config.cpp)
target_link_libraries(signature_test
        ${Boost_LIBRARIES}
        GTest::GTest
        GTest::Main)
gtest_discover_tests(signature_test)



