#ifndef PATIENT_MANAGER_H
#define PATIENT_MANAGER_H

#include <string>
#include <fstream>
#include "Patient.h"
#include "PriorityQueue.h"

// Node cho danh sách liên kết lưu trữ tất cả bệnh nhân
struct PatientNode {
    Patient data;
    PatientNode* next;
    
    PatientNode(const Patient& patient) : data(patient), next(nullptr) {}
};

// Node cho hash table chaining (tìm kiếm theo tên)
struct NameHashNode {
    std::string name;
    PatientNode* patientNode;  // Pointer tới node trong danh sách chính
    NameHashNode* next;
    
    NameHashNode(const std::string& n, PatientNode* pNode) 
        : name(n), patientNode(pNode), next(nullptr) {}
};

// Node cho hash table chaining (tìm kiếm theo ID)
struct IdHashNode {
    int id;
    PatientNode* patientNode;  // Pointer tới node trong danh sách chính
    IdHashNode* next;
    
    IdHashNode(int i, PatientNode* pNode) 
        : id(i), patientNode(pNode), next(nullptr) {}
};

class PatientManager {
private:
    PatientNode* allPatientsHead;  // Linked list chứa tất cả bệnh nhân
    PatientNode* allPatientsTail;
    
    // Hash table cho tìm kiếm theo ID (O(1))
    static const int ID_HASH_SIZE = 101;  // Số nguyên tố
    IdHashNode* idHashTable[ID_HASH_SIZE];
    
    // Hash table cho tìm kiếm theo tên với chaining
    static const int NAME_HASH_SIZE = 101;  // Số nguyên tố
    NameHashNode* nameHashTable[NAME_HASH_SIZE];
    
    PriorityQueue waitingQueue;
    int nextId;
    size_t totalPatients;
    
    // Helper functions
    int hashId(int id) const;
    int hashName(const std::string& name) const;
    void insertToIdHash(int id, PatientNode* patientNode);
    void insertToNameHash(const std::string& name, PatientNode* patientNode);
    
    // CSV helpers
    std::string escapeCSV(const std::string& str) const;
    std::string unescapeCSV(const std::string& str) const;
    
public:
    PatientManager();
    ~PatientManager();
    
    // File I/O
    bool saveToCSV(const std::string& filename = "patients.csv") const;
    bool loadFromCSV(const std::string& filename = "patients.csv");
    
    // Quản lý bệnh nhân
    int addPatient(const std::string& name, const std::string& gender, 
                   const Date& birthday, const std::string& citizenId, PriorityLevel priority);
    
    // Cập nhật thông tin y tế
    bool updateMedicalInfo(int id, const std::string& symptoms, 
                          const std::string& diagnosis, const std::string& prescription);
    bool updateSymptoms(int id, const std::string& symptoms);
    
    // Quản lý hàng đợi
    Patient getNextPatient();
    void markPatientAsExamined(int id);
    void markPatientAsCompleted(int id);  // Đánh dấu đã khám xong
    
    // Tìm kiếm (O(1) cho ID, O(k) cho tên với k là số bệnh nhân cùng tên)
    PatientNode* findById(int id);
    PatientNode* findByCitizenId(const std::string& citizenId);  // Tìm theo số CCCD
    void findAllByName(const std::string& name, PatientNode**& results, int& count);  // Tìm tất cả
    void findByStatus(Status status, PatientNode**& results, int& count);
    void findByPriority(PriorityLevel priority, PatientNode**& results, int& count);
    
    // Hiển thị
    void printAllPatients() const;
    void printWaitingQueue() const;
    void printPatientInfo(const Patient& patient) const;
    void printDetailedPatientInfo(const Patient& patient) const;  // In thông tin chi tiết
    
    // Statistics
    size_t getTotalPatients() const { return totalPatients; }
    
    // Validation
    bool isValidInput(const std::string& name, const std::string& gender, 
                     const Date& birthday, const std::string& citizenId) const;
    bool isValidCitizenId(const std::string& citizenId) const;
};

#endif // PATIENT_MANAGER_H 