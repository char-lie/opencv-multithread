#include "processors.hpp"

#include <cassert>
#include <cstddef>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "average_intensity_calculator.hpp"
#include "circular_buffer.hpp"
#include "thread_handle.hpp"

void clock_thread(ThreadHandle &thread_handle,
                  AverageIntensityCalculator &average_intensity_calculator,
                  const std::size_t threshold) {
  static constexpr float half{0.5F};
  cv::Mat flipped;
  while (!thread_handle.need_stop()) {
    average_intensity_calculator.replace_image(*thread_handle.output_image);
    if (average_intensity_calculator.average() >
        static_cast<float>(threshold) * half) {
      cv::flip(*thread_handle.output_image, flipped, 1);
      thread_handle.output_image.emplace(flipped);
    }
    thread_handle.waiting = true;
  }
}

void process_t2(ThreadHandle &clock_thread_handle, cv::Mat &image,
                std::chrono::time_point<std::chrono::system_clock>
                    &previous_frame_push_time) {
  static constexpr std::chrono::seconds image_timeout{1};
  if (clock_thread_handle.waiting and
      // Looks like false positive:
      // linter proposes replacing `>=` by `nullptr`
      // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
      std::chrono::system_clock::now() - previous_frame_push_time >=
          image_timeout) {
    clock_thread_handle.push_image(
        [&clock_thread_handle, &previous_frame_push_time, &image]() {
          clock_thread_handle.output_image.emplace(image);
          previous_frame_push_time = std::chrono::system_clock::now();
        });
  }

  if (clock_thread_handle.waiting) {
    clock_thread_handle.try_show_image();
  }
}
