#include "../include/PatientManager.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstring>
#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>

PatientManager::PatientManager() : allPatientsHead(nullptr), allPatientsTail(nullptr), nextId(1), totalPatients(0) {
    // Khởi tạo hash tables
    for (int i = 0; i < ID_HASH_SIZE; i++) {
        idHashTable[i] = nullptr;
    }
    for (int i = 0; i < NAME_HASH_SIZE; i++) {
        nameHashTable[i] = nullptr;
    }
}

PatientManager::~PatientManager() {
    // Xóa tất cả bệnh nhân
    PatientNode* current = allPatientsHead;
    while (current) {
        PatientNode* next = current->next;
        delete current;
        current = next;
    }
    
    // Xóa ID hash table
    for (int i = 0; i < ID_HASH_SIZE; i++) {
        IdHashNode* current = idHashTable[i];
        while (current) {
            IdHashNode* next = current->next;
            delete current;
            current = next;
        }
    }
    
    // Xóa Name hash table
    for (int i = 0; i < NAME_HASH_SIZE; i++) {
        NameHashNode* current = nameHashTable[i];
        while (current) {
            NameHashNode* next = current->next;
            delete current;
            current = next;
        }
    }
}

int PatientManager::hashId(int id) const {
    return id % ID_HASH_SIZE;
}

int PatientManager::hashName(const std::string& name) const {
    int hash = 0;
    for (char c : name) {
        hash = (hash * 31 + c) % NAME_HASH_SIZE;
    }
    return hash;
}

void PatientManager::insertToIdHash(int id, PatientNode* patientNode) {
    int index = hashId(id);
    IdHashNode* newNode = new IdHashNode(id, patientNode);
    newNode->next = idHashTable[index];
    idHashTable[index] = newNode;
}

void PatientManager::insertToNameHash(const std::string& name, PatientNode* patientNode) {
    int index = hashName(name);
    NameHashNode* newNode = new NameHashNode(name, patientNode);
    newNode->next = nameHashTable[index];
    nameHashTable[index] = newNode;
}

int PatientManager::addPatient(const std::string& name, const std::string& gender, 
                              const Date& birthday, const std::string& citizenId, PriorityLevel priority) {
    if (!isValidInput(name, gender, birthday, citizenId)) {
        throw std::invalid_argument("Du lieu khong hop le!");
    }
    
    std::string arrivalTime = getCurrentTimeString();
    Patient newPatient(nextId, name, gender, birthday, citizenId, priority, arrivalTime);
    
    // Tạo node mới
    PatientNode* newNode = new PatientNode(newPatient);
    
    // Thêm vào cuối linked list
    if (!allPatientsHead) {
        allPatientsHead = allPatientsTail = newNode;
    } else {
        allPatientsTail->next = newNode;
        allPatientsTail = newNode;
    }
    
    // Cập nhật hash tables
    insertToIdHash(nextId, newNode);
    insertToNameHash(name, newNode);
    
    // Thêm vào hàng đợi
    waitingQueue.push(newPatient);
    
    totalPatients++;
    return nextId++;
}

Patient PatientManager::getNextPatient() {
    if (waitingQueue.empty()) {
        throw std::runtime_error("Khong co benh nhan nao dang cho!");
    }
    
    Patient nextPatient = waitingQueue.pop();
    
    // Cập nhật trạng thái trong linked list và trả về patient với trạng thái đúng
    PatientNode* node = findById(nextPatient.id);
    if (node) {
        node->data.status = DangKham;
        nextPatient.status = DangKham;  // Đồng bộ trạng thái
    }
    
    return nextPatient;
}

void PatientManager::markPatientAsExamined(int id) {
    PatientNode* node = findById(id);
    if (node) {
        node->data.status = DaKham;  // Đánh dấu đã khám xong
    }
}

void PatientManager::markPatientAsCompleted(int id) {
    PatientNode* node = findById(id);
    if (node) {
        node->data.status = DaKham;  // Đánh dấu đã khám xong
    }
}

PatientNode* PatientManager::findById(int id) {
    int index = hashId(id);
    IdHashNode* current = idHashTable[index];
    
    while (current) {
        if (current->id == id) {
            return current->patientNode;
        }
        current = current->next;
    }
    return nullptr;
}

void PatientManager::findAllByName(const std::string& name, PatientNode**& results, int& count) {
    // Đếm số lượng trước
    count = 0;
    int index = hashName(name);
    NameHashNode* current = nameHashTable[index];
    
    while (current) {
        if (current->name == name) {
            count++;
        }
        current = current->next;
    }
    
    if (count == 0) {
        results = nullptr;
        return;
    }
    
    // Cấp phát mảng kết quả
    results = new PatientNode*[count];
    int i = 0;
    current = nameHashTable[index];
    
    while (current && i < count) {
        if (current->name == name) {
            results[i++] = current->patientNode;
        }
        current = current->next;
    }
}

