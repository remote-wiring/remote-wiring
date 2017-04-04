add_library(firmata ../firmata/FirmataMarshaller.cpp ../firmata/FirmataParser.cpp)
add_dependencies(firmata serial_wiring)

add_library(remote_wiring FirmataDevice.cpp)
add_dependencies(remote_wiring firmata serial_wiring)
