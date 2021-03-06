// MIT License
//
// Copyright (c) 2021-2022 char-lie
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cstddef>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

template <std::size_t buffer_size>
void queue_thread(ThreadHandle &thread_handle,
                  CircularBuffer<cv::Mat, buffer_size> &frames_queue) {
  static constexpr std::size_t rotation_period{5U};
  std::size_t image_number{0U};
  while (!thread_handle.need_stop()) {
    std::lock_guard queue_image_guard{thread_handle.image_mutex};
    cv::Mat result;
    {
      std::unique_lock lock{std::move(frames_queue.lock())};
      cv::cvtColor(frames_queue.last(), result, cv::COLOR_RGB2GRAY);
    }
    switch (image_number / rotation_period) {
      case 0:
        break;
      case 1:
        cv::rotate(result, result, cv::ROTATE_90_CLOCKWISE);
        break;
      case 2:
        cv::rotate(result, result, cv::ROTATE_180);
        break;
      case 3:
        cv::rotate(result, result, cv::ROTATE_90_COUNTERCLOCKWISE);
        break;
      case 4:
        image_number = 0U;
        break;
      default:
        // False positive for `assert`
        // NOLINTNEXTLINE(misc-static-assert,hicpp-static-assert,cert-dcl03-c,hicpp-no-array-decay,cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        assert("Check your `switch`. Something is wrong");
    }
    thread_handle.output_image.emplace(std::move(result));
    ++image_number;
    thread_handle.waiting = true;
  }
}

template <std::size_t buffer_size>
void process_t1(ThreadHandle &queue_thread_handle, cv::Mat &image,
                CircularBuffer<cv::Mat, buffer_size> &frames_queue) {
  queue_thread_handle.push_image(
      [&frames_queue, &image]() { frames_queue.push(image); });

  queue_thread_handle.try_show_image();
}
