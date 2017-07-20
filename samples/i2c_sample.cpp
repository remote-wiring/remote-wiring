/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include <remote_wiring>
#include <serial_wiring>

#include "Wire.h"

void log (void * context_) {
    serial_wiring::ByteCache & cache = *reinterpret_cast<serial_wiring::ByteCache *>(context_);
    while ( cache.size() ) {
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << cache.readCachedByte() << " " << std::flush;
    }
}

using namespace remote_wiring::boards::arduino::uno;  // change to your board
using namespace remote_wiring::wiring;

void baseFunc (remote_wiring::Wiring & board) {
    // Wiring API syntax
    // (not available without `operator .` override)

    // New object syntax
    remote_wiring::TwoWire & Wire = board.Wire;
    Wire.beginTransmission(55);
    Wire.write('h');

    // Dot syntax
    board.Wire.write('e');
    board.Wire.write('l');
    
    // Pointer syntax
    (&board.Wire)->write('l');
    (&board.Wire)->write('o');

    // Full `static_cast<>()` syntax
    static_cast<remote_wiring::TwoWire &>(board.Wire).endTransmission(false);
    std::cout << std::endl;
    static_cast<remote_wiring::TwoWire &>(board.Wire).requestFrom(55, 6);
    std::cout << std::endl;
}

void deviceFunc (remote_wiring::RemoteDevice & board) {
    // Wiring API syntax
    // (not available without `operator .` override)

    // New object syntax
    remote_wiring::TwoWire & Wire = board.Wire;
    Wire.beginTransmission(55);
    Wire.write('h');

    // Dot syntax
    board.Wire.write('e');
    board.Wire.write('l');
    
    // Pointer syntax
    (&board.Wire)->write('l');
    (&board.Wire)->write('o');

    // Full `static_cast<>()` syntax
    static_cast<remote_wiring::TwoWire &>(board.Wire).endTransmission(false);
    std::cout << std::endl;
    static_cast<remote_wiring::TwoWire &>(board.Wire).requestFrom(55, 6);
    std::cout << std::endl;
}

int main (void) {
    serial_wiring::CacheSerial cache;
    remote_wiring::FirmataDevice board(cache);

    // Connect output to terminal
    cache.registerCachedBytesCallback(log, &cache);
    
    // Establish a communication channel
    cache.begin();

    // Attach to the remote device
    board.attach();
    std::cout << std::endl;
    
    // Survey the board's capabilities
    // (not necessary but allows for error checking)
    board.survey();
    std::cout << std::endl;
    
    // Join I²C bus
    board.Wire.begin();

    // Send a write transaction to slave device at address 55
    board.Wire.beginTransmission(55);
    board.Wire.write('h');
    board.Wire.write('e');
    board.Wire.write('l');
    board.Wire.write('l');
    board.Wire.write('o');
    board.Wire.endTransmission(false);
    std::cout << std::endl;
    
    // Request 6 bytes from slave device at address 55
    board.Wire.requestFrom(55, 6);
    std::cout << std::endl;
    
    // Attempt to call through interfaces
    baseFunc(board);
    deviceFunc(board);
    
    // Clean-up and exit
    board.Wire.end();
    board.detach();
    cache.end();
    return 0;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
