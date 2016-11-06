

ot_add_current_dir_to_include_dirs ()

ot_add_source_file ( LinearModelAlgorithm.cxx )
ot_add_source_file ( LinearModelAnalysis.cxx )
ot_add_source_file ( LinearModelResult.cxx )
ot_add_source_file ( LinearModelStepwiseAlgorithm.cxx )
ot_add_source_file ( MonomialFactory.cxx )

ot_install_header_file ( LinearModelAlgorithm.hxx )
ot_install_header_file ( LinearModelAnalysis.hxx )
ot_install_header_file ( LinearModelResult.hxx )
ot_install_header_file ( LinearModelStepwiseAlgorithm.hxx )
ot_install_header_file ( MonomialFactory.hxx )


include_directories ( ${INTERNAL_INCLUDE_DIRS} )

add_library ( otlmr ${SOURCEFILES} )
if ( BUILD_SHARED_LIBS )
  set_target_properties ( otlmr PROPERTIES COMPILE_DEFINITIONS "OTLMR_DLL_EXPORTS" )
endif ()

if ( NOT DEFINED LIB_VERSION ) 
  set ( LIB_VERSION 0.0.0 )
endif ()
if ( NOT DEFINED LIB_SOVERSION ) 
  set ( LIB_SOVERSION 0 )
endif ()
set_target_properties ( otlmr PROPERTIES VERSION ${LIB_VERSION} )
set_target_properties ( otlmr PROPERTIES SOVERSION ${LIB_SOVERSION} )
target_link_libraries (otlmr ${OPENTURNS_LIBRARY})

# Add targets to the build-tree export set
export (TARGETS otlmr FILE ${PROJECT_BINARY_DIR}/otlmr-Targets.cmake)

# Install the export set for use with the install-tree
install(EXPORT otlmr-Targets
        DESTINATION "${OTLMR_CONFIG_CMAKE_PATH}"
        COMPONENT Development)

install(TARGETS otlmr
        EXPORT  otlmr-Targets 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${OTLMR_LIBRARY_PATH}
        ARCHIVE DESTINATION ${OTLMR_LIBRARY_PATH}
)

