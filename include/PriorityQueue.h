#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <functional>
#include "Patient.h"

// Node structure với raw pointer
struct Node {
    Patient data;
    Node* next;
    
    Node(const Patient& patient) : data(patient), next(nullptr) {}
};

class PriorityQueue {
private:
    Node* head;
    Node* tail;
    size_t size;
    
    // Hàm so sánh ưu tiên
    bool hasHigherPriority(const Patient& a, const Patient& b) const;
    
public:
    PriorityQueue();
    ~PriorityQueue();
    
    // Các thao tác cơ bản
    void push(const Patient& patient);
    Patient pop();
    bool empty() const;
    size_t getSize() const;
    
    // Hiển thị
    void printAll() const;
};

#endif // PRIORITY_QUEUE_H 