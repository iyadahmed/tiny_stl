add_executable(example "example.cpp")
target_link_libraries(example PRIVATE tiny_stl)

if(MSVC)
  target_link_options(example PRIVATE $<$<CONFIG:RELWITHDEBINFO>:/PROFILE>)
endif()