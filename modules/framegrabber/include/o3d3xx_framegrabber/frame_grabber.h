// -*- c++ -*-
/*
 * Copyright (C) 2014 Love Park Robotics, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distribted on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __O3D3XX_FRAME_GRABBER_H__
#define __O3D3XX_FRAME_GRABBER_H__

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include "o3d3xx_framegrabber/pcic_schema.h"
#include "o3d3xx_framegrabber/byte_buffer.hpp"
#include "o3d3xx_camera/camera.hpp"

namespace o3d3xx
{
  extern const std::size_t TICKET_ID_SZ;
  extern const std::string TICKET_image;
  extern const std::string TICKET_c;
  extern const std::string TICKET_t;

  /**
   * The FrameGrabber is a class that, when given access to an
   * o3d3xx::Camera::Ptr, it will grab frames from the camera in a separate
   * thread of execution.
   */
  class FrameGrabber
  {
  public:
    using Ptr = std::shared_ptr<FrameGrabber>;

    /**
     * Stores reference to the passed in camera and starts the worker thread
     *
     * @param[in] cam The camera instance to grab frames from
     * @param[in] mask A bitmask encoding the PCIC schema to use
     */
    FrameGrabber(o3d3xx::Camera::Ptr cam,
                 std::uint16_t mask = o3d3xx::DEFAULT_SCHEMA_MASK);

    /**
     * Cleans up any resources held by the frame grabbing thread object and
     * blocks until the operating system thread stops.
     */
    virtual ~FrameGrabber();

    // copy and move semantics
    FrameGrabber(FrameGrabber&&) = delete;
    FrameGrabber& operator=(FrameGrabber&&) = delete;
    FrameGrabber(FrameGrabber&) = delete;
    FrameGrabber& operator=(const FrameGrabber&) = delete;

    /**
     * Interrupts the running thread by throwing an o3d3xx::error_t with code
     * O3D3XX_THREAD_INTERRUPTED.
     *
     * While this is (currently) part of the public interface, clients should
     * be aware that there is really no way to restart a stopped FrameGrabber
     * instance. To do that, you would need to instantiate a new FrameGrabber.
     */
    void Stop();

    /**
     * Triggers the camera for an image acquisition.
     *
     * You should be sure to set the `TriggerMode` for your application to
     * `PROCESS_INTERFACE` in order for this to be effective. This function
     * simply does the triggering, data are still received asynchronously via
     * `WaitForFrame()`.
     */
    void SWTrigger();

    /**
     * This function is used to grab and parse out time synchronized image data
     * from the camera. It will call `SetBytes' on the passed in `ByteBuffer'
     * as well as (optionally, but by default) call `Organize'. Calling
     * `Organize' is the default behavior so the `buff' output parameter is
     * assumed to be synchronized and ready for analysis provided this function
     * returns true. In certain applications, it may be a performance
     * enhancement to not call `Organize' but rather handle that outside of the
     * `FrameGrabber'.
     *
     * @param[out] buff A pointer to an o3d3xx::ByteBuffer object to update
     * with the latest data from the camera.
     *
     * @param[in] timeout_millis Amount of time, in milliseconds, to wait for
     * new image data from the FrameGrabber. If `timeout_millis' is set to 0,
     * this function will block indefinitely.
     *
     * @param[in] copy_buff Flag indicating whether the frame grabber's
     * internal `front_buffer_' should be copied (O(n)) or swapped (O(1)) with
     * the raw `bytes_' of the passed in `buff'. You should only flag this as
     * `true' if you are planning to use multiple clients with a single
     * FrameGrabber.
     *
     * @parma[in] organize Flag indicating whether or not `Organize' should be
     * called on the byte buffer before returning.
     *
     * @return true if a new buffer was acquired within `timeout_millis', false
     * otherwise.
     */
    bool WaitForFrame(o3d3xx::ByteBuffer* buff,
                      long timeout_millis = 0,
                      bool copy_buff = false,
                      bool organize = true);

  protected:
    /**
     * This is the thread worker function -- its execution occurs in a separate
     * operating system thread. This thread will run its own ASIO event loop
     * continually streaming in images from the camera independent of other
     * control threads within the driver.
     */
    virtual void Run();

    /**
     * Sets the PCIC 'c' command schema buffer to send to the camera.
     */
    void SetSchemaBuffer(std::uint16_t mask);

    /**
     * Sets the PCIC 't' buffer
     */
    void SetTriggerBuffer();

    /**
     * Callback function for reading ticket data from the camera.
     * This function is generalized to handle reading (as of now) all return
     * tickets that we expect to see.
     */
    void TicketHandler(const boost::system::error_code& ec,
                       std::size_t bytes_transferred,
                       std::size_t bytes_read);

    /**
     * Callback function for reading image data from the camera.
     */
    void ImageHandler(const boost::system::error_code& ec,
                      std::size_t bytes_transferred,
                      std::size_t bytes_read);

  private:
    /**
     * Shared pointer to the camera this frame grabber will grab frames from.
     */
    o3d3xx::Camera::Ptr cam_;

    /**
     * Cached copy of the camera IP address
     */
    std::string cam_ip_;

    /**
     * Cached copy of the camera PCIC TCP port
     */
    int cam_port_;

    /**
     * The ASIO event loop handle
     */
    boost::asio::io_service io_service_;

    /**
     * The ASIO socket to PCIC
     */
    boost::asio::ip::tcp::socket sock_;

    /**
     * The ASIO endpoint to PCIC
     */
    boost::asio::ip::tcp::endpoint endpoint_;

    /**
     * A pointer to the wrapped thread object. This is the thread that
     * communicates directly with the sensor.
     */
    std::unique_ptr<std::thread> thread_;

    /**
     * Flag indicating that the PCIC daemon is ready
     * (we define "ready" to mean that our schema has been uploaded)
     */
    std::atomic<bool> pcic_ready_;

    /**
     * The user-supplied schema mask to stream from the camera.
     */
    std::uint16_t mask_;

    /**
     * Holds the PCIC command needed to set the result schema
     * for our open socket connection.
     */
    std::vector<std::uint8_t> schema_buffer_;

    /**
     * Holds the PCIC command needed to s/w trigger an image acquisition.
     */
    std::vector<std::uint8_t> trigger_buffer_;

    /**
     * Holds the raw 'Ticket' bytes received from the sensor.
     *
     * PCIC V3 (16 bytes total):
     * 4 bytes ticket number
     * 'L' + 9 bytes length + CR + LF
     *
     * The '9 bytes length' is a string of chars to be converted to an integer
     * number representing the length of the payload data.
     */
    std::vector<std::uint8_t> ticket_buffer_;

    /**
     * The FrameGrabber double buffers images when acquiring from the
     * camera. This implements the _back buffer_ in the double buffering
     * scheme.
     *
     * According to the PCIC V3 protocol, this will hold from the four digit
     * ticket sequence up to and including the ending CR + LF ('\r\n').
     */
    std::vector<std::uint8_t> back_buffer_;

    /**
     * The buffer that subscribed clients fetch data from.
     */
    std::vector<std::uint8_t> front_buffer_;

    /**
     * Protects front_buffer_
     */
    std::mutex front_buffer_mutex_;

    /**
     * Condition variable used to notify clients when image data are ready
     */
    std::condition_variable front_buffer_cv_;

  }; // end: class FrameGrabber

} // end: namespace o3d3xx

#endif // __O3D3XX_FRAME_GRABBER_H__
