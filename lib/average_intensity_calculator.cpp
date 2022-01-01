#include "average_intensity_calculator.hpp"

#include <numeric>
#include <opencv2/imgproc.hpp>
#include <vector>

void AverageIntensityCalculator::replace_image(const cv::Mat& image)
{
  cv::cvtColor(image, this->images[0], cv::COLOR_RGB2GRAY);
  if (this->images[0].type() != CV_8U or this->images[0].channels() != 1)
  {
    throw std::runtime_error{
        "Something is wrong with `cv::cvtColor` function: `cv::COLOR_RGB2GRAY` "
        "conversion is expected to create a grayscale image with [0; 255] "
        "intensity range."};
  }
}

float AverageIntensityCalculator::average() {
  static_assert(std::numeric_limits<int>::max() > 255);
  static_assert(std::numeric_limits<uchar>::max() == 255);
  static_assert(std::numeric_limits<uchar>::min() == 0);

  if (this->images[0].type() != CV_8U or this->images[0].channels() != 1 or
      this->images[0].empty()) {
    throw std::runtime_error{
        "Only non-empty grayscale images with [0; 255] intensity range are "
        "allowed."};
  }

  static constexpr int histogram_bars{256};
  static constexpr int minimal_intensity{0};

  static const std::vector<int> channels{};
  static const cv::Mat mask{};
  static const std::vector<int> histogram_size{histogram_bars};
  static const std::vector<float> histogram_range{minimal_intensity,
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
