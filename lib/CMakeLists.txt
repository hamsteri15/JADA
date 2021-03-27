SET(LibrarySubDirs 
    loops
    stencil
    grid
    communication
    ) #add more here if necessary
SUBDIRS(${LibrarySubDirs} )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
