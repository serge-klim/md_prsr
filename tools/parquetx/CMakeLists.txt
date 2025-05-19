find_package(Boost CONFIG REQUIRED )
find_package(Arrow CONFIG REQUIRED)
find_package(Parquet CONFIG REQUIRED)

add_library(parquetx INTERFACE)

add_library(parquetx::parquetx ALIAS parquetx)

target_sources(parquetx PUBLIC FILE_SET HEADERS
				FILES
                parquet.hpp
			   )

target_link_libraries(parquetx
		INTERFACE 
		Boost::boost
        Arrow::arrow_static
        Parquet::parquet_static
)

target_include_directories(parquetx INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS parquetx
        EXPORT parquetx-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/parquetx
)

install(EXPORT parquetx-targets
		FILE parquetx-targets.cmake
		NAMESPACE parquetx::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)




