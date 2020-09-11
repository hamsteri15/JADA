SET(LibrarySubDirs 
    #utils
    loops
    stencil
    operation
    #math 
    #io
    #communication
    ) #add more here if necessary
SUBDIRS(${LibrarySubDirs} )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
