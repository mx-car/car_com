
add_library(car_com_interface SHARED interface.cpp TimeoutSerial.cpp)
target_link_libraries(car_com_interface car_com ${Boost_LIBRARIES})
install(TARGETS car_com_interface  LIBRARY DESTINATION lib)

