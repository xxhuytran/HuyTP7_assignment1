#pragma once
#include <cstddef>
#include <memory>
#include <utility>

#include "Errors.h"

namespace sdp {
namespace ds {

// Queue danh sach lien ket don, node dung unique_ptr
template <typename T>
class LinkedQueue {
public:
    LinkedQueue() = default;
    LinkedQueue(const LinkedQueue&) = delete;
    LinkedQueue& operator=(const LinkedQueue&) = delete;

    // Huy node bang vong lap de tranh de quy sau khi co 100000 node
    ~LinkedQueue() {
        while (head_) {
            std::unique_ptr<Node> next = std::move(head_->next);
            head_ = std::move(next);
        }
    }

    void push(T value) {
        std::unique_ptr<Node> node = std::make_unique<Node>(std::move(value));
        Node* raw = node.get();
        if (tail_ == nullptr) {
            head_ = std::move(node);
        } else {
            tail_->next = std::move(node);
        }
        tail_ = raw;  // con tro khong so huu
        ++size_;
    }

    T pop() {
        if (head_ == nullptr) {
            throw EmptyContainerError("LinkedQueue::pop on empty queue");
        }
        T value = std::move(head_->value);
        std::unique_ptr<Node> next = std::move(head_->next);
        head_ = std::move(next);
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
        --size_;
        return value;
    }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }

private:
    struct Node {
        explicit Node(T v) : value(std::move(v)) {}
        T value;
        std::unique_ptr<Node> next;
    };

    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
};

}  // namespace ds
}  // namespace sdp
