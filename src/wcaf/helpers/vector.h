#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace wcaf {

template <class T>
class Vector {
 public:
  typedef uint16_t size_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *iterator;
  typedef const T *const_iterator;

  Vector() {}
  ~Vector() {
    if (data_) free(data_);
  }

  // MARK: Element access

  reference at(size_type pos) { return data_[pos]; }
  const_reference at(size_type pos) const { return data_[pos]; }

  reference operator[](size_type pos) { return data_[pos]; }
  const_reference operator[](size_type pos) const { return data_[pos]; }

  reference front() { return data_[0]; }
  const_reference front() const { return data_[0]; }

  reference back() { return data_[size_ - 1]; }
  const_reference back() const { return data_[size_ - 1]; }

  T *data() { return data_; }
  const T *data() const { return data_; }

  // MARK: Iterators

  iterator begin() { return &data_[0]; }
  const_iterator begin() const { return &data_[0]; }

  iterator end() { return &data_[size_]; }
  const_iterator end() const { return &data_[size_]; }

  // MARK: Capacity

  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }

  void reserve(size_type new_cap) {
    if (new_cap <= capacity_) return;

    T *new_data = static_cast<T *>(realloc(data_, new_cap * sizeof(T)));
    if (new_data == nullptr) return;  // Realloc failed

    data_ = new_data;
    capacity_ = new_cap;
  }

  size_type capacity() const { return capacity_; }

  // MARK: Modifiers

  iterator erase(iterator it) {
    if (it == (end() - 1)) {
      pop_back();
      return it;
    }

    iterator from = it + 1;
    iterator destination = it;
    for (; from != end(); ++from, ++destination) *destination = *from;

    --size_;
    return it;
  }

  void erase(const_reference d) {
    for (iterator it = begin(); it != end();) {
      if ((*it) == d) {
        it = erase(it);
      } else {
        ++it;
      }
    }
  }

  void push_back(const_reference d) {
    if (size_ == capacity_) reserve(capacity_ + 1);

    data_[size_] = d;
    ++size_;
  }

  void pop_back() {
    if (size_ == 0) return;

    data_[size_ - 1] = T(NULL);
    --size_;
  }

 protected:
  /// @brief The internal data storage
  T *data_{nullptr};
  /// @brief The maximum number of element the storage can store
  size_type capacity_{0};
  /// @brief The current amount of elements in the storage
  size_type size_{0};
};

}  // namespace wcaf
