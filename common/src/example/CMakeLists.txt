

add_executable(car_com_demo main_com_demo.cpp)
target_link_libraries(car_com_demo car_com car_com_interface ${Boost_LIBRARIES})
install(TARGETS car_com_demo  RUNTIME DESTINATION bin)
