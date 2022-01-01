#ifndef AVERAGE_INTENSITY_CALCULATOR_HPP
#define AVERAGE_INTENSITY_CALCULATOR_HPP

#include <vector>

#include "opencv2/core/mat.hpp"

class AverageIntensityCalculator {
 private:
  cv::Mat histogram{};
  std::vector<cv::Mat> images{cv::Mat{}};

 public:
  AverageIntensityCalculator() = default;
  AverageIntensityCalculator(const AverageIntensityCalculator&) = delete;
  AverageIntensityCalculator(AverageIntensityCalculator&&) noexcept = default;
  AverageIntensityCalculator& operator=(const AverageIntensityCalculator&) =
      delete;
  AverageIntensityCalculator& operator=(AverageIntensityCalculator&&) noexcept =
      default;

  void replace_image(const cv::Mat& image);
  float average();

  ~AverageIntensityCalculator() = default;
};

#endif
