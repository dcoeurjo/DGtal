# -----------------------------------------------------------------------------
# Project definition
# -----------------------------------------------------------------------------
project (DGtal)
cmake_minimum_required (VERSION 2.6)

# By default, do not warn when built on machines using only VS Express:
IF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
ENDIF()

# -----------------------------------------------------------------------------
# Define variables and cmake parameters, and display some information
# -----------------------------------------------------------------------------
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
SET(DGtal_VERSION_MAJOR 0)
SET(DGtal_VERSION_MINOR 8)
SET(DGtal_VERSION_PATCH beta)
SET(DGTAL_VERSION "${DGtal_VERSION_MAJOR}.${DGtal_VERSION_MINOR}.${DGtal_VERSION_PATCH}")
SET(PROJECT_VERSION "${DGtal_VERSION_MAJOR}.${DGtal_VERSION_MINOR}.${DGtal_VERSION_PATCH}")
SET(VERSION ${DGtal_VERSION_MAJOR}.${DGtal_VERSION_MINOR}.${DGtal_VERSION_PATCH})
# -----------------------------------------------------------------------------
# Common build options/settings
# -----------------------------------------------------------------------------
INCLUDE(Common)

# -----------------------------------------------------------------------------
# Common build options/settings
# -----------------------------------------------------------------------------
INCLUDE(OSDependentSettings)

# -----------------------------------------------------------------------------
# Check Mandatory Dependencies
# -----------------------------------------------------------------------------
INCLUDE(CheckDGtalDependencies)

# -----------------------------------------------------------------------------
# Check Optional Dependencies
# -----------------------------------------------------------------------------
INCLUDE(CheckDGtalOptionalDependencies)

# -----------------------------------------------------------------------------
# Debug, Cpack and Ctest settings
# -----------------------------------------------------------------------------
INCLUDE(CpackCtest)

# -----------------------------------------------------------------------------
# Coverage
# -----------------------------------------------------------------------------
#INCLUDE(CodeCoverage)
## lcov target
ADD_CUSTOM_TARGET(lcov)
ADD_CUSTOM_COMMAND(TARGET lcov
    COMMAND mkdir -p coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
ADD_CUSTOM_COMMAND(TARGET lcov
    COMMAND lcov --directory . --zerocounters
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
ADD_CUSTOM_COMMAND(TARGET lcov
    COMMAND make test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
ADD_CUSTOM_COMMAND(TARGET lcov
    COMMAND lcov --directory . --capture --output-file ./coverage/stap_all.info --no-checksum
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
ADD_CUSTOM_COMMAND(TARGET lcov
    COMMAND lcov --directory . -r ./coverage/stap_all.info ${REMOVE_PATTERN} --output-file ./coverage/stap.info
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )


ADD_CUSTOM_COMMAND(TARGET lcov
    COMMAND genhtml -o ./coverage --title "Code Coverage" --legend --show-details --demangle-cpp ./coverage/stap.info
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
ADD_CUSTOM_COMMAND(TARGET lcov
    COMMAND echo "Open ${CMAKE_BINARY_DIR}/coverage/index.html to view the coverage analysis results."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
# -----------------------------------------------------------------------------
# Custom command/targets depending on the cmake generator
# -----------------------------------------------------------------------------
INCLUDE(GeneratorSpecific)

#------------------------------------------------------------------------------
# Add subdirectories
#------------------------------------------------------------------------------
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

#------------------------------------------------------------------------------
# Examples
#------------------------------------------------------------------------------
INCLUDE(BuildExamples)

# -----------------------------------------------------------------------------
# Install settings
# -----------------------------------------------------------------------------
INCLUDE(Install)
