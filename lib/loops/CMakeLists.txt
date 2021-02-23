ADD_LIBRARY(loops INTERFACE)
TARGET_INCLUDE_DIRECTORIES(loops INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


#SET(BOOST_MIN_VERSION "1.66")
find_package(Boost REQUIRED)

#find_package(HPX REQUIRED)

#hpx_setup_target(loops COMPONENT_DEPENDENCIES iostreams)


#TODO: remove when c++20 is realeased
TARGET_COMPILE_OPTIONS(loops INTERFACE 
    -fcoroutines
    )



TARGET_LINK_LIBRARIES(loops PRIVATE INTERFACE project_warnings project_options)
