add_library(cme_mdp3_sbe INTERFACE)

add_library (md_prsr::cme_mdp3_sbe ALIAS cme_mdp3_sbe)

target_sources(cme_mdp3_sbe PUBLIC FILE_SET HEADERS
				FILES
				sbe.hpp
			   )

target_link_libraries(cme_mdp3_sbe INTERFACE)

target_include_directories(cme_mdp3_sbe INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS cme_mdp3_sbe
        EXPORT cme_mdp3_sbe-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/cme/mdp3/sbe
)

install(EXPORT cme_mdp3_sbe-targets
		FILE cme_mdp3_sbe-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)




