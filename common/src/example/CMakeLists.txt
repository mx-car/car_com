add_executable(demo_car_com main_com_demo.cpp)

target_link_libraries(demo_car_com car_com ${Boost_LIBRARIES})

install(TARGETS demo_car_com
  # IMPORTANT: Add the demo_car_com executable to the "export-set"
  EXPORT CarComTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
