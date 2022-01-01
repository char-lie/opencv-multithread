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
