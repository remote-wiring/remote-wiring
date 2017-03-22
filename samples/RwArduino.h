/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 //%% Aduino Sketch Environment %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <chrono>
#include <iostream>
#include <thread>

#include <remote_wiring>
#include <serial_wiring>

void loop(remote_wiring::RemoteDevice & board_);
void setup(remote_wiring::RemoteDevice & board_);

namespace remote_wiring {
namespace wiring {

inline
void
delay (
    size_t ms_
) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms_));
}

inline
void
delayMicroseconds (
    size_t us_
) {
    std::this_thread::sleep_for(std::chrono::microseconds(us_));
}

}  // namespace wiring
}  // namespace remote_wiring

using namespace remote_wiring::wiring;

std::atomic_bool run;

namespace {
    void sketch_thread(remote_wiring::RemoteDevice & board_) {
      setup(board_);
      for (run = true ; run ; ) { loop(board_); }
    }
}

int main (int argc, char * argv []) {
    std::cout << "************************************************" << std::endl;
    std::cout << "** The \"Examples > Firmata > StandardFirmata\" **" << std::endl;
    std::cout << "** sketch must be deployed to the Arduino in  **" << std::endl;
    std::cout << "** order for the sample to work correctly.    **" << std::endl;
    std::cout << "************************************************" << std::endl;

    if ( argc < 2 ) { std::cout << "Usage: " << argv[0] << " <serial device descriptor>" << std::endl; return -1; }

    serial_wiring::UartSerial usb(argv[1]);
    remote_wiring::FirmataDevice board(usb);

    // Establish a communication channel
    usb.begin(57600);

    // Attach to the remote device
    board.attach();

    // Survey the board's capabilities
    board.survey();

    // Launch Arduino sketch environment
    std::thread sketch(sketch_thread, std::ref(board));

    // Prompt for exit
    std::cout << std::endl << "Processing sketch on remote device." << std::endl;
    if ( board.firmware() ) {
        std::cout << "(detected firmware as: " << board.firmware() << " - v" << board.version().sv_major << "." << board.version().sv_minor << ")" << std::endl;
    } else {
        std::cout << "(unable to detect firmware)" << std::endl;
    }

    // Await user input
    std::cout << std::endl << "Press any key to exit sketch..." << std::endl;
    ::getchar();
    run = false;
    sketch.join();
    std::cout << "Sketch environment has exited." << std::endl;

    // Clean-up and exit
    board.detach();
    usb.end();
    return 0;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
