add_library(tiny_stl
	"src/tiny_stl.cpp"
)

target_link_libraries(tiny_stl
	PRIVATE
		fast_float
)

target_include_directories(tiny_stl
	PUBLIC
		"include"
)

target_compile_features(tiny_stl PUBLIC cxx_std_14)