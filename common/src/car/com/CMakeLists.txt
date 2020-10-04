configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_library(car_com SHARED message.cpp objects/object.cpp pc/interface.cpp pc/TimeoutSerial.cpp config.h.in)

set_target_properties(car_com PROPERTIES
  PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/config.h")

install(TARGETS car_com
  # IMPORTANT: Add the car_com library to the "export-set"
  EXPORT CarComTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/car/com"
    COMPONENT dev)

