set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS  system date_time thread signals program_options  serialization regex filesystem chrono) 
link_libraries(${Boost_LIBRARIES})

include_directories(include)
add_subdirectory(src)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.h")
