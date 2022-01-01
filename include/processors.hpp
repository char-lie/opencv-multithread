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

#ifndef PROCESSORS_HPP
#define PROCESSORS_HPP

#include <cstddef>
#include <opencv2/core/mat.hpp>

#include "average_intensity_calculator.hpp"
#include "circular_buffer.hpp"
#include "thread_handle.hpp"

template <std::size_t buffer_size>
void queue_thread(ThreadHandle &thread_handle,
                  CircularBuffer<cv::Mat, buffer_size> &frames_queue);

void clock_thread(ThreadHandle &thread_handle,
                  AverageIntensityCalculator &average_intensity_calculator,
                  std::size_t threshold);

template <std::size_t buffer_size>
void process_t1(ThreadHandle &queue_thread_handle, cv::Mat &image,
                CircularBuffer<cv::Mat, buffer_size> &frames_queue);

void process_t2(ThreadHandle &clock_thread_handle, cv::Mat &image,
                std::chrono::time_point<std::chrono::system_clock>
                    &previous_frame_push_time);

#include "processors.tpp"

#endif
