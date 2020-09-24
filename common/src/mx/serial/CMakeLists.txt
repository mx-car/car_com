
add_library(mx_serial_arduino
   TimeoutSerial.cpp
   serial_base.cpp
   serial_message.cpp
   serial_interface.cpp
)
target_link_libraries(mx_serial_arduino ${Boost_LIBRARIES})
add_subdirectory(example)
