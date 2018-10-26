cmake_minimum_required(VERSION 3.8)
project(veeam_signature)

# Application dependencies:
#  - Boost: required
#  - Google Test: optional, required for building tests
#  - OpenSSL: optional, required for enabling additional hash algorithms (MD5, SHA-1)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall")

# external dependencies
find_package(Boost COMPONENTS program_options iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest)

find_package(OpenSSL)

find_package(Threads REQUIRED)

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
        src/thread_executor.cpp
        )
target_link_libraries(signature
        ${Boost_LIBRARIES}
        ${hasher_LIBRARY}
        Threads::Threads)

# tests
if (GTest_FOUND)
    add_executable(signature_test
            test/program_config_test.cpp
            src/program_config.cpp)
    target_link_libraries(signature_test
            ${Boost_LIBRARIES}
            ${hasher_LIBRARY}
            GTest::GTest
            GTest::Main)
    gtest_discover_tests(signature_test)
else ()
    message(WARNING "Google Test not found. Omitting building tests.")
endif ()


