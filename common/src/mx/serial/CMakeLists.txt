
add_library(mx_serial
   TimeoutSerial.cpp
   serial_base.cpp
   serial_message.cpp
   serial_interface.cpp
)
target_link_libraries(mx_serial ${Boost_LIBRARIES})
add_subdirectory(example)
