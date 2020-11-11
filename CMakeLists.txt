cmake_minimum_required(VERSION 3.10)
# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# expected behaviour
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++lastest")

set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost)


# set the project name
project(boost_asio_samples)

add_subdirectory(./schaeling_examples)
add_subdirectory(./file_transfer_client)
add_subdirectory(./file_transfer_server)

