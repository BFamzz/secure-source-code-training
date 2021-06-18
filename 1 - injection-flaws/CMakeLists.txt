#find_package(Threads)
#find_package(ZLIB)
#find_package(OpenSSL)

#if(OPENSSL_FOUND)
#    include_directories(${OPENSSL_INCLUDE_DIR})
#endif()

#if (NOT CMAKE_BUILD_TYPE)
#    message(STATUS "No build type selected, default to Release")
#    set(CMAKE_BUILD_TYPE "Release")
#endif()
#
#if (MSVC)
#    set(Boost_USE_STATIC_LIBS "On")
#    find_package( Boost 1.70 COMPONENTS system thread regex REQUIRED )
#else()
#    find_package( Boost 1.70 COMPONENTS system thread REQUIRED )
#endif()

#include_directories(${Boost_INCLUDE_DIR})

cmake_minimum_required(VERSION 3.13)

project(injection-flaws)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS  system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(libpqxx REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(injection-flaws ${sources1} ${sources2} main.cpp)

target_link_libraries(injection-flaws pq ${Boost_LIBRARIES} Threads::Threads pqxx ${PostgreSQL_LIBRARIES} crypto pthread dl) 

target_include_directories(injection-flaws PRIVATE ${Boost_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS})