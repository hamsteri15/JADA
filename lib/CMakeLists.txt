SET(LibrarySubDirs 
    loops
    stencil
    grid
    communication
    operation
    ) #add more here if necessary
SUBDIRS(${LibrarySubDirs} )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
