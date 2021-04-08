
# Build catch main
add_library(catch_main STATIC catch_main.cpp)



SET(SerialTestSources
    test_utils.cpp;
    test_loops.cpp;
    test_grid.cpp;
    test_containers.cpp;
    #test_communication.cpp;
    test_stencil.cpp;
    #test_operation.cpp;u
)




add_executable(SerialTest.bin ${SerialTestSources})
target_link_libraries(SerialTest.bin PUBLIC project_options catch_main loops stencil communication)
target_compile_options(SerialTest.bin PRIVATE -DDEBUG)
add_test( NAME SerialTest COMMAND SerialTest.bin)

SET(ParallelTestSources
    test_communication.cpp;
)

add_hpx_executable(ParallelTest.bin
    SOURCES catch_hpx_main.cpp ${ParallelTestSources}
    COMPONENT_DEPENDENCIES iostreams partitioned_vector #hpx deps
    DEPENDENCIES grid loops communication project_warnings project_options                           #other deps
    #COMPILE_FLAGS       -Wno-unused-variable
    COMPILE_FLAGS   -DDEBUG
)

#For some reason the below approach doesnt work
###################################################
# Build catch hpx main
#add_library(catch_hpx_main STATIC catch_hpx_main.cpp)
#target_link_libraries(catch_hpx_main PUBLIC HPX::hpx)
#add_executable(ParallelTest.bin ${ParallelTestSources})
#target_link_libraries(ParallelTest.bin PUBLIC catch_hpx_main communication)
#target_compile_options(ParallelTest.bin PRIVATE -DDEBUG)

###################################################

add_test(NAME ParallelTest COMMAND ParallelTest.bin)

