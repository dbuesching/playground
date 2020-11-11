cmake_minimum_required(VERSION 3.10)

# set the project name
project(schaeling_examples)

find_package (Threads)


# add the executable
add_executable(schaeling_examples schaeling_examples.cpp)


target_include_directories(schaeling_examples PUBLIC /usr/local/include)
target_link_libraries(schaeling_examples ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
