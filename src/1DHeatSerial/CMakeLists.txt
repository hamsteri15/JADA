
SET(Sources
    main.cpp;
)



add_executable(1DHeatSerial ${Sources})



TARGET_COMPILE_OPTIONS(1DHeatSerial PUBLIC 
    -Wall 
    -Wextra 
    -pedantic 
    -msse3 
    -mavx 
    -mfpmath=sse 
    -O3
    -ffast-math 
    -march=native 
    -ftree-vectorize 
    -DNDEBUG
    )

target_include_directories(1DHeatSerial PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(1DHeatSerial PRIVATE grid loops operation stencil)
#add_test( NAME Benchmark COMMAND Benchmark.bin)                                    
