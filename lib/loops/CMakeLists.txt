ADD_LIBRARY(loops INTERFACE)
TARGET_INCLUDE_DIRECTORIES(loops INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})




#TODO: remove when c++20 is realeased
TARGET_COMPILE_OPTIONS(loops INTERFACE 
    -fcoroutines
    )


TARGET_LINK_LIBRARIES(loops PRIVATE INTERFACE project_warnings project_options)
