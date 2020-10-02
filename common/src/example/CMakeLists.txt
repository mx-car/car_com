

add_executable(com_demo main_com_demo.cpp)
target_link_libraries(com_demo car_com car_com_interface ${Boost_LIBRARIES})
