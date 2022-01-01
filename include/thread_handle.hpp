#ifndef THREAD_HANDLE_HPP
#define THREAD_HANDLE_HPP

#include <concepts>
#include <condition_variable>
#include <mutex>
#include <opencv2/core/mat.hpp>
#include <optional>
#include <string>

struct ThreadHandle {
  std::optional<cv::Mat> output_image{};
  std::mutex mutex{};
  std::mutex image_mutex{};
  std::condition_variable condition_variable{};
  std::string window_name;
  bool is_finished{false};
  bool waiting{true};

  explicit ThreadHandle(std::string cv_window_name);

  bool need_stop();
  bool try_show_image();
  void finish();
  template <typename F>
  requires std::invocable<F> void push_image(F pusher);
};

template <typename F>
requires std::invocable<F> void ThreadHandle::push_image(F pusher) {
  std::lock_guard lock{this->image_mutex};
  pusher();
  this->waiting = false;
  this->condition_variable.notify_one();
}

#endif
