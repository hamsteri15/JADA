add_library(communication INTERFACE)
target_include_directories(communication INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(HPX REQUIRED)
target_include_directories(communication PUBLIC INTERFACE ${HPX_INCLUDE_DIRS})
target_link_libraries(communication PUBLIC INTERFACE ${HPX_LIBRARIES})

#SET(HPX_INC_DIR ${HPX_INCLUDE_DIRS})
#SET(HPX_LIBRARY_DIR ${HPX_LIBRARY_DIRS}) 
#SET(HPX_LIB ${HPX_LIBRARIES})


#target_include_directories(communication PUBLIC INTERFACE ${HPX_INC_DIR})
#target_link_libraries(communication PUBLIC INTERFACE ${HPX_LIB})

TARGET_LINK_LIBRARIES(communication PRIVATE INTERFACE project_warnings project_options loops)