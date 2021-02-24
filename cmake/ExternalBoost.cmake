########################################################################
#
#  Queries Boost from system libs using cmake find_package
#  If succesful a target can be linked to boost by
#  target_include_directories(target PRIVATE ${BOOST_INC_DIR})
#  target_link_libraries(target PRIVATE ${BOOST_LIB})
#       
#
########################################################################

find_package(Boost COMPONENTS program_options REQUIRED)


message("STATUS boost FOUND: ${Boost_FOUND}")

SET(BOOST_INC_DIR ${Boost_INCLUDE_DIR})
set(BOOST_LIBRARY_DIR ${Boost_LIBRARY_DIR_RELEASE})
SET(BOOST_LIB ${Boost_LIBRARIES})


message("BOOST INC ${BOOST_INC_DIR}")
message("BOOST LIBRARY_DIR ${BOOST_LIBRARY_DIR}")
message("BOOST LIB ${BOOST_LIB}")
#${Boost_LIBRARY_DIR}
