cmake_minimum_required ( VERSION 2.8 )

option ( BUILD_PYTHON                 "Build the python module for the library"                               ON )
option (USE_SPHINX                    "Use sphinx for documentation"                                          ON)
option ( LINK_PYTHON_LIBRARY          "Link python modules against python library"                            ON )
option ( BUILD_SHARED_LIBS            "Build shared libraries"                                                ON )

# By default, build in Release mode. Must appear before project() command
if ( NOT DEFINED CMAKE_BUILD_TYPE )
   set( CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif ()

project ( otlm CXX )

string ( TOLOWER ${CMAKE_PROJECT_NAME} PACKAGE_NAME )

# set module dir to find custom scripts
list ( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

find_package (OpenTURNS NO_MODULE REQUIRED)
if (OPENTURNS_FOUND)
  message (STATUS "Found OpenTURNS: ${OPENTURNS_ROOT_DIR}")
endif ()
include ( ${OPENTURNS_USE_FILE} )
set ( OTLM_DEFINITIONS ${OPENTURNS_DEFINITIONS} )
if (NOT BUILD_SHARED_LIBS)
  list ( APPEND OTLM_DEFINITIONS "-DOTLM_STATIC" )
endif ()
add_definitions ( ${OTLM_DEFINITIONS} )

set ( OTLM_LIBRARY_PATH lib${LIB_SUFFIX} )
set ( OTLM_INCLUDE_PATH include)
set ( OTLM_SWIG_INCLUDE_PATH ${OTLM_INCLUDE_PATH}/${PACKAGE_NAME}/swig )
set ( OTLM_DATA_PATH share )
set ( OTLM_EXAMPLE_PATH ${OTLM_DATA_PATH}/${PACKAGE_NAME}/examples )
set ( OTLM_CONFIG_CMAKE_PATH lib${LIB_SUFFIX}/cmake/otlm )
set (OTLM_DOC_PATH share/${PACKAGE_NAME}/doc)

if (BUILD_PYTHON)
  # workaround for cmake bug #0013449
  if (CMAKE_VERSION VERSION_LESS 3 AND DEFINED CMAKE_FIND_ROOT_PATH)
    find_program (SWIG_EXECUTABLE NAMES swig2.0 swig)
    if (SWIG_EXECUTABLE)
      set (SWIG_USE_FILE ${CMAKE_ROOT}/Modules/UseSWIG.cmake)
      set (SWIG_FOUND TRUE)
    endif ()
  else ()
    # we need absolute module imports
    find_package (SWIG 2.0.9)
  endif ()
  if (SWIG_FOUND)
    include (${SWIG_USE_FILE})
  endif ()

  find_package ( PythonInterp )
  find_package ( PythonLibs )
  include (FindPythonModule)

  if (USE_SPHINX)
    find_program (SPHINX_EXECUTABLE NAMES sphinx-build DOC "Sphinx Documentation Builder (sphinx-doc.org)")
    if (SPHINX_EXECUTABLE)
      find_python_module (numpydoc)
      if (NUMPYDOC_FOUND)
        find_python_module (sphinx 1.1)
      endif ()
    endif ()
    if (NOT SPHINX_FOUND)
      message (STATUS "Disabling documentation")
    endif ()
  endif ()

  if ( PYTHONINTERP_FOUND )
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print(sysconfig.get_python_lib(plat_specific=True, prefix='${CMAKE_INSTALL_PREFIX}'))"
                      OUTPUT_VARIABLE _ABS_PYTHON_MODULE_PATH
                      RESULT_VARIABLE _PYTHON_pythonlib_result 
                      OUTPUT_STRIP_TRAILING_WHITESPACE )

    if (NOT _PYTHON_pythonlib_result) 
      get_filename_component ( _ABS_PYTHON_MODULE_PATH ${_ABS_PYTHON_MODULE_PATH} ABSOLUTE )
      file ( RELATIVE_PATH _REL_PYTHON_MODULE_PATH ${CMAKE_INSTALL_PREFIX} ${_ABS_PYTHON_MODULE_PATH} )                   

      set (OTLM_PYTHON_MODULE_PATH ${_REL_PYTHON_MODULE_PATH})

    else ()
      message ( SEND_ERROR "Could not run ${PYTHON_EXECUTABLE}" ) 
    endif ()

    if (CMAKE_VERSION VERSION_LESS "2.8.6")
      execute_process (COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print(sys.version_info[0])"
                       OUTPUT_VARIABLE PYTHON_VERSION_MAJOR
                       OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif ()
    set (OTLM_PYTHON${PYTHON_VERSION_MAJOR}_MODULE_PATH ${OTLM_PYTHON_MODULE_PATH})
  endif ( PYTHONINTERP_FOUND )
endif ()


# WARNING: This is where variables without OTLM_ prefix are defined !
foreach (_var PYTHON_MODULE_PATH LIBRARY_PATH INCLUDE_PATH CONFIG_CMAKE_PATH)
  file (TO_NATIVE_PATH "${OTLM_${_var}}" NATIVE_OTLM_${_var})
  if (NOT IS_ABSOLUTE ${NATIVE_OTLM_${_var}})
    file (TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${OTLM_${_var}} NATIVE_OTLM_${_var})
  endif ()
  string (REPLACE "\\" "\\\\" ${_var} ${NATIVE_OTLM_${_var}})
endforeach ()

# RPATH settings
set (CMAKE_INSTALL_RPATH ${LIBRARY_PATH} ${CMAKE_Fortran_IMPLICIT_LINK_DIRECTORIES})
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(POLICY CMP0042)
  # Set MACOSX_RPATH to ON
  cmake_policy(SET CMP0042 NEW)
endif()

# Some useful macros to ease CMakeLists.txt file writing
set ( SOURCEFILES "" CACHE INTERNAL "List of source files to compile" )
macro ( ot_add_source_file FILENAME )
  set ( sf ${SOURCEFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( SOURCEFILES ${sf} CACHE INTERNAL "List of source files to compile" )
endmacro ( ot_add_source_file )

set ( HEADERFILES "" CACHE INTERNAL "List of header files to install" )
macro ( ot_install_header_file FILENAME )
  set ( hf ${HEADERFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}/${FILENAME} )
  set ( HEADERFILES ${hf} CACHE INTERNAL "List of header files to install" )
endmacro ( ot_install_header_file )

set ( SWIGFILES "" CACHE INTERNAL "List of SWIG files to install" )
macro ( ot_install_swig_file FILENAME )
  set ( hf ${SWIGFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( SWIGFILES ${hf} CACHE INTERNAL "List of SWIG files to install" )
endmacro ( ot_install_swig_file )

set ( INTERNAL_INCLUDE_DIRS "" CACHE INTERNAL "List of directories with header files needed for build" )
macro ( ot_add_current_dir_to_include_dirs )
  set ( inc_dirs ${INTERNAL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
  set ( INTERNAL_INCLUDE_DIRS ${inc_dirs} CACHE INTERNAL "List of directories with header files needed for build" )
endmacro ( ot_add_current_dir_to_include_dirs )


set ( CPACK_PACKAGE_NAME          ${PACKAGE_NAME} )
set ( CPACK_PACKAGE_VERSION_MAJOR 0              )
set ( CPACK_PACKAGE_VERSION_MINOR 6              )
set ( CPACK_PACKAGE_VERSION_PATCH                )
set ( CPACK_SOURCE_GENERATOR      "TGZ;TBZ2"     )
set ( CPACK_BINARY_STGZ           "OFF"          )
set ( CPACK_BINARY_TBZ2           "ON"           )
set ( CPACK_BINARY_TGZ            "ON"           )
set ( CPACK_BINARY_TZ             "OFF"          )
set ( CPACK_SOURCE_IGNORE_FILES "/.git;/build;.*~;${CPACK_SOURCE_IGNORE_FILES}" )
set ( PACKAGE_NAME           ${CPACK_PACKAGE_NAME} )
set ( PACKAGE_VERSION        ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR} )
if ( CPACK_PACKAGE_VERSION_PATCH )
  set ( PACKAGE_VERSION       ${PACKAGE_VERSION}.${CPACK_PACKAGE_VERSION_PATCH} )
endif ()
set ( CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION} )


# The tests can't be run if this function is absent
enable_testing ()
add_custom_target (tests COMMENT "Build tests")
add_custom_target (check COMMENT "Run pre-installation tests")
add_custom_target (installcheck COMMENT "Run post-installation tests")

add_subdirectory (lib)
add_dependencies (check cppcheck)

if (PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND AND SWIG_FOUND AND BUILD_SHARED_LIBS)
  add_subdirectory (python)
  add_dependencies (installcheck pyinstallcheck)
endif ()


# uninstall target
configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
  IMMEDIATE @ONLY
)

add_custom_target ( uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

set ( OTLM_LIBRARY                 otlm )
set ( OTLM_LIBRARIES ${OPENTURNS_LIBRARIES} )
list ( APPEND OTLM_LIBRARIES ${OTLM_LIBRARY} )
set ( OTLM_LIBRARY_DIRS ${OPENTURNS_LIBRARY_DIRS} )
list ( APPEND OTLM_LIBRARY_DIRS ${LIBRARY_PATH} )
set ( OTLM_INCLUDE_DIR ${INCLUDE_PATH} )
set ( OTLM_INCLUDE_DIRS ${OTLM_INCLUDE_DIR} )
set ( OTLM_ROOT_DIR       ${CMAKE_INSTALL_PREFIX} )
list ( APPEND OTLM_INCLUDE_DIRS ${OPENTURNS_INCLUDE_DIRS} )
set ( OTLM_VERSION_STRING ${PACKAGE_VERSION} )
set ( OTLM_VERSION_MAJOR ${CPACK_PACKAGE_VERSION_MAJOR} )
set ( OTLM_VERSION_MINOR ${CPACK_PACKAGE_VERSION_MINOR} )
set ( OTLM_VERSION_PATCH ${CPACK_PACKAGE_VERSION_PATCH} )
set ( OTLM_USE_FILE      ${CONFIG_CMAKE_PATH}/Useotlm.cmake )

if (CMAKE_VERSION VERSION_LESS 2.8.8)
  # CMakePackageConfigHelpers is not available
  set (PACKAGE_INIT "
# Generated by a CMake version without CMakePackageConfigHelpers.
macro(set_and_check _var _value)
  set(\${_var} \"\${_value}\")
endmacro(set_and_check _var _value)
"     )
  foreach (_var
      OTLM_USE_FILE
      OTLM_ROOT_DIR
      OTLM_INCLUDE_DIR
      OTLM_INCLUDE_DIRS
      OTLM_LIBRARY_DIRS )
    set(PACKAGE_${_var} ${${_var}})
  endforeach ()
  
configure_file ( ${CMAKE_SOURCE_DIR}/cmake/otlmConfig.cmake.in
                 ${CMAKE_BINARY_DIR}/otlmConfig.cmake
                 @ONLY ESCAPE_QUOTES
               )

install ( FILES cmake/Useotlm.cmake 
                ${CMAKE_BINARY_DIR}/otlmConfig.cmake
          DESTINATION ${OTLM_CONFIG_CMAKE_PATH}
        )

else ()

  include(CMakePackageConfigHelpers)
  configure_package_config_file (
    cmake/otlmConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/otlmConfig.cmake
    INSTALL_DESTINATION ${OTLM_CONFIG_CMAKE_PATH}
    PATH_VARS
      OTLM_USE_FILE
      OTLM_ROOT_DIR
      OTLM_INCLUDE_DIR
      OTLM_INCLUDE_DIRS
      OTLM_LIBRARY_DIRS
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
                                )                   
  write_basic_package_version_file( otlmConfigVersion.cmake VERSION ${OTLM_VERSION_STRING} COMPATIBILITY AnyNewerVersion )               

  install ( FILES cmake/Useotlm.cmake
                  ${CMAKE_CURRENT_BINARY_DIR}/otlmConfig.cmake
                  ${CMAKE_CURRENT_BINARY_DIR}/otlmConfigVersion.cmake
            DESTINATION ${OTLM_CONFIG_CMAKE_PATH}
          )
endif ()

include (CPack)
