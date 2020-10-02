
add_library(car_com_interface interface.cpp TimeoutSerial.cpp)
target_link_libraries(car_com_interface car_com ${Boost_LIBRARIES})

