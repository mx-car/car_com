

find_package(Boost COMPONENTS     program_options ) 
link_libraries(${Boost_LIBRARIES})

add_executable(demo_car_com main_com_demo.cpp callback.cpp)
target_link_libraries(demo_car_com car_com ${Boost_LIBRARIES})

install(TARGETS demo_car_com
  # IMPORTANT: Add the demo_car_com executable to the "export-set"
  EXPORT CarComTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)

set(CMAKE_CXX_FLAGS -pthread)
  
add_executable(demo_gamepad main_gamepad.cpp callback.cpp)
target_link_libraries(demo_gamepad car_com ${Boost_LIBRARIES} ${MXJoystick_LIBRARIES})

install(TARGETS demo_gamepad
  # IMPORTANT: Add the demo_gamepad executable to the "export-set"
  EXPORT CarComTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)


