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

#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <array>
#include <concepts>
#include <cstddef>
#include <mutex>

template <std::default_initializable T, std::size_t size>
class CircularBuffer {
  std::mutex mutex{};
  std::array<T, size> data{};
  typename std::array<T, size>::iterator iterator{data.begin()};

 public:
  CircularBuffer() = default;
  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer(CircularBuffer&&) noexcept = default;
  CircularBuffer& operator=(const CircularBuffer&) = delete;
  CircularBuffer& operator=(CircularBuffer&&) noexcept = default;

  constexpr void push(const T& t);
  constexpr const T& last() const noexcept;
  std::unique_lock<std::mutex> lock();

  ~CircularBuffer() = default;
};

template <std::default_initializable T, std::size_t size>
constexpr void CircularBuffer<T, size>::push(const T& t) {
  static_assert(size > 0);
  std::lock_guard lock{this->mutex};
  const auto next_iterator{std::next(this->iterator)};
  this->iterator =
      next_iterator == this->data.end() ? this->data.begin() : next_iterator;
  *this->iterator = t;
}

template <std::default_initializable T, std::size_t size>
constexpr const T& CircularBuffer<T, size>::last() const noexcept {
  static_assert(size > 0);
  return *this->iterator;
}

template <std::default_initializable T, std::size_t size>
std::unique_lock<std::mutex> CircularBuffer<T, size>::lock() {
  return std::unique_lock<std::mutex>{this->mutex};
}

#endif
