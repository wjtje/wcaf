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
      prev_ptr_ = ptr_;
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

    Node *prev_ptr_;
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
   * @brief Adds a new item to the front of the list
   *
   * @param data
   */
  void push_front(T data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = this->head_;

    this->head_ = node;

    if (this->tail_ == nullptr) this->tail_ = node;
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

  Iterator erase(Iterator it) {
    Node *prev = it.prev_ptr_;
    Node *current = it.ptr_;
    Node *next = current->next;

    // Move next point to the correct location
    if (prev->next != current)
      this->head_ = next;
    else
      prev->next = next;

    // Update tail if needed
    if (prev->next == nullptr) this->tail_ = prev;
    if (this->tail_ == current) this->tail_ = next;

    // Free up memory
    delete current;

    return Iterator(next);
  }

  Iterator erase(T data) {
    for (List<T>::Iterator it = this->begin(); it != this->end();) {
      if (data == *it) return this->erase(it);
    }

    return this->end();
  }

 protected:
  Node *head_ = nullptr;
  Node *tail_ = nullptr;
};

}  // namespace list
}  // namespace wcaf
