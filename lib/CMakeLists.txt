SET(LibrarySubDirs 
    loops
    stencil
    operation
    grid
    ) #add more here if necessary
SUBDIRS(${LibrarySubDirs} )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
