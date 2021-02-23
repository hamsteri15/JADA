

# Build catch main
add_library(catch_main STATIC catch_main.cpp)



SET(SerialTestSources
    test_utils.cpp;
    test_loops.cpp;
    test_grid.cpp;
    test_containers.cpp;
    test_communication.cpp;
    #test_stencil.cpp;
    #test_operation.cpp;
)

add_executable(SerialTest.bin ${SerialTestSources})
target_link_libraries(SerialTest.bin PUBLIC project_options catch_main loops stencil operation)
target_compile_options(SerialTest.bin PRIVATE -DDEBUG)
add_test( NAME SerialTest COMMAND SerialTest.bin)


