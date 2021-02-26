########################################################################
#
#  Queries Hpx from system libs using cmake find_package
#  If succesful a target can be linked to boost by
#  target_include_directories(target PRIVATE ${HPX_INC_DIR})
#  target_link_libraries(target PRIVATE ${HPX_LIB})
#       
#
########################################################################

find_package(HPX REQUIRED)


SET(HPX_INC_DIR ${HPX_INCLUDE_DIRS})
SET(HPX_LIBRARY_DIR ${HPX_LIBRARY_DIRS}) 
SET(HPX_LIB ${HPX_LIBRARIES})


message("HPX_INC_DIR ${HPX_INC_DIR}")
message("HPX_LIBRARY_DIR ${HPX_LIBRARY_DIR}")
message("HPX_LIB ${HPX_LIB}")
#${Boost_LIBRARY_DIR}