void PatientManager::findByStatus(Status status, PatientNode**& results, int& count) {
    // Đếm số lượng trước
    count = 0;
    PatientNode* current = allPatientsHead;
    while (current) {
        if (current->data.status == status) {
            count++;
        }
        current = current->next;
    }
    
    if (count == 0) {
        results = nullptr;
        return;
    }
    
    // Cấp phát mảng kết quả
    results = new PatientNode*[count];
    int i = 0;
    current = allPatientsHead;
    
    while (current && i < count) {
        if (current->data.status == status) {
            results[i++] = current;
        }
        current = current->next;
    }
}

void PatientManager::findByPriority(PriorityLevel priority, PatientNode**& results, int& count) {
    // Đếm số lượng trước
    count = 0;
    PatientNode* current = allPatientsHead;
    while (current) {
        if (current->data.priorityLevel == priority) {
            count++;
        }
        current = current->next;
    }
    
    if (count == 0) {
        results = nullptr;
        return;
    }
    
    // Cấp phát mảng kết quả
    results = new PatientNode*[count];
    int i = 0;
    current = allPatientsHead;
    
    while (current && i < count) {
        if (current->data.priorityLevel == priority) {
            results[i++] = current;
        }
        current = current->next;
    }
}

void PatientManager::printAllPatients() const {
    if (!allPatientsHead) {
        std::cout << "Danh sach rong.\n";
        return;
    }
    
    // Header
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
    
    PatientNode* current = allPatientsHead;
    while (current) {
        printPatientInfo(current->data);
        current = current->next;
    }
    std::cout << std::string(112, '=') << "\n";
}

void PatientManager::printWaitingQueue() const {
    std::cout << "\n=== DANH SACH BENH NHAN DANG CHO ===\n";
    if (waitingQueue.empty()) {
        std::cout << "Hang doi rong.\n";
        return;
    }
    
    // Header
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
    
    waitingQueue.printAll();
    std::cout << std::string(112, '=') << "\n";
}

void PatientManager::printPatientInfo(const Patient& patient) const {
    std::string shortCitizenId = patient.citizenId.length() > 12 ? patient.citizenId.substr(0, 12) : patient.citizenId;
    
    std::cout << std::left
              << std::setw(5)  << patient.id
              << std::setw(25) << patient.name
              << std::setw(10) << patient.gender
              << std::setw(12) << patient.birthday.toString()
              << std::setw(14) << shortCitizenId
              << std::setw(15) << priorityToString(patient.priorityLevel)
              << std::setw(20) << patient.arrivalTime
              << statusToString(patient.status)
              << "\n";
}

bool PatientManager::isValidInput(const std::string& name, const std::string& gender, 
                                 const Date& birthday, const std::string& citizenId) const {
    if (name.empty() || name.length() > 50) {
        std::cout << "Ten khong hop le (rong hoac qua dai)!\n";
        return false;
    }
    
    if (gender != "Nam" && gender != "Nu" && gender != "Khac") {
        std::cout << "Gioi tinh khong hop le (Nam/Nu/Khac)!\n";
        return false;
    }
    
    if (!birthday.isValid()) {
        std::cout << "Ngay sinh khong hop le!\n";
        return false;
    }
    
    if (citizenId.empty()) {
        std::cout << "So CCCD khong duoc de trong!\n";
        return false;
    }
    
    if (!isValidCitizenId(citizenId)) {
        std::cout << "So CCCD khong hop le (phai co 12 chu so)!\n";
        return false;
    }
    
    return true;
}

// Cập nhật thông tin y tế
bool PatientManager::updateMedicalInfo(int id, const std::string& symptoms, 
                                      const std::string& diagnosis, const std::string& prescription) {
    PatientNode* node = findById(id);
    if (node) {
        node->data.symptoms = symptoms;
        node->data.diagnosis = diagnosis;
        node->data.prescription = prescription;
        
        // Tự động đánh dấu đã khám xong nếu có đầy đủ thông tin
        if (!diagnosis.empty() && !prescription.empty()) {
            node->data.status = DaKham;
        }
        
        return true;
    }
    return false;
}

bool PatientManager::updateSymptoms(int id, const std::string& symptoms) {
    PatientNode* node = findById(id);
    if (node) {
        node->data.symptoms = symptoms;
        return true;
    }
    return false;
}

