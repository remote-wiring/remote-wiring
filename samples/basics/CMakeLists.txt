add_executable(AnalogReadSerial AnalogReadSerial.cpp)
target_include_directories(AnalogReadSerial PUBLIC ${CMAKE_SOURCE_DIR}/samples)
target_link_libraries(AnalogReadSerial LINK_PUBLIC remote_wiring firmata serial_wiring pthread)
add_dependencies(AnalogReadSerial remote_wiring firmata serial_wiring)

add_executable(BareMinimum BareMinimum.cpp)
target_include_directories(BareMinimum PUBLIC ${CMAKE_SOURCE_DIR}/samples)
target_link_libraries(BareMinimum LINK_PUBLIC remote_wiring firmata serial_wiring pthread)
add_dependencies(BareMinimum remote_wiring firmata serial_wiring)

add_executable(Blink Blink.cpp)
target_include_directories(Blink PUBLIC ${CMAKE_SOURCE_DIR}/samples)
target_link_libraries(Blink LINK_PUBLIC remote_wiring firmata serial_wiring pthread)
add_dependencies(Blink remote_wiring firmata serial_wiring)

add_executable(DigitalReadSerial DigitalReadSerial.cpp)
target_include_directories(DigitalReadSerial PUBLIC ${CMAKE_SOURCE_DIR}/samples)
target_link_libraries(DigitalReadSerial LINK_PUBLIC remote_wiring firmata serial_wiring pthread)
add_dependencies(DigitalReadSerial remote_wiring firmata serial_wiring)

add_executable(Fade Fade.cpp)
target_include_directories(Fade PUBLIC ${CMAKE_SOURCE_DIR}/samples)
target_link_libraries(Fade LINK_PUBLIC remote_wiring firmata serial_wiring pthread)
add_dependencies(Fade remote_wiring firmata serial_wiring)
