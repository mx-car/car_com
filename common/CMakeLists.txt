set(CMAKE_CXX_STANDARD 17)


find_package(MXJoystick REQUIRED) # https://github.com/mx-robotics/mx_joystick
include_directories(${MXJoystick_INCLUDE_DIRS})

find_package(MXObjects REQUIRED)  # https://github.com/mx-robotics/mx_objects
include_directories(${MXObjects_INCLUDE_DIRS})
message(${MXObjects_INCLUDE_DIRS})
    
include_directories(include)
add_subdirectory(src)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.h")