// Tìm kiếm theo số CCCD
PatientNode* PatientManager::findByCitizenId(const std::string& citizenId) {
    PatientNode* current = allPatientsHead;
    while (current) {
        if (current->data.citizenId == citizenId) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// In thông tin chi tiết bệnh nhân
void PatientManager::printDetailedPatientInfo(const Patient& patient) const {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "           THONG TIN CHI TIET BENH NHAN\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "ID: " << patient.id << "\n";
    std::cout << "Ten: " << patient.name << "\n";
    std::cout << "Gioi tinh: " << patient.gender << "\n";
    std::cout << "Ngay sinh: " << patient.birthday.toString() << "\n";
    std::cout << "CCCD: " << patient.citizenId << "\n";
    std::cout << "Muc uu tien: " << priorityToString(patient.priorityLevel) << "\n";
    std::cout << "Thoi gian den: " << patient.arrivalTime << "\n";
    std::cout << "Trang thai: " << statusToString(patient.status) << "\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << "HO SO BENH AN\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << "Trieu chung:  " << (patient.symptoms.empty() ? "Chua cap nhat" : patient.symptoms) << "\n";
    std::cout << "Chuan doan :  " << (patient.diagnosis.empty() ? "Chua cap nhat" : patient.diagnosis) << "\n";
    std::cout << "Don thuoc  :  " << (patient.prescription.empty() ? "Chua cap nhat" : patient.prescription) << "\n";
    std::cout << std::string(60, '=') << "\n";
}

// Validation cho CCCD
bool PatientManager::isValidCitizenId(const std::string& citizenId) const {
    // CCCD phải có 12 chữ số
    if (citizenId.length() != 12) {
        return false;
    }
    
    // Tất cả phải là số
    for (char c : citizenId) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    
    return true;
}

// CSV helper functions
std::string PatientManager::escapeCSV(const std::string& str) const {
    if (str.find(',') != std::string::npos || str.find('"') != std::string::npos || str.find('\n') != std::string::npos) {
        std::string escaped = "\"";
        for (char c : str) {
            if (c == '"') {
                escaped += "\"\"";
            } else {
                escaped += c;
            }
        }
        escaped += "\"";
        return escaped;
    }
    return str;
}

std::string PatientManager::unescapeCSV(const std::string& str) const {
    if (str.length() >= 2 && str[0] == '"' && str[str.length()-1] == '"') {
        std::string unescaped = str.substr(1, str.length()-2);
        size_t pos = 0;
        while ((pos = unescaped.find("\"\"", pos)) != std::string::npos) {
            unescaped.replace(pos, 2, "\"");
            pos += 1;
        }
        return unescaped;
    }
    return str;
}

// File I/O operations
bool PatientManager::saveToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Header
    file << "ID,Ten,GioiTinh,NgaySinh,CCCD,UuTien,ThoiGianDen,TrangThai,TrieuChung,ChuanDoan,DonThuoc\n";
    
    // Data
    PatientNode* current = allPatientsHead;
    while (current) {
        const Patient& p = current->data;
        file << p.id << ","
             << escapeCSV(p.name) << ","
             << escapeCSV(p.gender) << ","
             << p.birthday.toString() << ","
             << p.citizenId << ","
             << static_cast<int>(p.priorityLevel) << ","
             << escapeCSV(p.arrivalTime) << ","
             << escapeCSV(statusToString(p.status)) << ","
             << escapeCSV(p.symptoms) << ","
             << escapeCSV(p.diagnosis) << ","
             << escapeCSV(p.prescription) << "\n";
        current = current->next;
    }
    
    file.close();
    return true;
}

bool PatientManager::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Khong the mo file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        try {
            std::string fields[11];  // Array to store all fields
            int fieldIndex = 0;
            bool inQuotes = false;
            std::string currentField;
            
            // Parse the line character by character
            for (size_t i = 0; i < line.length(); i++) {
                char c = line[i];
                
                if (c == '"') {
                    if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                        // Handle escaped quotes
                        currentField += '"';
                        i++;
                    } else {
                        // Toggle quote mode
                        inQuotes = !inQuotes;
                    }
                } else if (c == ',' && !inQuotes) {
                    // End of field
                    if (fieldIndex < 11) {
                        fields[fieldIndex++] = currentField;
                    }
                    currentField.clear();
                } else {
                    currentField += c;
                }
            }
            // Add the last field
            if (fieldIndex < 11) {
                fields[fieldIndex++] = currentField;
            }
            
            if (fieldIndex < 11) {
                std::cerr << "Dong khong hop le (thieu truong): " << line << std::endl;
                continue;
            }
            
            int id = std::stoi(fields[0]);
            std::string name = fields[1];
            std::string gender = fields[2];
            Date birthday = Date::fromString(fields[3]);
            std::string citizenId = fields[4];
            PriorityLevel priority = static_cast<PriorityLevel>(std::stoi(fields[5]));
            std::string arrivalTime = fields[6];
            Status status = stringToStatus(fields[7]);
            std::string symptoms = fields[8];
            std::string diagnosis = fields[9];
            std::string prescription = fields[10];
            
            // Create patient
            Patient newPatient(id, name, gender, birthday, citizenId, priority, arrivalTime,
                             symptoms, diagnosis, prescription);
            newPatient.status = status;
            
            // Add to data structures
            PatientNode* newNode = new PatientNode(newPatient);
            
            if (!allPatientsHead) {
                allPatientsHead = allPatientsTail = newNode;
            } else {
                allPatientsTail->next = newNode;
                allPatientsTail = newNode;
            }
            
            insertToIdHash(id, newNode);
            insertToNameHash(name, newNode);
            
            // Add to waiting queue if status is waiting
            if (status == DangCho) {
                waitingQueue.push(newPatient);
            }
            
            totalPatients++;
            if (id >= nextId) {
                nextId = id + 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Loi khi doc dong: " << line << "\nLoi: " << e.what() << std::endl;
            continue;
        }
    }
    
    file.close();
    return true;
} 