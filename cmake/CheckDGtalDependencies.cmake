# -----------------------------------------------------------------------------
# Check Mandatory Dependencies
# -----------------------------------------------------------------------------

message(STATUS "-------------------------------------------------------------------------------")
message(STATUS "DGtal required dependencies: ")

# -----------------------------------------------------------------------------
# Looking for boost
# -----------------------------------------------------------------------------
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_FOUND FALSE)
FIND_PACKAGE(Boost 1.50.0 REQUIRED)
if ( Boost_FOUND )
  ADD_DEFINITIONS(${BOOST_DEFINITIONS} -DBOOST_ALL_NO_LIB)
  # SYSTEM to avoid warnings from boost.
  include_directories(SYSTEM ${Boost_INCLUDE_DIRS} )
  SET(DGtalLibInc ${DGtalLibInc} ${Boost_INCLUDE_DIRS})
endif( Boost_FOUND )

# -----------------------------------------------------------------------------
# Looking for zlib
# -----------------------------------------------------------------------------
set(ZLIB_FOUND FALSE)
FIND_PACKAGE(ZLIB REQUIRED)
if ( ZLIB_FOUND )
  include_directories(SYSTEM ${ZLIB_INCLUDE_DIRS} )
  SET(DGtalLibInc ${DGtalLibInc} ${ZLIB_INCLUDE_DIRS})
  SET(DGtalLibDependencies ${DGtalLibDependencies} ${ZLIB_LIBRARIES})
endif( ZLIB_FOUND )


# -----------------------------------------------------------------------------
# Check some CPP11 features in the compiler
# -----------------------------------------------------------------------------
MESSAGE(STATUS "Checking C++11 compatibility:")
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
MESSAGE(STATUS "  c++11 enabled by cmake. ")


# -----------------------------------------------------------------------------
# Fixing Catch issue for C++11 and old GCC
# -----------------------------------------------------------------------------
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 4.7
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
    add_definitions("-DCATCH_CONFIG_CPP11_NO_IS_ENUM")
    message(STATUS "Patching Catch for gcc < 4.7")
  endif()
endif()


# -----------------------------------------------------------------------------
# Setting librt dependency on Linux
# -----------------------------------------------------------------------------
if (UNIX AND NOT(APPLE))
  SET(DGtalLibDependencies ${DGtalLibDependencies} -lrt)
endif()
