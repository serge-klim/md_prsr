find_package(Boost REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_library(b3_umdf_v2_3_1 INTERFACE)

add_library (md_prsr::b3_umdf_v2_3_1 ALIAS b3_umdf_v2_3_1)

target_sources(b3_umdf_v2_3_1 PUBLIC FILE_SET HEADERS
				FILES
				messages.hpp
				describe.hpp
				transcoder.hpp
				io.hpp
			   )

target_link_libraries(b3_umdf_v2_3_1
		INTERFACE 
		md_prsr::cme_mdp3_sbe
		transcoder::transcoder
		Boost::boost
)

target_include_directories(b3_umdf_v2_3_1 INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS b3_umdf_v2_3_1
        EXPORT b3_umdf_v2_3_1-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/cme/mdp3/v13
)

install(EXPORT b3_umdf_v2_3_1-targets
		FILE b3_umdf_v2_3_1-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)




