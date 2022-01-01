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

#include "average_intensity_calculator.hpp"

#include <numeric>
#include <opencv2/imgproc.hpp>
#include <vector>

void AverageIntensityCalculator::replace_image(const cv::Mat& image) {
  cv::cvtColor(image, this->images[0], cv::COLOR_RGB2GRAY);
  if (this->images[0].type() != CV_8U or this->images[0].channels() != 1) {
    throw std::runtime_error{
        "Something is wrong with `cv::cvtColor` function: `cv::COLOR_RGB2GRAY` "
        "conversion is expected to create a grayscale image with [0; 255] "
        "intensity range."};
  }
}

float AverageIntensityCalculator::average() {
  static constexpr int min_intensity{0};
  static constexpr int max_intensity{255};
  static constexpr int histogram_bars{max_intensity + 1};
  static_assert(std::numeric_limits<int>::max() > max_intensity);
  static_assert(std::numeric_limits<uchar>::max() == max_intensity);
  static_assert(std::numeric_limits<uchar>::min() == min_intensity);

  if (this->images[0].type() != CV_8U or this->images[0].channels() != 1 or
      this->images[0].empty()) {
    throw std::runtime_error{
        "Only non-empty grayscale images with [0; 255] intensity range are "
        "allowed."};
  }

  static const std::vector<int> channels{};
  static const cv::Mat mask{};
  static const std::vector<int> histogram_size{histogram_bars};
  static const std::vector<float> histogram_range{min_intensity,
                                                  histogram_bars};

  cv::calcHist(this->images, channels, mask, this->histogram, histogram_size,
               histogram_range);
  this->histogram /= this->images[0].cols * this->images[0].rows;

  float current_intensity{0.0F};
  float average_color{std::accumulate(
      histogram.begin<float>(), histogram.end<float>(), 0.0F,
      [&current_intensity](float accumulator, float value) -> float {
        return (current_intensity++) * value + accumulator;
      })};

  return average_color;
}
