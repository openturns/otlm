
include (${SWIG_USE_FILE})
include (TargetLinkLibrariesWithDynamicLookup)

include_directories ( ${OPENTURNS_SWIG_INCLUDE_DIRS} )
add_definitions( ${OPENTURNS_SWIG_DEFINITIONS} )
include_directories ( ${INTERNAL_INCLUDE_DIRS} )
include_directories ( ${PYTHON_INCLUDE_DIRS} )

add_custom_target( generate_docstrings )

macro ( ot_add_python_module MODULENAME SOURCEFILE )
  set_source_files_properties ( ${SOURCEFILE} PROPERTIES CPLUSPLUS ON )
  set_source_files_properties ( ${SOURCEFILE} PROPERTIES SWIG_MODULE_NAME ${MODULENAME} )
  ot_install_swig_file ( ${SOURCEFILE} )
  if (${ARGC} GREATER 2)
    set (SWIG_MODULE_${MODULENAME}_python_EXTRA_DEPS ${ARGN})
    list (APPEND SWIG_MODULE_${MODULENAME}_python_EXTRA_DEPS generate_docstrings)
  endif ()

  set ( swig_other_sources )
  foreach ( it ${ARGN} )
    if ( ${it} MATCHES ".*\\.i$" )
      ot_install_swig_file ( ${it} )
    elseif(${it} MATCHES ".*_doc\\.i\\.in$")
      # handle docstring generation
      set (DOCSTRING_IN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${it})
      get_filename_component ( DOCSTRING_FILE_BASENAME ${DOCSTRING_IN_FILE} NAME_WE )
      add_custom_target ( generate_${DOCSTRING_FILE_BASENAME}
                          COMMAND ${CMAKE_COMMAND} -DDOCSTRING_IN_FILE=${DOCSTRING_IN_FILE} -DDOCSTRING_FILE=${CMAKE_CURRENT_BINARY_DIR}/${DOCSTRING_FILE_BASENAME}.i -P ${CMAKE_SOURCE_DIR}/cmake/escape_backslash_x4.cmake
                          DEPENDS ${DOCSTRING_IN_FILE} )
      add_dependencies ( generate_docstrings generate_${DOCSTRING_FILE_BASENAME} )
      list ( APPEND SWIGFILES ${CMAKE_CURRENT_BINARY_DIR}/${DOCSTRING_FILE_BASENAME}.i )
    else ()
      set ( swig_other_sources ${swig_other_sources} "${it}" )
    endif ()
  endforeach ()

  # swig_add_module is deprecated
  if (CMAKE_VERSION VERSION_LESS 3.8)
    swig_add_module (${MODULENAME}_python python ${SOURCEFILE} ${swig_other_sources} )
  else ()
    swig_add_library (${MODULENAME}_python LANGUAGE python SOURCES ${SOURCEFILE} ${swig_other_sources})
  endif ()

  # UseSWIG generates now standard target names
  if (CMAKE_VERSION VERSION_LESS 3.13)
    set (module_target ${SWIG_MODULE_${MODULENAME}_python_REAL_NAME})
  else ()
    set (module_target ${MODULENAME}_python)
  endif ()

  swig_link_libraries (${MODULENAME}_python otlm)

  target_link_libraries_with_dynamic_lookup (${module_target} ${PYTHON_LIBRARIES})

  set_target_properties (${module_target} PROPERTIES OUTPUT_NAME _${MODULENAME})

  if (CMAKE_VERSION VERSION_LESS 3.1)
    set_target_properties (${module_target} PROPERTIES NO_SONAME ON)
  endif ()

  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${MODULENAME}.py
           DESTINATION ${OTLM_PYTHON_MODULE_PATH}/${PACKAGE_NAME})
  install (TARGETS ${module_target}
            LIBRARY DESTINATION ${OTLM_PYTHON_MODULE_PATH}/${PACKAGE_NAME})

  list (APPEND OTLM_PYTHON_MODULES ${MODULENAME})
endmacro ()



ot_add_python_module( ${PACKAGE_NAME} ${PACKAGE_NAME}_module.i 
                      LinearModelAlgorithm.i LinearModelAlgorithm_doc.i.in
                      LinearModelAnalysis.i LinearModelAnalysis_doc.i.in
                      LinearModelResult.i LinearModelResult_doc.i.in
                      LinearModelStepwiseAlgorithm.i LinearModelStepwiseAlgorithm_doc.i.in
                    )


install ( FILES __init__.py
          DESTINATION ${OTLM_PYTHON_MODULE_PATH}/${PACKAGE_NAME}
        )

install ( FILES ${SWIGFILES}
          DESTINATION ${OTLM_SWIG_INCLUDE_DIRS}
        )




