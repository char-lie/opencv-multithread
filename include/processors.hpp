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
