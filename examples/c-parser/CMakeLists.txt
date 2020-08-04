add_executable(c-parser
    include/c-parser/root.h
    include/c-parser/direction.h

    src/root.cpp
    src/direction.cpp

    main.cpp include/c-parser/variable.h include/c-parser/function.h src/variable.cpp src/function.cpp include/c-parser/type.h src/type.cpp include/c-parser/expression.h src/expression.cpp)

target_include_directories(c-parser PUBLIC include)
target_link_libraries(c-parser PUBLIC ares fmt)
