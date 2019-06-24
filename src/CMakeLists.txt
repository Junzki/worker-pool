cmake_minimum_required(VERSION 3.8)


find_package(Threads)


add_executable(worker-pool main.c)
target_link_libraries(worker-pool ${CMAKE_THREAD_LIBS_INIT})