if (SPHINX_FOUND)

  # create a build site-package from which sphinx can import
  set (OTLM_PYTHON_BUILD_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR}/site-packages)
  execute_process (COMMAND ${CMAKE_COMMAND} -E make_directory ${OTLM_PYTHON_BUILD_MODULE_PATH}/otlm)
  foreach (module ${OTLM_PYTHON_MODULES})
    execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/${module}.py ${OTLM_PYTHON_BUILD_MODULE_PATH}/otlm/${module}.py)
    # FIXME: get_target_property (suffix ${SWIG_MODULE_${module}_REAL_NAME} SUFFIX)
    execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/_${module}.so ${OTLM_PYTHON_BUILD_MODULE_PATH}/otlm/_${module}.so)
  endforeach ()
  foreach (file __init__.py)
    execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${OTLM_PYTHON_BUILD_MODULE_PATH}/otlm/${file})
  endforeach ()

  # sphinx configuration
  set (doc_formats)
  list (APPEND doc_formats html)
  list (APPEND doc_formats htmlhelp)
  list (APPEND doc_formats latex)
  list (APPEND doc_formats doctest)
  list (APPEND doc_formats qthelp)
  list (APPEND doc_formats devhelp)

  set (SPHINX_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/sphinx_build)
  set (SPHINX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../doc)

  file (MAKE_DIRECTORY ${SPHINX_BINARY_DIR}/_static) # currently empty

  if (NOT DEFINED OPENTURNS_PYTHON_MODULE_PATH)
    set(OPENTURNS_PYTHON_MODULE_PATH ${OPENTURNS_PYTHON${PYTHON_VERSION_MAJOR}_MODULE_PATH})
  endif ()
  configure_file (${SPHINX_SOURCE_DIR}/conf.py.in ${SPHINX_BINARY_DIR}/conf.py @ONLY)

  # get sphinx sources
  file (GLOB_RECURSE sphinx_sources
    ${SPHINX_SOURCE_DIR}/*.rst
    ${SPHINX_SOURCE_DIR}/*.rst_t
    ${SPHINX_SOURCE_DIR}/*.css_t
    ${SPHINX_SOURCE_DIR}/*.conf
    ${SPHINX_SOURCE_DIR}/*.py
    ${SPHINX_SOURCE_DIR}/*.png
    ${SPHINX_SOURCE_DIR}/*.html
    ${SPHINX_SOURCE_DIR}/*.css
    ${SPHINX_SOURCE_DIR}/*.jpg
    ${SPHINX_SOURCE_DIR}/*.ico
    ${SPHINX_SOURCE_DIR}/*.sty
    ${SPHINX_SOURCE_DIR}/*.txt
    ${SPHINX_SOURCE_DIR}/*.in
    ${SPHINX_SOURCE_DIR}/*.inv
  )

  # copy sphinx sources
  set (sphinx_clones)
  foreach (file ${sphinx_sources})
    if (EXISTS ${file})
      file ( RELATIVE_PATH rel_file ${SPHINX_SOURCE_DIR} ${file} )
      get_filename_component (rel_path ${rel_file} PATH)
      file (MAKE_DIRECTORY ${SPHINX_BINARY_DIR}/${rel_path})
      set (sphinx_clone ${SPHINX_BINARY_DIR}/${rel_file})
      add_custom_command (OUTPUT ${sphinx_clone}
        COMMAND ${CMAKE_COMMAND} -E copy ${file} ${sphinx_clone}
        DEPENDS ${file}
      )
      list (APPEND sphinx_clones ${sphinx_clone})
    endif ()
  endforeach ()

  set (SPHINX_FLAGS "-W" CACHE STRING "sphinx flags")
  separate_arguments (SPHINX_FLAGS)

  # generate sphinx rule for each format
  foreach (format ${doc_formats})
    add_custom_target ( sphinx_${format}
                        COMMAND ${SPHINX_EXECUTABLE}
                                  -b ${format}
                                  -d ${SPHINX_BINARY_DIR}/.doctrees
                                  ${SPHINX_FLAGS}
                                  ${SPHINX_BINARY_DIR}
                                  ${SPHINX_BINARY_DIR}/${format}
                        DEPENDS ${sphinx_clones}
                      )
    foreach (MODULENAME ${OTLM_PYTHON_MODULES})
      if (CMAKE_VERSION VERSION_LESS 3.13)
        set (module_target ${SWIG_MODULE_${MODULENAME}_python_REAL_NAME})
      else ()
        set (module_target ${MODULENAME}_python)
      endif ()
      add_dependencies (sphinx_${format} ${module_target})
    endforeach ()
  endforeach ()

  # build html by default
  set_target_properties (sphinx_html PROPERTIES EXCLUDE_FROM_ALL OFF)

  add_custom_command (OUTPUT ${SPHINX_BINARY_DIR}/latex/OpenTURNS.pdf
                      WORKING_DIRECTORY ${SPHINX_BINARY_DIR}/latex
                      COMMAND make all-pdf
                    )
  add_custom_target (sphinx_pdf DEPENDS ${SPHINX_BINARY_DIR}/latex/OpenTURNS.pdf)
  add_dependencies (sphinx_pdf sphinx_latex)

  install (DIRECTORY ${SPHINX_BINARY_DIR}/html DESTINATION ${OTLM_DOC_PATH})
endif ()
