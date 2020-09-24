

add_executable(serial_demo main_serial_demo.cpp)
target_link_libraries(serial_demo mx_serial ${Boost_LIBRARIES})