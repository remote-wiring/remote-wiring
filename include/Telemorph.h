/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef TELEMORPH_H
#define TELEMORPH_H

#include <chrono>
#include <future>

#include "RwConstants.h"
#include "RwTypes.h"
#include "RwUtility.h"
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
     * \param [in] uponComplete_ A function pointer to a callback to be fired
     *                           once the attach operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     * \warning Ensure the underlying stream is available before invoking
     */
    inline
    void
    attach (
        signal_t uponComplete_ = nullptr,
        void * context_ = nullptr
    ) {
        utility::invoke_blockable_async_method(*this, &Telemorph::_attach, uponComplete_, context_);
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
        int error;
        if ( 0 != (error = _detach()) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: Underlying implementation encoutered error!");
#endif
        }
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
     * \param [in] uponComplete_ A function pointer to a callback to be fired
     *                           once the refresh operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     */
    inline
    void
    refresh (
        signal_t uponComplete_ = nullptr,
        void * context_ = nullptr
    ) {
        utility::invoke_blockable_async_method(*this, &Telemorph::_refresh, uponComplete_, context_);
    }

    /*!
     * \brief Reset the target device
     *
     * Reset the connection details, parsing cache and the state of
     * the remote device.
     *
     * \param [in] uponComplete_ A function pointer to a callback to be fired
     *                           once the survey operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     */
    inline
    void
    reset (
        signal_t uponComplete_ = nullptr,
        void * context_ = nullptr
    ) {
        utility::invoke_blockable_async_method(*this, &Telemorph::_reset, uponComplete_, context_);
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
     * \note The default interval for the Arduino implementation is 19ms and
     *       the minimum sampling interval is 1ms.
     */
    inline
    void
    samplingInterval (
        size_t interval_ms_
    ) {
        int error = 0;
        if ( 1 > interval_ms_ ) {
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: Specified interval is too small!");
#endif
        } else if ( 0 != (error = _samplingInterval(interval_ms_)) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: Underlying implementation encoutered error!");
#endif
        }
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
     * \param [in] uponComplete_ A function pointer to a callback to be fired
     *                           once the survey operation has completed
     * \param [in] context_ A context to be passed to the callback
     *
     * \note The signature of the signalling callback is `void(*)(void *)`
     * \warning Some features of the remote device may not function without
     *          first surveying the device.
     */
    inline
    void
    survey (
        signal_t uponComplete_ = nullptr,
        void * context_ = nullptr
    ) {
        utility::invoke_blockable_async_method(*this, &Telemorph::_survey, uponComplete_, context_);
    }

    /*!
     * \brief Firmware version running on the remote device
     *
     * \return A pointer to structure containing Semantic Versioning details
     *
     * \note Only available after a survey has occurred
     *
     * \sa remote_wiring::Telemorph::survey
     */
    inline
    const SemVer *
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
        signal_t uponComplete_,
        void * context_
    ) = 0;

    virtual
    int
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
        signal_t uponComplete_,
        void * context_
    ) = 0;

    virtual
    int
    _reset (
        signal_t uponComplete_,
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
        signal_t uponComplete_,
        void * context_
    ) = 0;

    virtual
    const SemVer *
    _version (
        void
    ) = 0;
};

} // namespace remote_wiring

#endif // TELEMORPH_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
