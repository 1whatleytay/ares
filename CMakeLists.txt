cmake_minimum_required(VERSION 3.17)
project(ares)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/fmt)

add_library(ares
    include/ares/rule.h
    include/ares/text.h
    include/ares/instructions.h
    include/ares/builder.h
    include/ares/context.h
    include/ares/callbacks.h

    src/rule.cpp
    src/text.cpp
    src/instructions.cpp
    src/builder.cpp)
target_include_directories(ares PUBLIC include)
target_link_libraries(ares PUBLIC fmt)

add_subdirectory(examples)
