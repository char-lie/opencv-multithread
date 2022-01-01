#include "thread_handle.hpp"

#include <concepts>
#include <opencv2/highgui.hpp>
#include <string>

ThreadHandle::ThreadHandle(std::string cv_window_name)
    : window_name{std::move(cv_window_name)} {}

bool ThreadHandle::need_stop() {
  std::unique_lock<std::mutex> lock{this->mutex};

  while (this->waiting and !this->is_finished) {
    this->condition_variable.wait(lock, [this] {
      return !this->waiting or this->is_finished;
    });
  }

  return this->is_finished;
}

bool ThreadHandle::try_show_image()
{
  std::lock_guard queue_image_guard{this->image_mutex};
  if (this->output_image.has_value()) {
    cv::imshow(this->window_name, *this->output_image);
    this->output_image.reset();
    return true;
  }
  return false;
}

void ThreadHandle::finish()
{
    std::lock_guard<std::mutex> lock{this->mutex};
    this->is_finished = true;
    this->condition_variable.notify_one();
}
