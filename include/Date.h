#ifndef DATE_H
#define DATE_H

#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

struct Date {
    int day, month, year;
    
    // Constructor
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = ltm->tm_mon + 1;
        year = ltm->tm_year + 1900;
    }
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    // Toán tử so sánh
    bool operator>(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
    
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    
    bool operator==(const Date& other) const {
        return year == other.year && month == other.month && day == other.day;
    }
    
    bool operator!=(const Date& other) const {
        return !(*this == other);
    }
    
    bool operator>=(const Date& other) const {
        return *this > other || *this == other;
    }
    
    bool operator<=(const Date& other) const {
        return *this < other || *this == other;
    }
    
    // Chuyển đổi sang string format dd/mm/yyyy
    std::string toString() const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << day << "/"
            << std::setw(2) << month << "/"
            << year;
        return oss.str();
    }
    
    // Parse từ string format dd/mm/yyyy
    static Date fromString(const std::string& dateStr) {
        Date date;
        size_t pos1 = dateStr.find('/');
        size_t pos2 = dateStr.find('/', pos1 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            try {
                date.day = std::stoi(dateStr.substr(0, pos1));
                date.month = std::stoi(dateStr.substr(pos1 + 1, pos2 - pos1 - 1));
                date.year = std::stoi(dateStr.substr(pos2 + 1));
            } catch (const std::exception&) {
                // Trả về ngày không hợp lệ
                date.day = -1;
                date.month = -1;
                date.year = -1;
            }
        } else {
            // Format không đúng
            date.day = -1;
            date.month = -1;
            date.year = -1;
        }
        
        return date;
    }
    
    // Validate ngày hợp lệ
    bool isValid() const {
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Kiểm tra năm nhuận
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
            daysInMonth[1] = 29;
        }
        
        return day >= 1 && day <= daysInMonth[month - 1];
    }
};

#endif // DATE_H 