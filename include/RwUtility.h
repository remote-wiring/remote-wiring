/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */ 

#include <cerrno>
#include <cstdio>
#include <future>
#include <thread>

#include "RwConstants.h"
#include "RwTypes.h"

namespace remote_wiring {
namespace utility {

inline static
void
synchronizing_callback (
    void * context_
) {
    /*
     * `std::promise` is honored on a separate thread
     * to ensure underlying implementation is able to
     * invoke the callback on the originating thread
     */
    (void)std::thread(
        [&](void) {
            reinterpret_cast<std::promise<void> *>(context_)->set_value();
        }
    ).detach();
}

template <class T>
inline static
void
invoke_blockable_async_callback (
    T * object_,
    int(T::*method_)(signal_t uponComplete_, void * context_),
    signal_t uponComplete_,
    void * context_
) {
    int error;
    bool block = false;
    std::promise<void> callback_signal;
    std::future<void> callback_gate(callback_signal.get_future());

    if ( !uponComplete_ ) {
        block = true;
        uponComplete_ = synchronizing_callback;
        context_ = reinterpret_cast<void *>(&callback_signal);
    }

    if ( 0 != (error = (object_->*method_)(uponComplete_, context_)) ) {
        errno = error;
#ifdef LOG_ERROR
        ::perror("ERROR: Underlying implementation encountered error!");
#endif
    } else if ( block ) {
        switch (callback_gate.wait_for(std::chrono::milliseconds(REMOTE_DEVICE_TIMEOUT_MS))) {
          case std::future_status::ready:
            break;
          case std::future_status::timeout:
            errno = ETIME;
#ifdef LOG_ERROR
            ::perror("ERROR: Block on future timed out!");
#endif
            break;
          case std::future_status::deferred:
          default:
            // This case is unexpected and NOT covered by unit tests
            errno = EWOULDBLOCK;
#ifdef LOG_ERROR
            ::perror("ERROR: Block on future returned unhandled result!");
#endif
            break;
        }
    }
}

}  // utility
}  // remote_wiring

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */ 
