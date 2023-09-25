cmake_minimum_required(VERSION 3.0.0)
project(BladeAlleyIrr VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

include_directories(
#    irrlicht-1.8.5/include
    irrKlang/include
)

add_executable(BladeAlleyIrr src/main.cpp)

target_link_directories(BladeAlleyIrr PRIVATE irrKlang/bin/linux-gcc-64)
target_link_libraries(BladeAlleyIrr PRIVATE -lIrrlicht -lIrrKlang)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
