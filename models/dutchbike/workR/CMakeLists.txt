
#   Universite catholique de Louvain
#   CEREM : Centre for research in mechatronics
#   http://www.robotran.be
#   Contact : info@robotran.be
#
#
# CMake for compiling a simple robotran project in C
#

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                       PROJECT MAIN CONFIGURATIONS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# CMake minimum version
cmake_minimum_required(VERSION 3.11)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# project name
project (DutchBike)

# message to display the project name
message(STATUS "Processing ${PROJECT_NAME}")

find_package(mbsysc 1.26 COMPONENTS module load utilities numerics realtime)
message("Using MBsysC ${MBSYSC_VERSION} from ${MBSYSC_DIRECTORY}")
add_definitions( "-DMBSYSC_PATH=\"${MBSYSC_DIRECTORY}\"")

# Change default installation directory in case of not specified by user
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)  # Requires v3.7.1
    set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install"
         CACHE PATH "default install path" FORCE)
endif()

include_directories(${MBSYSC_INCLUDE_DIRECTORIES})

file(GLOB_RECURSE SYMBOLIC_SRC "../symbolicR/*.c")
file(GLOB_RECURSE USER_SRC "../userfctR/*.c")
file(GLOB MAIN_SRC "src/*.c")
include_directories("../symbolicR")
include_directories("../userfctR")
include_directories("src")


add_compile_definitions("BUILD_PATH=\"${CMAKE_CURRENT_BINARY_DIR}\"")
add_compile_definitions("PROJECT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

# Windows M_PI definitions
if (WIN32)
    add_definitions(-D _USE_MATH_DEFINES)
endif (WIN32)

# generate the libraries
add_library (Project_user SHARED ${USER_SRC})
add_library (Project_symbolic SHARED ${SYMBOLIC_SRC})
target_link_libraries(Project_user ${MBSYSC_LIBRARIES})
target_link_libraries(Project_symbolic ${MBSYSC_LIBRARIES})

# generate the executable
add_executable (exe_${PROJECT_NAME} ${MAIN_SRC})
add_dependencies(exe_${PROJECT_NAME} Project_user Project_symbolic)
target_link_libraries(exe_${PROJECT_NAME} ${MBSYSC_LIBRARIES} Project_user)

# installation process
install(TARGETS Project_user DESTINATION .)
install(TARGETS Project_symbolic DESTINATION .)
install(TARGETS exe_${PROJECT_NAME} DESTINATION .)
