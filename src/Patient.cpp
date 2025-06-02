#include "../include/Patient.h"
#include <cstdio>
#include <cstring>

std::string priorityToString(PriorityLevel level) {
    switch (level) {
        case CapCuu: return "Cap cuu";
        case ThongThuong: return "Thong thuong";
        case TaiKham: return "Tai kham";
        default: return "Khong ro";
    }
}

std::string statusToString(Status s) {
    switch (s) {
        case DangCho: return "Dang cho";
        case DangKham: return "Dang kham";
        case DaKham: return "Da kham";
        default: return "Khong ro";
    }
}

Status stringToStatus(const std::string& str) {
    if (str == "Dang cho") return DangCho;
    if (str == "Dang kham") return DangKham;
    if (str == "Da kham") return DaKham;
    return DangCho; // Mặc định
}

time_t parseTimeString(const std::string& timeStr) {
    int hour, minute, second = 0;
    int day, month, year;

    int matched = sscanf(timeStr.c_str(), "%d:%d:%d %d-%d-%d",
                         &hour, &minute, &second, &day, &month, &year);

    if (matched < 5) {
        matched = sscanf(timeStr.c_str(), "%d:%d %d-%d-%d",
                         &hour, &minute, &day, &month, &year);
        second = 0;
    }

    if (matched < 5) {
        std::cerr << "ERROR: Khong the phan tich chuoi thoi gian: " << timeStr << std::endl;
        return -1;
    }

    tm time = {};
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = second;
    time.tm_mday = day;
    time.tm_mon = month - 1;
    time.tm_year = year - 1900;

    return mktime(&time);
}

std::string getCurrentTimeString() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S %d-%m-%Y", ltm);
    return std::string(buffer);
} 