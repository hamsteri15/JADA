CMAKE_MINIMUM_REQUIRED(VERSION 3.12)


PROJECT(JADA)

message("COMPILER ID " ${CMAKE_CXX_COMPILER_ID})
#INCLUDE(cmake/StandardProjectSettings.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)




#config boost
include(cmake/ExternalBoost.cmake)

#config hpx
include(cmake/ExternalHpx.cmake)





#target_compile_features(project_options INTERFACE cxx_std_20)
#target_compile_features(project_options INTERFACE cxx_std_17)


add_library(project_warnings INTERFACE)
include(CompilerWarnings.cmake)
set_project_warnings(project_warnings)



#option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)


#Set library and binary paths
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)




#Build jada
INCLUDE_DIRECTORIES(lib)
ADD_SUBDIRECTORY(lib)

ADD_SUBDIRECTORY(app)

#Build jada
#INCLUDE_DIRECTORIES(src)
#ADD_SUBDIRECTORY(src)

if(ENABLE_TESTING)
  enable_testing()
  message(
    "Building Tests."
  )
  add_subdirectory(test)
endif()

