cmake_minimum_required(VERSION 3.10)

# set the project name
project(file_transfer_server)

find_package (Threads)


# add the executable
add_executable(file_transfer_server file_transfer_server.cpp)


target_include_directories(file_transfer_server PUBLIC /usr/local/include)
target_link_libraries(file_transfer_server ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
