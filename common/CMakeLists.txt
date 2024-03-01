set(CMAKE_CXX_STANDARD 17)


find_package(MXJoystick REQUIRED) # https://github.com/mx-robotics/mx_joystick
message(STATUS "MXJoystick_INCLUDE_DIRS ${MXJoystick_INCLUDE_DIRS}")
include_directories(${MXJoystick_INCLUDE_DIRS})

find_package(MXObjects REQUIRED)  # https://github.com/mx-robotics/mx_objects
message(STATUS "MXObjects_INCLUDE_DIRS ${MXObjects_INCLUDE_DIRS}")
include_directories(${MXObjects_INCLUDE_DIRS})
    
include_directories(include)
add_subdirectory(src)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.h")
