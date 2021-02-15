set(CMAKE_CXX_STANDARD 17)


find_package(MXJoystick REQUIRED) # https://github.com/mx-robotics/mx_joystick

include_directories(include)
add_subdirectory(src)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.h")
