find_package(Boost COMPONENTS unit_test_framework REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_executable (eobi_v13_1_test
		eobi_v13_1_test.cpp
)

target_link_libraries(eobi_v13_1_test
		transcoder::transcoder
		md_prsr::eobi_v13_1
		test_helpers
	    Boost::disable_autolinking
        Boost::boost
		Boost::unit_test_framework
)

target_compile_options(eobi_v13_1_test PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
)
