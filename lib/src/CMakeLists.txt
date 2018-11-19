

ot_add_current_dir_to_include_dirs ()

ot_add_source_file ( LinearModelAlgorithm.cxx )
ot_add_source_file ( LinearModelAnalysis.cxx )
ot_add_source_file ( LinearModelResult.cxx )
ot_add_source_file ( LinearModelStepwiseAlgorithm.cxx )

ot_install_header_file ( LinearModelAlgorithm.hxx )
ot_install_header_file ( LinearModelAnalysis.hxx )
ot_install_header_file ( LinearModelResult.hxx )
ot_install_header_file ( LinearModelStepwiseAlgorithm.hxx )


include_directories (${INTERNAL_INCLUDE_DIRS})

add_library (otlm ${SOURCEFILES})
set_target_properties (otlm PROPERTIES POSITION_INDEPENDENT_CODE ON)
if (BUILD_SHARED_LIBS)
  set_target_properties (otlm PROPERTIES COMPILE_DEFINITIONS "OTLM_DLL_EXPORTS")
endif ()

if ( NOT DEFINED LIB_VERSION ) 
  set ( LIB_VERSION 0.0.0 )
endif ()
if ( NOT DEFINED LIB_SOVERSION ) 
  set ( LIB_SOVERSION 0 )
endif ()
set_target_properties ( otlm PROPERTIES VERSION ${LIB_VERSION} )
set_target_properties ( otlm PROPERTIES SOVERSION ${LIB_SOVERSION} )
target_link_libraries (otlm ${OPENTURNS_LIBRARY})

# Add targets to the build-tree export set
export (TARGETS otlm FILE ${PROJECT_BINARY_DIR}/otlm-Targets.cmake)

# Install the export set for use with the install-tree
install(EXPORT otlm-Targets
        DESTINATION "${OTLM_CONFIG_CMAKE_PATH}"
        COMPONENT Development)

install(TARGETS otlm
        EXPORT  otlm-Targets 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${OTLM_LIBRARY_PATH}
        ARCHIVE DESTINATION ${OTLM_LIBRARY_PATH}
)

