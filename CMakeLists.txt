cmake_minimum_required(VERSION 3.0.0)
project(REST VERSION 0.1.0)

set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)

find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/redisclient/src)

add_subdirectory(redisclient)

set(SOURCE_FILES main.cpp api.h api.cpp)

add_executable(REST ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} cpprestsdk::cpprest RedisClient ${Boost_LIBRARIES} -lboost_system)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
