cmake_minimum_required(VERSION 3.10.0)
project(BladeAlleyIrr VERSION 0.1.0 LANGUAGES C CXX)

include_directories(
    src/
    irrKlang/include
)

file(GLOB SOURCES "src/*.cpp")
add_executable(BladeAlleyIrr ${SOURCES})

# target_link_directories(BladeAlleyIrr PRIVATE irrKlang/bin/linux-gcc-64)
# target_link_libraries(BladeAlleyIrr PRIVATE -lIrrKlang)
target_link_libraries(BladeAlleyIrr PRIVATE -lIrrlicht)
