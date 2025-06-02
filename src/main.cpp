#include "../include/PatientManager.h"
#include <iostream>
#include <limits>
#include <string>
#include <iomanip>
#include <climits>

// Giao diện người dùng
class UserInterface {
private:
    PatientManager manager;
    
    void pauseScreen() {
        std::cout << "\nNhan Enter de tiep tuc...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    int getValidatedInt(const std::string& prompt, int min = 0, int max = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= min && value <= max) {
                std::cin.ignore();
                return value;
            }
            std::cout << "Gia tri khong hop le! Vui long nhap lai.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    std::string getValidatedString(const std::string& prompt, bool isMultiValue = false) {
        std::string value;
        while (true) {
            std::cout << prompt;
            if (isMultiValue) {
                std::cout << ": ";
            }
            std::getline(std::cin, value);
            
            // Loại bỏ khoảng trắng ở đầu và cuối
            value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
            value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
            
            if (!value.empty()) {
                if (isMultiValue) {
                    // Chuẩn hóa khoảng trắng xung quanh dấu phẩy
                    std::string result;
                    bool lastWasComma = true;  // Để tránh thêm khoảng trắng ở đầu
                    
                    for (size_t i = 0; i < value.length(); i++) {
                        if (value[i] == ',') {
                            if (!result.empty() && result.back() == ' ') {
                                result.pop_back();  // Xóa khoảng trắng trước dấu phẩy
                            }
                            result += ", ";
                            lastWasComma = true;
                        } else if (value[i] != ' ' || !lastWasComma) {
                            result += value[i];
                            lastWasComma = false;
                        }
                    }
                    return result;
                }
                return value;
            }
            std::cout << "Khong duoc de trong! Vui long nhap lai.\n";
        }
    }
    
    std::string getValidatedCitizenId(const std::string& prompt) {
        std::string value;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, value);
            
            // Kiểm tra độ dài và ký tự
            if (value.length() != 12) {
                std::cout << "CCCD phai co 12 chu so! Vui long nhap lai.\n";
                continue;
            }
            
            bool isValid = true;
            for (char c : value) {
                if (!std::isdigit(c)) {
                    isValid = false;
                    break;
                }
            }
            
            if (isValid) {
                return value;
            }
            std::cout << "CCCD chi duoc chua chu so! Vui long nhap lai.\n";
        }
    }
    
    std::string getValidatedName(const std::string& prompt) {
        std::string value;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, value);
            
            // Loại bỏ khoảng trắng ở đầu và cuối
            value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
            value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
            
            if (value.empty()) {
                std::cout << "Ten khong duoc de trong! Vui long nhap lai.\n";
                continue;
            }
            
            if (value.length() > 50) {
                std::cout << "Ten qua dai (toi da 50 ky tu)! Vui long nhap lai.\n";
                continue;
            }
            
            bool isValid = true;
            bool hasLetter = false;
            for (char c : value) {
                if (!std::isalpha(c) && !std::isspace(c)) {
                    isValid = false;
                    break;
                }
                if (std::isalpha(c)) {
                    hasLetter = true;
                }
            }
            
            if (!hasLetter) {
                std::cout << "Ten phai chua it nhat mot chu cai! Vui long nhap lai.\n";
                continue;
            }
            
            if (isValid) {
                return value;
            }
            std::cout << "Ten chi duoc chua chu cai va khoang trang! Vui long nhap lai.\n";
        }
    }
    
    Date getValidatedDate(const std::string& prompt) {
        while (true) {
            std::cout << prompt;
            std::string dateStr;
            std::getline(std::cin, dateStr);
            
            Date date = Date::fromString(dateStr);
            
            if (date.isValid()) {
                // Kiểm tra ngày không được trong tương lai
                Date currentDate;  // Ngày hiện tại
                if (date > currentDate) {
                    std::cout << "Ngay sinh khong duoc o tuong lai! Vui long nhap lai.\n";
                    continue;
                }
                return date;
            }
            std::cout << "Ngay khong hop le! Vui long nhap theo dinh dang dd/mm/yyyy.\n";
        }
    }
    
    void showMainMenu() {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "           HE THONG QUAN LY BENH NHAN\n";
        std::cout << std::string(60, '=') << "\n";
        std::cout << "1.  Them benh nhan moi\n";
        std::cout << "2.  Goi benh nhan ke tiep\n";
        std::cout << "3.  Hien thi hang doi cho kham\n";
        std::cout << "4.  Hien thi tat ca benh nhan\n";
        std::cout << "5.  Xem thong tin chi tiet benh nhan\n";
        std::cout << "6.  Cap nhat thong tin y te\n";
        std::cout << "7.  Tim kiem benh nhan theo ID\n";
        std::cout << "8.  Tim kiem benh nhan theo ten\n";
        std::cout << "9.  Tim kiem benh nhan theo CCCD\n";
        std::cout << "10. Tim kiem benh nhan theo trang thai\n";
        std::cout << "11. Tim kiem benh nhan theo muc do uu tien\n";
        std::cout << "12. Thong ke\n";
        std::cout << "0.  Thoat chuong trinh\n";
        std::cout << std::string(60, '-') << "\n";
    }
    
    void addPatient() {
        try {
            std::cout << "\n=== THEM BENH NHAN MOI ===\n";
            
            std::string name = getValidatedName("Nhap ten benh nhan: ");
            
            std::string gender;
            while (true) {
                std::cout << "Chon gioi tinh (1-Nam, 2-Nu, 3-Khac): ";
                int choice = getValidatedInt("", 1, 3);
                switch (choice) {
                    case 1: gender = "Nam"; break;
                    case 2: gender = "Nu"; break;
                    case 3: gender = "Khac"; break;
                }
                break;
            }
            
            Date birthday = getValidatedDate("Nhap ngay sinh (dd/mm/yyyy): ");
            
            std::string citizenId = getValidatedCitizenId("Nhap so CCCD (12 chu so): ");
            
            PriorityLevel priority;
            std::cout << "Chon muc uu tien(1-Cap cuu, 2-Thong thuong, 3-Tai kham): ";
            int priorityChoice = getValidatedInt("", 1, 3);
            priority = static_cast<PriorityLevel>(priorityChoice);
            
            std::string symptoms = getValidatedString("Nhap trieu chung", true);
            
            int id = manager.addPatient(name, gender, birthday, citizenId, priority);
            
            if (manager.updateSymptoms(id, symptoms)) {
                std::cout << "\nDa them benh nhan thanh cong! ID: " << id << "\n";
                
                // Lưu vào file CSV ngay sau khi thêm thành công
                if (manager.saveToCSV()) {
                    std::cout << "Da cap nhat du lieu vao file patients.csv\n";
                } else {
                    std::cout << "Luu file that bai!\n";
                }
            } else {
                std::cout << "\nDa them benh nhan thanh cong! ID: " << id << "\n";
                std::cout << "Loi khi cap nhat trieu chung!\n";
            }
            
        } catch (const std::exception& e) {
            std::cout << "Loi: " << e.what() << "\n";
        }
    }
    
    void callNextPatient() {
        try {
            Patient nextPatient = manager.getNextPatient();
            std::cout << "\n=== GOI BENH NHAN KE TIEP ===\n";
            std::cout << "Da goi benh nhan:\n";
            std::cout << std::string(112, '=') << "\n";
            std::cout << std::left
                      << std::setw(5)  << "ID"
                      << std::setw(25) << "Ten"
                      << std::setw(10) << "GT"
                      << std::setw(12) << "Ngay Sinh"
                      << std::setw(14) << "CCCD"
                      << std::setw(15) << "Uu Tien"
                      << std::setw(20) << "Thoi Gian Den"
                      << "Trang Thai"
                      << "\n";
            std::cout << std::string(112, '-') << "\n";
            manager.printPatientInfo(nextPatient);
            std::cout << std::string(112, '=') << "\n";
            
            // Lưu vào file CSV sau khi gọi bệnh nhân
            if (manager.saveToCSV()) {
                std::cout << "\nDa cap nhat du lieu vao file patients.csv\n";
            } else {
                std::cout << "Luu file that bai!\n";
            }
            
        } catch (const std::exception& e) {
            std::cout << "Loi: " << e.what() << "\n";
        }
    }
    
    void searchById() {
        std::cout << "\n=== TIM KIEM BENH NHAN THEO ID ===\n";
        int id = getValidatedInt("Nhap ID benh nhan: ");
        PatientNode* node = manager.findById(id);
        
        if (node) {
            std::cout << "\nKet qua tim kiem:\n";
            std::cout << std::string(112, '=') << "\n";
            std::cout << std::left
                      << std::setw(5)  << "ID"
                      << std::setw(25) << "Ten"
                      << std::setw(10) << "GT"
                      << std::setw(12) << "Ngay Sinh"
                      << std::setw(14) << "CCCD"
                      << std::setw(15) << "Uu Tien"
                      << std::setw(20) << "Thoi Gian Den"
                      << "Trang Thai"
                      << "\n";
            std::cout << std::string(112, '-') << "\n";
            manager.printPatientInfo(node->data);
            std::cout << std::string(112, '=') << "\n";
        } else {
            std::cout << "Khong tim thay benh nhan voi ID: " << id << "\n";
        }
    }
    
    void searchByName() {
        std::cout << "\n=== TIM KIEM BENH NHAN THEO TEN ===\n";
        std::string name = getValidatedString("Nhap ten benh nhan: ");
        PatientNode** results = nullptr;
        int count = 0;
        
        manager.findAllByName(name, results, count);
        
        if (count > 0) {
            std::cout << "\nTim thay " << count << " benh nhan:\n";
            std::cout << std::string(112, '=') << "\n";
            std::cout << std::left
                      << std::setw(5)  << "ID"
                      << std::setw(25) << "Ten"
                      << std::setw(10) << "GT"
                      << std::setw(12) << "Ngay Sinh"
                      << std::setw(14) << "CCCD"
                      << std::setw(15) << "Uu Tien"
                      << std::setw(20) << "Thoi Gian Den"
                      << "Trang Thai"
                      << "\n";
            std::cout << std::string(112, '-') << "\n";
            for (int i = 0; i < count; i++) {
                manager.printPatientInfo(results[i]->data);
            }
            std::cout << std::string(112, '=') << "\n";
            delete[] results;
        } else {
            std::cout << "Khong tim thay benh nhan voi ten: " << name << "\n";
        }
    }
    
    void searchByStatus() {
        std::cout << "\n=== TIM KIEM BENH NHAN THEO TRANG THAI ===\n";
        std::cout << "1. Dang cho\n2. Dang kham\n3. Da kham\n";
        int choice = getValidatedInt("Chon trang thai: ", 1, 3);
        Status status = static_cast<Status>(choice);
        
        PatientNode** results = nullptr;
        int count = 0;
        
        manager.findByStatus(status, results, count);
        
        if (count > 0) {
            std::cout << "\nTim thay " << count << " benh nhan:\n";
            std::cout << std::string(112, '=') << "\n";
            std::cout << std::left
                      << std::setw(5)  << "ID"
                      << std::setw(25) << "Ten"
                      << std::setw(10) << "GT"
                      << std::setw(12) << "Ngay Sinh"
                      << std::setw(14) << "CCCD"
                      << std::setw(15) << "Uu Tien"
                      << std::setw(20) << "Thoi Gian Den"
                      << "Trang Thai"
                      << "\n";
            std::cout << std::string(112, '-') << "\n";
            for (int i = 0; i < count; i++) {
                manager.printPatientInfo(results[i]->data);
            }
            std::cout << std::string(112, '=') << "\n";
            delete[] results;
        } else {
            std::cout << "Khong tim thay benh nhan nao!\n";
        }
    }
    
    void searchByPriority() {
        std::cout << "\n=== TIM KIEM BENH NHAN THEO MUC DO UU TIEN ===\n";
        std::cout << "1. Cap cuu\n2. Thong thuong\n3. Tai kham\n";
        int choice = getValidatedInt("Chon muc do uu tien: ", 1, 3);
        PriorityLevel priority = static_cast<PriorityLevel>(choice);
        
        PatientNode** results = nullptr;
        int count = 0;
        
        manager.findByPriority(priority, results, count);
        
        if (count > 0) {
            std::cout << "\nTim thay " << count << " benh nhan:\n";
            std::cout << std::string(112, '=') << "\n";
            std::cout << std::left
                      << std::setw(5)  << "ID"
                      << std::setw(25) << "Ten"
                      << std::setw(10) << "GT"
                      << std::setw(12) << "Ngay Sinh"
                      << std::setw(14) << "CCCD"
                      << std::setw(15) << "Uu Tien"
                      << std::setw(20) << "Thoi Gian Den"
                      << "Trang Thai"
                      << "\n";
            std::cout << std::string(112, '-') << "\n";
            for (int i = 0; i < count; i++) {
                manager.printPatientInfo(results[i]->data);
            }
            std::cout << std::string(112, '=') << "\n";
            delete[] results;
        } else {
            std::cout << "Khong tim thay benh nhan nao!\n";
        }
    }
    
    void showStatistics() {
        std::cout << "\n=== THONG KE BENH NHAN ===\n";
        std::cout << std::string(60, '=') << "\n";
        
        // Tổng số bệnh nhân
        std::cout << "Tong so benh nhan: " << manager.getTotalPatients() << "\n";
        std::cout << std::string(60, '-') << "\n";
        
        // Thống kê theo trạng thái
        PatientNode** results = nullptr;
        int count = 0;
        
        // Đang chờ
        manager.findByStatus(DangCho, results, count);
        std::cout << "Dang cho kham: " << count << " benh nhan\n";
        if (results) delete[] results;
        
        // Đang khám
        manager.findByStatus(DangKham, results, count);
        std::cout << "Dang kham:     " << count << " benh nhan\n";
        if (results) delete[] results;
        
        // Đã khám
        manager.findByStatus(DaKham, results, count);
        std::cout << "Da kham xong:  " << count << " benh nhan\n";
        
        std::cout << std::string(60, '=') << "\n";
    }
    
    void updateMedicalInfo() {
        try {
            int id = getValidatedInt("Nhap ID benh nhan: ");
            PatientNode* node = manager.findById(id);
            
            if (!node) {
                std::cout << "Khong tim thay benh nhan voi ID: " << id << "\n";
                return;
            }
            
            std::cout << "\n=== CAP NHAT THONG TIN Y TE ===\n";
            std::cout << "Benh nhan: " << node->data.name << " (ID: " << id << ")\n";
            std::cout << std::string(60, '-') << "\n";
            
            // Hiển thị triệu chứng
            std::cout << "Trieu chung hien tai: " << node->data.symptoms << "\n\n";
            
            // Cập nhật chuẩn đoán và đơn thuốc
            std::string diagnosis = getValidatedString("Nhap chuan doan", true);
            std::string prescription = getValidatedString("Nhap don thuoc", true);
            
            if (manager.updateMedicalInfo(id, node->data.symptoms, diagnosis, prescription)) {
                std::cout << "\nDa cap nhat thong tin y te thanh cong!\n";
                
                // Lưu vào file CSV ngay sau khi cập nhật thành công
                if (manager.saveToCSV()) {
                    std::cout << "Da cap nhat du lieu vao file patients.csv\n";
                } else {
                    std::cout << "Luu file that bai!\n";
                }
            } else {
                std::cout << "\nLoi khi cap nhat thong tin!\n";
            }
            
        } catch (const std::exception& e) {
            std::cout << "Loi: " << e.what() << "\n";
        }
    }
    
    void viewDetailedInfo() {
        try {
            int id = getValidatedInt("Nhap ID benh nhan: ");
            PatientNode* node = manager.findById(id);
            
            if (node) {
                manager.printDetailedPatientInfo(node->data);
            } else {
                std::cout << "Khong tim thay benh nhan voi ID: " << id << "\n";
            }
            
        } catch (const std::exception& e) {
            std::cout << "Loi: " << e.what() << "\n";
        }
    }
    
    void searchByCitizenId() {
        std::cout << "\n=== TIM KIEM BENH NHAN THEO CCCD ===\n";
        std::string citizenId = getValidatedCitizenId("Nhap so CCCD (12 chu so): ");
        PatientNode* node = manager.findByCitizenId(citizenId);
        
        if (node) {
            std::cout << "\nKet qua tim kiem:\n";
            std::cout << std::string(112, '=') << "\n";
            std::cout << std::left
                      << std::setw(5)  << "ID"
                      << std::setw(25) << "Ten"
                      << std::setw(10) << "GT"
                      << std::setw(12) << "Ngay Sinh"
                      << std::setw(14) << "CCCD"
                      << std::setw(15) << "Uu Tien"
                      << std::setw(20) << "Thoi Gian Den"
                      << "Trang Thai"
                      << "\n";
            std::cout << std::string(112, '-') << "\n";
            manager.printPatientInfo(node->data);
            std::cout << std::string(112, '=') << "\n";
        } else {
            std::cout << "Khong tim thay benh nhan voi CCCD: " << citizenId << "\n";
        }
    }
    
