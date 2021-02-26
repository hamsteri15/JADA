
# Build catch main
add_library(catch_main STATIC catch_main.cpp)



SET(SerialTestSources
    test_utils.cpp;
    test_loops.cpp;
    test_grid.cpp;
    test_containers.cpp;
    #test_communication.cpp;
    #test_stencil.cpp;
    #test_operation.cpp;u
)




add_executable(SerialTest.bin ${SerialTestSources})
target_link_libraries(SerialTest.bin PUBLIC project_options catch_main loops stencil operation communication)
target_compile_options(SerialTest.bin PRIVATE -DDEBUG)
add_test( NAME SerialTest COMMAND SerialTest.bin)

SET(ParallelTestSources
    test_communication.cpp;
)

# Build catch hpx main
add_library(catch_hpx_main STATIC catch_hpx_main.cpp)

target_link_libraries(catch_hpx_main PUBLIC HPX::hpx)

add_executable(ParallelTest.bin ${ParallelTestSources})
target_link_libraries(ParallelTest.bin PUBLIC catch_hpx_main communication)
target_compile_options(ParallelTest.bin PRIVATE -DDEBUG)

add_test(NAME ParallelTest COMMAND ParallelTest.bin --hpx:localities=2)