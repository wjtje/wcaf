#pragma once
#include "stdlib.h"

namespace wcaf {
namespace list {

template <typename T>
class List {
 public:
  struct Node {
    T data;
    Node *next;
  };

  struct Iterator {
    Iterator(Node *ptr) : ptr_(ptr) {}

    T operator*() const { return ptr_->data; }
    T *operator->() { return &ptr_->data; }

    Iterator &operator++() {
      ptr_ = ptr_->next;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.ptr_ == b.ptr_;
    }
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.ptr_ != b.ptr_;
    }

   protected:
    Node *ptr_;
  };

  /**
   * @brief Adds a new item to the end of the list.
   *
   * @param data
   */
  void push_back(T data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = nullptr;

    // Update head
    if (this->head_ == nullptr) head_ = node;

    // Update tail
    if (this->tail_ != nullptr) this->tail_->next = node;
    this->tail_ = node;
  }

  /**
   * @brief Get the amount of items stored in the list.
   *
   * @return int
   */
  int size() {
    int size = 0;
    Node *node = this->head_;

    while (node != nullptr) {
      size++;
      node = node->next;
    }

    return size;
  }

  Iterator begin() { return Iterator(this->head_); }
  Iterator end() { return Iterator(nullptr); }

 protected:
  Node *head_ = nullptr;
  Node *tail_ = nullptr;
};

}  // namespace list
}  // namespace wcaf
