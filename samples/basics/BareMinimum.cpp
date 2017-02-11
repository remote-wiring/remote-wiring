/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include <chrono>
#include <iostream>
#include <thread>

#include <remote_wiring>
#include <serial_wiring>

#define delay(ms_) std::this_thread::sleep_for(std::chrono::milliseconds(ms_))

using namespace remote_wiring::boards::arduino::uno;
using namespace remote_wiring::wiring;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 //%% Aduino Sketch Environment %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void setup(remote_wiring::RemoteDevice & uno) {
  // put your setup code here, to run once:

}

void loop(remote_wiring::RemoteDevice & uno) {
  // put your main code here, to run repeatedly:

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::atomic_bool run;

void sketch_thread(remote_wiring::RemoteDevice & uno) {
  setup(uno);
  for (run = true ; run ; ) { loop(uno); }
}

int main (int argc, char * argv []) {
  if ( argc < 2 ) { std::cout << "Usage: " << argv[0] << " <serial device descriptor>" << std::endl; return -1; }

  serial_wiring::UartSerial usb(argv[1]);
  remote_wiring::FirmataDevice uno(usb);

  usb.begin(57600);
  uno.attach();

  // Launch Arduino sketch environment
  std::thread sketch(sketch_thread, std::ref(uno));

  // Prompt for exit
  std::cout << std::endl << "Processing sketch on remote device." << std::endl;
  if ( uno.firmware() ) {
    std::cout << "(detected firmware as: " << uno.firmware() << " - v" << uno.version().sv_major << "." << uno.version().sv_minor << ")" << std::endl;
  } else {
    std::cout << "(unable to detect firmware)" << std::endl;
  }

  std::cout << std::endl << "Press any key to exit sketch..." << std::endl;
  ::getchar();
  run = false;
  sketch.join();
  std::cout << "Sketch environment has exited." << std::endl;

  uno.detach();
  usb.end();
  return 0;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
