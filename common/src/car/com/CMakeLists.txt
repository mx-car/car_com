add_library(car_com SHARED message.cpp objects/object.cpp)
install(TARGETS car_com  LIBRARY DESTINATION lib)

add_subdirectory(pc)

