cmake_minimum_required(VERSION 3.8)
project(veeam_signature)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS_RELEASE -g)
# external dependencies
find_package(Boost COMPONENTS program_options iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest REQUIRED)

find_package(OpenSSL)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else ()
    message(WARNING "OpenMP not found. Compiling without threading support.")
endif ()

# hasher library
include_directories(include)

set(HASHER_SRC
        lib/hasher/base_hasher.cpp
        lib/hasher/crc32_hasher.cpp
        )

if (OpenSSL_FOUND)
    message(STATUS "OpenSSL ${OPENSSL_VERSION} found. Additional hashers are available.")
    set(HASHER_SRC
            ${HASHER_SRC}
            lib/hasher/md5_hasher.cpp
            lib/hasher/sha1_hasher.cpp
            )
else ()
    message(WARNING "OpenSSL not found. Additional hashers are not available.")
endif ()

add_library(hasher ${HASHER_SRC})
if (OpenSSL_FOUND)
    target_link_libraries(hasher OpenSSL::SSL)
endif ()

# enable static factory registration
# without this hack register functions wouldn't be included in an executable
if (MSVC)
    SET(hasher_LIBRARY hasher /WHOLEARCHIVE:hasher)
else ()
    SET(hasher_LIBRARY -Wl,--whole-archive hasher -Wl,--no-whole-archive)
endif ()


# main executable
add_executable(signature
        src/main.cpp
        src/program_config.cpp
        src/result_writer/base_result_writer.cpp
        src/result_writer/m_mapped_result_writer.cpp
        src/file_chunker.cpp
        )
target_link_libraries(signature ${Boost_LIBRARIES} ${hasher_LIBRARY})

# tests
add_executable(signature_test
        test/program_config_test.cpp
        src/program_config.cpp)
target_link_libraries(signature_test
        ${Boost_LIBRARIES}
        GTest::GTest
        GTest::Main)
gtest_discover_tests(signature_test)



