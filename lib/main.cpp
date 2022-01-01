#include <chrono>
#include <exception>
#include <iostream>
#include <limits>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <thread>

#include "average_intensity_calculator.hpp"
#include "circular_buffer.hpp"
#include "processors.hpp"

int main(int argc, char* argv[]) try {
  if (argc != 2) {
    throw std::runtime_error{
        "You should provide exactly one parameter `threshold`, which is a "
        "non-negative integer"};
  }

  // Function `atoul` returns `unsigned long`.
  // It is a bad idea to use `std::uint64_t` here
  // NOLINTNEXTLINE(google-runtime-int)
  const std::size_t threshold{[](char* string) -> unsigned long {
    try {
      return std::stoul(string);
    } catch (std::invalid_argument&) {
      std::cerr << "Threshold should be a non-negative integer" << std::endl;
      throw;
    } catch (std::out_of_range&) {
      std::cerr << "Threshold should be a non-negative integer that is not "
                   "greater than "
                // Function `atoul` returns `unsigned long`.
                // It is a bad idea to use `std::uint64_t` here
                // NOLINTNEXTLINE(google-runtime-int)
                << std::numeric_limits<unsigned long>::max() << std::endl;
      throw;
    }
  }(argv[1])};

  const std::string queue_thread_window_name{"Thread 1"};
  const std::string clock_thread_window_name{"Thread 2"};

  constexpr std::size_t frames_queue_size{10U};
  CircularBuffer<cv::Mat, frames_queue_size> frames_queue{};
  AverageIntensityCalculator average_intensity_calculator{};

  ThreadHandle queue_thread_handle{queue_thread_window_name};
  ThreadHandle clock_thread_handle{clock_thread_window_name};

  std::thread t1{
      queue_thread<frames_queue_size>,
      std::ref(queue_thread_handle),
      std::ref(frames_queue),
  };
  std::thread t2{
      clock_thread,
      std::ref(clock_thread_handle),
      std::ref(average_intensity_calculator),
      threshold,
  };

  cv::namedWindow(queue_thread_window_name);
  cv::namedWindow(clock_thread_window_name);

  constexpr int default_camera{0};
  cv::VideoCapture cap(default_camera);
  cv::Mat image;

  if (!cap.isOpened()) {
    std::cerr << "Cannot open camera" << std::endl;
    return EXIT_FAILURE;
  }

  std::chrono::time_point<std::chrono::system_clock> previous_frame_push_time{};
  bool is_even{false};
  constexpr int esc_key{27};
  constexpr int byte_mask{0xFF};
  while ((cv::waitKey(1) & byte_mask) != esc_key) {
    cap >> image;

    process_t2(clock_thread_handle, image, previous_frame_push_time);

    if (is_even) {
      process_t1(queue_thread_handle, image, frames_queue);
    }

    is_even = !is_even;
  }

  queue_thread_handle.finish();
  clock_thread_handle.finish();

  t1.join();
  t2.join();

  return EXIT_SUCCESS;
} catch (std::exception& exception) {
  std::cerr << "Unhandled exception: '" << exception.what() << "'" << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "Unexpected exception" << std::endl;
  return EXIT_FAILURE;
}
