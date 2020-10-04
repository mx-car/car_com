set(CMAKE_CXX_STANDARD 17)


include_directories(include)
add_subdirectory(src)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.h")
