#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <ctime>
#include <iostream>
#include "Date.h"

// Enum trạng thái và mức độ ưu tiên
enum Status { DangCho = 1, DangKham = 2, DaKham = 3 };
enum PriorityLevel { CapCuu = 1, ThongThuong = 2, TaiKham = 3 };

// Cấu trúc bệnh nhân
struct Patient {
    int id;
    std::string name;
    std::string gender;
    Date birthday;
    std::string citizenId;          // Số căn cước công dân
    PriorityLevel priorityLevel;
    std::string arrivalTime;
    Status status;
    std::string symptoms;           // Triệu chứng
    std::string diagnosis;          // Chuẩn đoán
    std::string prescription;       // Đơn thuốc
    
    // Constructor
    Patient() : id(0), citizenId(""), priorityLevel(ThongThuong), status(DangCho), 
                symptoms(""), diagnosis(""), prescription("") {}
    
    // Constructor với tham số cơ bản
    Patient(int id, const std::string& name, const std::string& gender, 
            const Date& birthday, const std::string& citizenId, PriorityLevel priority, 
            const std::string& arrivalTime)
        : id(id), name(name), gender(gender), birthday(birthday), citizenId(citizenId),
          priorityLevel(priority), arrivalTime(arrivalTime), status(DangCho),
          symptoms(""), diagnosis(""), prescription("") {}
          
    // Constructor với tham số đầy đủ
    Patient(int id, const std::string& name, const std::string& gender, 
            const Date& birthday, const std::string& citizenId, PriorityLevel priority, 
            const std::string& arrivalTime, const std::string& symptoms,
            const std::string& diagnosis, const std::string& prescription)
        : id(id), name(name), gender(gender), birthday(birthday), citizenId(citizenId),
          priorityLevel(priority), arrivalTime(arrivalTime), status(DangCho),
          symptoms(symptoms), diagnosis(diagnosis), prescription(prescription) {}
};

// Utility functions
std::string priorityToString(PriorityLevel level);
std::string statusToString(Status s);
Status stringToStatus(const std::string& str);
time_t parseTimeString(const std::string& timeStr);
std::string getCurrentTimeString();

#endif // PATIENT_H 