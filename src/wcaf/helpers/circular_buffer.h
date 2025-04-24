#pragma once
#include "stdint.h"
#include "stdlib.h"

namespace wcaf {

/**
 * @brief A basic circular buffer using a static buffer
 *
 * @tparam T The type of the static buffer
 * @tparam SIZE The length of the buffer
 */
template <typename T, uint16_t SIZE>
class CircularBuffer {
 public:
  typedef uint16_t size_type;

  /**
   * @brief Return true when the buffer is full.
   *
   * @return true
   * @return false
   */
  bool full() const { return this->full_; }
  /**
   * @brief Return true when the buffer is empty
   *
   * @return true
   * @return false
   */
  bool empty() const { return (!this->full_ && (this->tail_ == this->head_)); }
  void clear() {
    this->full_ = false;
    this->tail_ = 0;
    this->head_ = 0;
  }
  /**
   * @brief Return the size (capacity) of the buffer.
   *
   * @return size_type
   */
  size_type size() const { return SIZE; }
  /**
   * @brief Return the amount of elements in the buffer, this is between 0 and
   * size.
   *
   * @return size_type
   */
  size_type length() const {
    if (this->full_) return SIZE;
    if (this->tail_ >= this->head_) return this->tail_ - this->head_;
    return SIZE + this->tail_ - this->head_;
  }
  /**
   * @brief Push data to the end of the buffer.
   *
   * @param data[in]
   * @return int Return 0 on success, -1 when out of space.
   */
  int push(const T& data) {
    if (this->full_) return -1;
    this->buffer_[this->tail_] = data;
    this->advance_pointer_();
    return 0;
  }
  /**
   * @brief Push data to the end of the buffer, even if the buffer is full.
   *
   * @param data[in]
   */
  void push_force(const T& data) {
    this->buffer_[this->tail_] = data;
    this->advance_pointer_();
  }
  /**
   * @brief Get the data that is at the front of the buffer
   *
   * @param data[out]
   * @return int Returns 0 on success, -1 when there is no data
   */
  int pop(T* data) {
    if (this->empty()) return -1;
    *data = this->buffer_[this->head_];
    this->retreat_pointer_();
    return 0;
  }
  /**
   * @brief Remove the data this is at the front of the buffer
   *
   * @return int Returns 0 on success, -1 when there is no data.
   */
  int pop() {
    if (this->empty()) return -1;
    this->retreat_pointer_();
    return 0;
  }
  /**
   * @brief Direct pop.
   * Get the data that is at the front of the buffer. Even if that data is
   * invalid.
   *
   * @return const T& A reference to that value
   */
  const T& dpop() {
    const T& d = this->buffer_[this->head_];
    this->retreat_pointer_();
    return d;
  }
  /**
   * @brief Get the data that in the front of the buffer, without removing it.
   *
   * @param data
   * @return int Returns 0 on success, -1 when there is no data
   */
  int peek(T** data) {
    if (this->empty()) return -1;
    *data = &this->buffer_[this->head_];
    return 0;
  }

  /**
   * @brief Get access to the front of the buffer, even if it is invalid
   */
  const T& front() { return this->buffer_[this->head_]; }

  struct Iterator {
    Iterator(size_t position, T* buffer, bool is_tail)
        : position_(position), buffer_(buffer), is_head_(is_tail) {}

    T& operator*() const { return buffer_[position_]; }
    T* operator->() { return &buffer_[position_]; }
    T& operator=(const T& p) { return buffer_[position_] = p; }
    /**
     * @brief Get access to the pointer of the current item of the Iterator.
     * This can be used to update an item that's already in the queue.
     *
     * @return T*
     */
    T* item() { return &buffer_[position_]; }

    Iterator& operator++() {
      if (++position_ == SIZE) position_ = 0;
      is_head_ = false;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.position_ == b.position_ && a.is_head_ == b.is_head_;
    }
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.position_ != b.position_ || a.is_head_ != b.is_head_;
    }

    size_t position_;
    T* buffer_;
    bool is_head_;  // Indicated the tail (begin) of the iterator
  };

  Iterator begin() { return Iterator(this->head_, this->buffer_, true); }
  Iterator end() { return Iterator(this->tail_, this->buffer_, this->empty()); }

 protected:
  T buffer_[SIZE];
  size_t tail_{0}, head_{0};
  bool full_{false};

  void advance_pointer_() {
    if (this->full_)
      if (++(this->head_) == SIZE) this->head_ = 0;
    if (++(this->tail_) == SIZE) this->tail_ = 0;
    this->full_ = (this->tail_ == this->head_);
  }
  void retreat_pointer_() {
    this->full_ = false;
    if (++(this->head_) == SIZE) this->head_ = 0;
  }
};

}  // namespace wcaf
