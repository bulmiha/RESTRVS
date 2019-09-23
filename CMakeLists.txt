cmake_minimum_required(VERSION 3.0.0)
project(REST VERSION 0.1.0)

find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/FindBerkeleyDB")

find_package(BerkeleyDB REQUIRED)

include(CTest)
enable_testing()

set(SOURCE_FILES main.cpp api.h api.cpp)

add_executable(REST ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Oracle::BerkeleyDB cpprestsdk::cpprest ${Boost_LIBRARIES} -lboost_system)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
