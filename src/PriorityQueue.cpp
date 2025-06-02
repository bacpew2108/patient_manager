#include "../include/PriorityQueue.h"
#include <iostream>
#include <iomanip>

PriorityQueue::PriorityQueue() : head(nullptr), tail(nullptr), size(0) {}

PriorityQueue::~PriorityQueue() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

bool PriorityQueue::hasHigherPriority(const Patient& a, const Patient& b) const {
    // Ưu tiên cao hơn nếu:
    // 1. Mức ưu tiên thấp hơn (CapCuu = 1 > ThongThuong = 2 > TaiKham = 3)
    // 2. Cùng mức ưu tiên thì đến trước
    if (a.priorityLevel != b.priorityLevel) {
        return a.priorityLevel < b.priorityLevel;
    }
    return parseTimeString(a.arrivalTime) < parseTimeString(b.arrivalTime);
}

void PriorityQueue::push(const Patient& patient) {
    Node* newNode = new Node(patient);
    newNode->data.status = DangCho;
    
    if (!head) {
        head = tail = newNode;
    } else if (hasHigherPriority(patient, head->data)) {
        // Chèn vào đầu
        newNode->next = head;
        head = newNode;
    } else if (!hasHigherPriority(patient, tail->data)) {
        // Chèn vào cuối
        tail->next = newNode;
        tail = newNode;
    } else {
        // Tìm vị trí thích hợp để chèn
        Node* current = head;
        while (current->next && hasHigherPriority(current->next->data, patient)) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    size++;
}

Patient PriorityQueue::pop() {
    if (empty()) {
        throw std::runtime_error("Hang doi rong!");
    }
    
    Patient result = head->data;
    result.status = DangKham;
    
    Node* temp = head;
    head = head->next;
    if (!head) {
        tail = nullptr;
    }
    delete temp;
    size--;
    
    return result;
}

bool PriorityQueue::empty() const {
    return head == nullptr;
}

size_t PriorityQueue::getSize() const {
    return size;
}

void PriorityQueue::printAll() const {
    if (empty()) {
        std::cout << "Hang doi rong.\n";
        return;
    }
    
    Node* current = head;
    while (current) {
        const Patient& p = current->data;
        std::cout << std::left
                  << std::setw(5)  << p.id
                  << std::setw(25) << p.name
                  << std::setw(10) << p.gender
                  << std::setw(12) << p.birthday.toString()
                  << std::setw(14) << p.citizenId
                  << std::setw(15) << priorityToString(p.priorityLevel)
                  << std::setw(20) << p.arrivalTime
                  << statusToString(p.status)
                  << "\n";
        current = current->next;
    }
} 