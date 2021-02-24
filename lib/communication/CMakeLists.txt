ADD_LIBRARY(communication INTERFACE)
TARGET_INCLUDE_DIRECTORIES(communication INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})



find_package(HPX REQUIRED)


target_include_directories(communication PUBLIC INTERFACE ${HPX_INC_DIR})
target_link_libraries(communication PUBLIC INTERFACE ${HPX_LIB})


TARGET_LINK_LIBRARIES(communication PRIVATE INTERFACE project_warnings project_options loops)