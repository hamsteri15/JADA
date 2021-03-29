
find_package(HPX REQUIRED)

set(CMAKE_CXX_STANDARD 20)


#see https://stellar-ci.org/hpx_test/manual/creating_hpx_projects.html
add_hpx_executable(2d_stencil
    SOURCES 2d_stencil.cpp
    COMPONENT_DEPENDENCIES iostreams partitioned_vector #hpx deps
    DEPENDENCIES grid loops communication project_warnings project_options                           #other deps
    COMPILE_FLAGS       -Wno-unused-variable
) 

#OR
#find_package(HPX REQUIRED)
#add_executable(2d_stencil 2d_stencil.cpp)
#target_link_libraries(2d_stencil HPX::hpx HPX::iostreams_component HPX::containers_component grid loops)
#target_compile_options(2d_stencil PUBLIC -Wno-error)

add_hpx_executable(hpx_test
    SOURCES hpx_test.cpp
    COMPONENT_DEPENDENCIES iostreams partitioned_vector #hpx deps
    DEPENDENCIES grid loops communication project_warnings project_options                           #other deps
    COMPILE_FLAGS       -Wno-unused-variable
) 

