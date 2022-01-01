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

#include "thread_handle.hpp"

#include <concepts>
#include <opencv2/highgui.hpp>
#include <string>

ThreadHandle::ThreadHandle(std::string cv_window_name)
    : window_name{std::move(cv_window_name)} {}

bool ThreadHandle::need_stop() {
  std::unique_lock<std::mutex> lock{this->mutex};

  while (this->waiting and !this->is_finished) {
    this->condition_variable.wait(
        lock, [this] { return !this->waiting or this->is_finished; });
  }

  return this->is_finished;
}

bool ThreadHandle::try_show_image() {
  std::lock_guard queue_image_guard{this->image_mutex};
  if (this->output_image.has_value()) {
    cv::imshow(this->window_name, *this->output_image);
    this->output_image.reset();
    return true;
  }
  return false;
}

void ThreadHandle::finish() {
  std::lock_guard<std::mutex> lock{this->mutex};
  this->is_finished = true;
  this->condition_variable.notify_one();
}
