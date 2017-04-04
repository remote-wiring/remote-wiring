/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef TELEMORPH_H
#define TELEMORPH_H

#include <chrono>
#include <future>

#include "RwConstants.h"
#include "RwTypes.h"
#include "SemanticVersion.h"

namespace remote_wiring {

/*!
 * \brief An interface to define the interaction model with a remote device
 *
 * Telemorph, is derived from the greek prefix “tele-” (meaning
 * remote) and the suffix “-morph” (meaning form or body). It is
 * a virtual representation of physical hardware, used to define
 * the capabilities and interaction model of the remote hardware.
 */
class Telemorph {
  public:
    /*!
     * \brief Attach the virtual representation to the remote device
     *
     * \param [in] uponAttach_ A function pointer to a callback to be fired
     *                         once the attach operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \return An error code for the attach request. A zero indicates no
     *         error occurred, while a non-zero value indicates an error
     *         occurred while sending the request.
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     * \warning Ensure the underlying stream is available before invoking
     */
    inline
    int
    attach (
        signal_t uponAttach_ = nullptr,
        void * context_ = nullptr
    ) {
        return invoke_blockable_async_method(&Telemorph::_attach, uponAttach_, context_);
    }

    /*!
     * \brief Detach the virtual representation from the remote device
     *
     * Detach will close the connection to the remote device and free
     * any resources required to establish the connection.
     */
    inline
    void
    detach (
        void
    ) {
        _detach();
    }

    /*!
     * \brief Firmware running on the remote device
     *
     * \return A string describing the firmware
     *
     * \note Only available after a survey has occurred
     *
     * \sa remote_wiring::Telemorph::survey
     */
    inline
    const char *
    firmware (
        void
    ) {
        return _firmware();
    }

    /*!
     * \brief Updates local cache based on the current state of the remote device
     *
     * Refresh provides a mechanism for the virtual representation
     * to synchronize its state by querying the current values of the
     * remote device.
     *
     * \param [in] uponRefresh_ A function pointer to a callback to be fired
     *                          once the refresh operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \return An error code for the refresh request. A zero indicates no
     *         error occurred, while a non-zero value indicates an error
     *         occurred while sending the request.
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     */
    inline
    int
    refresh (
        signal_t uponRefresh_ = nullptr,
        void * context_ = nullptr
    ) {
        return invoke_blockable_async_method(&Telemorph::_refresh, uponRefresh_, context_);
    }

    /*!
     * \brief Reset the target device
     *
     * Reset the connection details, parsing cache and the state of
     * the remote device.
     *
     * \param [in] uponReset_ A function pointer to a callback to be fired
     *                        once the survey operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \return An error code for the reset request. A zero indicates no
     *         error occurred, while a non-zero value indicates an error
     *         occurred while sending the request.
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     */
    inline
    int
    reset (
        signal_t uponReset_ = nullptr,
        void * context_ = nullptr
    ) {
        return invoke_blockable_async_method(&Telemorph::_reset, uponReset_, context_);
    }

    /*!
     * \brief Adjust the sampling interval
     *
     * The sampling interval is the delay between readings on the remote
     * device. For example, if you needed to read an analog value every 5ms,
     * but the sampling interval is set to 19ms (the default), then you will
     * see the same value 3 times before a change can occur. However, if you
     * were to set the sampling interval to 5 or less milliseconds, then you
     * would be ensured a new value each time.
     *
     * \param [in] interval_ms_ The number of milliseconds to pass between
     *                          read and report operations on the remote device
     *
     * \return An error code for the set sampling interval request. A zero
     *         indicates no error occurred, while a non-zero value indicates
     *         an error occurred while sending the request.
     *
     * \note The default interval for the arduino implementation is 19ms and
     *       the minimum sampling interval is 1ms.
     */
    inline
    int
    samplingInterval (
        size_t interval_ms_
    ) {
        return _samplingInterval(interval_ms_);
    }

    /*!
     * \brief Examine the features and capabilities of the remote device
     *
     * Survey queries the capabilities of the board, which bounds the
     * previously unbounded representation of the remote device. This
     * allows for optimization of the serial traffic generated by the
     * telemorph. Survey may also provide additional mapping necessary
     * to ensure certain features of the remote device function properly.
     *
     * \param [in] uponSurvey_ A function pointer to a callback to be fired
     *                         once the survey operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \return An error code for the survey request. A zero indicates no
     *         error occurred, while a non-zero value indicates an error
     *         occurred while sending the request.
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     * \warning Some features of the remote device may not function without
     *          first surveying the device.
     */
    inline
    int
    survey (
        signal_t uponSurvey_ = nullptr,
        void * context_ = nullptr
    ) {
        return invoke_blockable_async_method(&Telemorph::_survey, uponSurvey_, context_);
    }

    /*!
     * \brief Firmware version running on the remote device
     *
     * \return A structure containing Semantic Versioning details
     *
     * \note Only available after a survey has occurred
     *
     * \sa remote_wiring::Telemorph::survey
     */
    inline
    SemVer
    version (
        void
    ) {
        return _version();
    }

  protected:
    ~Telemorph (void) {}

    virtual
    int
    _attach (
        signal_t uponAttach_,
        void * context_
    ) = 0;

    virtual
    void
    _detach (
        void
    ) = 0;

    virtual
    const char *
    _firmware (
        void
    ) = 0;

    virtual
    int
    _refresh (
        signal_t uponRefresh_,
        void * context_
    ) = 0;

    virtual
    int
    _reset (
        signal_t uponReset_,
        void * context_
    ) = 0;

    virtual
    int
    _samplingInterval (
        size_t interval_ms_
    ) = 0;

    virtual
    int
    _survey (
        signal_t uponSurvey_,
        void * context_
    ) = 0;

    virtual
    SemVer
    _version (
        void
    ) = 0;

  private:
    typedef int(Telemorph::*blockable_async_method_t)(signal_t uponComplete_, void * context_);

    int
    invoke_blockable_async_method (
        blockable_async_method_t method_,
        signal_t methodComplete_,
        void * context_
    ) {
        int error;
        bool block = false;
        std::promise<void> callback_signal;
        std::future<void> callback_gate(callback_signal.get_future());

        if ( !methodComplete_ ) {
            block = true;
            methodComplete_ = synchronous_callback;
            context_ = static_cast<void *>(&callback_signal);
        }

        error = (this->*method_)(methodComplete_, context_);
        if ( !error && block ) {
            switch (callback_gate.wait_for(std::chrono::seconds(REMOTE_DEVICE_TIMEOUT_S)))
            {
              case std::future_status::ready:
                break;
              case std::future_status::deferred:
              case std::future_status::timeout:
              default:
                error = __LINE__;
                break;
            }
        }

        return error;
    }

    static
    void
    synchronous_callback (
        void * context_
    ) {
        static_cast<std::promise<void> *>(context_)->set_value();
    }
};

} // namespace remote_wiring

#endif // TELEMORPH_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
