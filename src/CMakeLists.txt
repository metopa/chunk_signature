cmake_minimum_required(VERSION 3.12)
project(veeam_app)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(veeam_calculate_checksum main.cpp)
target_link_libraries(veeam_calculate_checksum ${Boost_LIBRARIES})
