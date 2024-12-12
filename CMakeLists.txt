cmake_minimum_required(VERSION 3.30)

project(5oxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR src)

find_package(Boost 1.86.0 REQUIRED COMPONENTS log log_setup system filesystem)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost libraries not found")
endif ()

add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::log Boost::log_setup Boost::system Boost::filesystem)