public:
    void run() {
        std::cout << "Chao mung ban den voi He thong Quan ly Benh nhan!\n";
        
        // Tự động đọc dữ liệu khi khởi động
        if (manager.loadFromCSV()) {
            std::cout << "Da tai du lieu tu file patients.csv\n";
        }
        
        int choice;
        do {
            showMainMenu();
            choice = getValidatedInt("Lua chon cua ban: ", 0, 12);
            
            switch (choice) {
                case 1:
                    addPatient();
                    break;
                case 2:
                    callNextPatient();
                    break;
                case 3:
                    manager.printWaitingQueue();
                    break;
                case 4:
                    std::cout << "\n=== TAT CA BENH NHAN ===\n";
                    manager.printAllPatients();
                    break;
                case 5:
                    viewDetailedInfo();
                    break;
                case 6:
                    updateMedicalInfo();
                    break;
                case 7:
                    searchById();
                    break;
                case 8:
                    searchByName();
                    break;
                case 9:
                    searchByCitizenId();
                    break;
                case 10:
                    searchByStatus();
                    break;
                case 11:
                    searchByPriority();
                    break;
                case 12:
                    showStatistics();
                    break;
                case 0:
                    std::cout << "Cam on ban da su dung he thong! Tam biet!\n";
                    break;
                default:
                    std::cout << "Lua chon khong hop le!\n";
            }
            
            if (choice != 0) {
                pauseScreen();
            }
            
        } while (choice != 0);
    }
};

int main() {
    try {
        UserInterface ui;
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "Loi nghiem trong: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 