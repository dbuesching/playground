cmake_minimum_required(VERSION 3.10)

# set the project name
project(file_transfer_client)

find_package (Threads)


# add the executable
add_executable(file_transfer_client file_transfer_client.cpp)


target_include_directories(file_transfer_client PUBLIC /usr/local/include)
target_link_libraries(file_transfer_client ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
