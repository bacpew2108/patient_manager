# 🏥 Hệ Thống Quản Lý Bệnh Nhân

> **Một hệ thống quản lý bệnh nhân hiện đại với hàng đợi ưu tiên, được viết bằng C++17**  
> **100% cấu trúc dữ liệu tự cài đặt - Không sử dụng STL containers**
---

## 📋 Mục Lục

- [✨ Tính Năng Chính](#-tính-năng-chính)
- [🔄 Quản Lý Trạng Thái](#-quản-lý-trạng-thái)
- [🏗️ Kiến Trúc Hệ Thống](#️-kiến-trúc-hệ-thống)
- [🚀 Cài Đặt & Chạy](#-cài-đặt--chạy)
- [📊 Cấu Trúc Dự Án](#-cấu-trúc-dự-án)
- [🔧 Hướng Dẫn Sử Dụng](#-hướng-dẫn-sử-dụng)
- [⚡ Hiệu Suất](#-hiệu-suất)
- [🛠️ Công Nghệ](#️-công-nghệ)

---

## ✨ Tính Năng Chính

### 🏥 **Quản Lý Bệnh Nhân**
- ➕ **Thêm bệnh nhân** với validation đầy đủ (tên, CCCD, ngày sinh)
- 🔄 **Hàng đợi ưu tiên** thông minh (Cấp cứu → Thông thường → Tái khám)
- 📞 **Gọi bệnh nhân** theo thứ tự ưu tiên và thời gian đến với header đẹp mắt
- 📝 **Cập nhật hồ sơ y tế** (triệu chứng, chẩn đoán, đơn thuốc)
- 🔄 **Tự động cập nhật trạng thái** khi hoàn thành khám

### 🔍 **Tìm Kiếm Tối Ưu**
- **O(1)** - Tìm kiếm theo ID với Hash Table
- **O(k)** - Tìm kiếm theo tên với Hash Table Chaining  
- **O(1)** - Tìm kiếm theo CCCD (12 chữ số)
- **O(n)** - Tìm kiếm theo trạng thái, mức ưu tiên
- 📊 **Hiển thị kết quả** dạng bảng đẹp mắt với header chuẩn

### 💾 **Lưu Trữ & Thống Kê**
- 📁 **Import/Export CSV** với text tiếng Việt cho trạng thái
- 📈 **Thống kê real-time** theo trạng thái bệnh nhân
- 🔄 **Auto-save** sau mỗi thao tác quan trọng
- 🌐 **UTF-8 Encoding** hỗ trợ tiếng Việt đầy đủ

---

## 🔄 Quản Lý Trạng Thái

### **📊 Enum Trạng Thái Mới**
```cpp
enum Status { 
    DangCho = 1,   // Đang chờ
    DangKham = 2,  // Đang khám 
    DaKham = 3     // Đã khám
};
```

### **🔄 Quy Trình Trạng Thái**
```
Đăng Ký → Đang Chờ → Được Gọi → Đang Khám → Hoàn Thành → Đã Khám
   📝         ⏳         📞        🩺         ✅        📋
```

### **🤖 Tự Động Hóa**
- **Khi thêm bệnh nhân**: Tự động set `DangCho`
- **Khi gọi khám**: Tự động chuyển sang `DangKham`
- **Khi cập nhật đầy đủ**: Tự động chuyển sang `DaKham`

---

## 🏗️ Kiến Trúc Hệ Thống

### **Cấu Trúc Dữ Liệu Tự Cài Đặt**

```
┌──────────────────────────────────────────────────────────────┐
│                    PATIENT MANAGER                           │
├──────────────────────────────────────────────────────────────┤ 
│  📋 Linked List        🔍 Hash Tables       ⏳ Priority Queue │
│  ├─ All Patients      ├─ ID Hash (O(1))    ├─ Waiting Queue  │
│  ├─ Sequential        ├─ Name Hash (O(k))  ├─ Smart Priority │
│  └─ Easy Traverse     ├─ CCCD Hash (O(1))  └─ Time-based     │
└──────────────────────────────────────────────────────────────┘
```

### **Thuật Toán Ưu Tiên**
1. **Cấp cứu** (Priority 1) - Ưu tiên cao nhất
2. **Thông thường** (Priority 2) - Ưu tiên trung bình  
3. **Tái khám** (Priority 3) - Ưu tiên thấp nhất
4. **Cùng mức ưu tiên** → Sắp xếp theo thời gian đến

---

## 🚀 Cài Đặt & Chạy

### **Yêu Cầu Hệ Thống**
- **C++ Compiler** hỗ trợ C++17 (GCC 7+, Clang 5+, MSVC 2017+)
- **Make** (tùy chọn - có thể build thủ công)

### **🔥 Cách 1: Sử Dụng Make (Khuyến nghị)**

```bash
cd DSAproject

# Build & Run
make clean && make
./patient_manager
```

### **⚙️ Cách 2: Build Thủ Công**

#### **🪟 Windows (MinGW/MSYS2)**
```cmd
mkdir obj
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/Patient.cpp -o obj/Patient.o
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/PatientManager.cpp -o obj/PatientManager.o
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/PriorityQueue.cpp -o obj/PriorityQueue.o
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/main.cpp -o obj/main.o
g++ obj/*.o -o patient_manager.exe
```

#### **🍎 macOS**
```bash
mkdir obj
clang++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/Patient.cpp -o obj/Patient.o
clang++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/PatientManager.cpp -o obj/PatientManager.o
clang++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/PriorityQueue.cpp -o obj/PriorityQueue.o
clang++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/main.cpp -o obj/main.o
clang++ obj/*.o -o patient_manager
```

#### **🐧 Linux**
```bash
mkdir obj
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/Patient.cpp -o obj/Patient.o
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/PatientManager.cpp -o obj/PatientManager.o
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/PriorityQueue.cpp -o obj/PriorityQueue.o
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/main.cpp -o obj/main.o
g++ obj/*.o -o patient_manager
```

---

## 📊 Cấu Trúc Dự Án

```
DSAproject/
├── 📁 include/               # Header files
│   ├── 📄 Date.h             # Cấu trúc ngày tháng với validation
│   ├── 📄 Patient.h          # Cấu trúc bệnh nhân và enums
│   ├── 📄 PriorityQueue.h    # Hàng đợi ưu tiên tự cài đặt
│   └── 📄 PatientManager.h   # Class quản lý chính
├── 📁 src/                   # Implementation files
│   ├── 📄 Patient.cpp        # Utility functions cho Patient
│   ├── 📄 PriorityQueue.cpp  # Thuật toán hàng đợi ưu tiên
│   ├── 📄 PatientManager.cpp # Logic quản lý và hash tables
│   └── 📄 main.cpp           # Giao diện người dùng
├── 📁 obj/                   # Object files (tạo khi build)
├── 📄 patients.csv           # Dữ liệu bệnh nhân (5 mẫu có sẵn)
├── 📄 patient_manager        # File thực thi
├── 📄 Makefile               # Build system
└── 📄 README.txt             # Tài liệu này
```

---

## 🔧 Hướng Dẫn Sử Dụng

### **📋 Menu Chính**
```
========================================================================
                     HE THONG QUAN LY BENH NHAN
========================================================================
1.  Them benh nhan moi          7.  Tim kiem benh nhan theo ID
2.  Goi benh nhan ke tiep       8.  Tim kiem benh nhan theo ten
3.  Hien thi hang doi cho kham  9.  Tim kiem benh nhan theo CCCD
4.  Hien thi tat ca benh nhan   10. Tim kiem benh nhan theo trang thai
5.  Xem thong tin chi tiet      11. Tim kiem benh nhan theo muc uu tien
6.  Cap nhat thong tin y te     12. Thong ke
0.  Thoat chuong trinh
------------------------------------------------------------------------
```

### **💡 Tính Năng Nổi Bật**

#### **🔍 Tìm Kiếm Theo Trạng Thái**
```
=== TIM KIEM BENH NHAN THEO TRANG THAI ===
1. Dang cho
2. Dang kham  
3. Da kham
```

#### **📞 Gọi Bệnh Nhân Với Header Đẹp**
```
=== GOI BENH NHAN KE TIEP ===
Da goi benh nhan:
================================================
ID   Ten                      GT    Ngay Sinh  ...
------------------------------------------------
```

#### **🤖 Tự Động Cập Nhật Trạng Thái**
- Nhập đầy đủ **chẩn đoán** + **đơn thuốc** → Tự động chuyển sang **"Đã khám"**

---

## ⚡ Hiệu Suất

|    Thao Tác    |  Độ Phức Tạp  |              Mô Tả                |
|----------------|---------------|-----------------------------------|
| Tìm theo ID    |  O(1)         | Hash Table với collision handling |
| Tìm theo tên   |  O(k)         | k = số bệnh nhân cùng tên         |
| Tìm theo CCCD  |  O(1)         | Hash Table (12 chữ số)            |
| Thêm bệnh nhân |  O(n)         | Amortized constant time           |
| Gọi bệnh nhân  |  O(1)         | Priority Queue với linked list    |
| Lưu/Tải CSV    |  O(n)         | Linear scan qua tất cả bệnh nhân  |

### **🎯 Tối Ưu Hóa**
- **Hash Table Size**: 101 (số nguyên tố) để giảm collision
- **Memory Management**: Smart pointer simulation với raw pointers
- **CSV Parsing**: Optimized string processing với escape handling
- **Status Sync**: Đồng bộ trạng thái giữa queue và linked list

---

## 🛠️ Công Nghệ

### **📚 Cấu Trúc Dữ Liệu Tự Cài Đặt**
- ✅ **Linked List** - Lưu trữ tuần tự tất cả bệnh nhân
- ✅ **Hash Table** - Tìm kiếm nhanh theo ID, tên và CCCD
- ✅ **Priority Queue** - Hàng đợi ưu tiên với linked list
- ✅ **Date Structure** - Xử lý ngày tháng với validation

### **🚫 Không Sử Dụng**
- ❌ `std::vector`, `std::list`, `std::queue`
- ❌ `std::map`, `std::unordered_map`
- ❌ `std::priority_queue`
- ❌ `std::sort`, `std::find`, `std::algorithm`

### **✅ Tuân Thủ Tiêu Chuẩn**
- 🎯 **100% cấu trúc dữ liệu tự cài đặt**
- 🎯 **Không dùng STL containers**
- 🎯 **Không dùng thuật toán có sẵn**
- 🎯 **C++17 standard compliant**

---

## 📈 **Thống Kê & Báo Cáo**

```
=== THONG KE BENH NHAN ===
========================================
Tong so benh nhan: 25
----------------------------------------
Dang cho kham: 12 benh nhan
Dang kham:     3 benh nhan  
Da kham xong:  10 benh nhan
========================================
```

---

## 🎯 **Tính Năng Nâng Cao**

- 🔄 **Auto-save** sau mỗi thao tác
- 📊 **Validation** đầy đủ input
- 🌐 **UTF-8** hỗ trợ tiếng Việt
- 🎨 **Pretty Print** tables
- 🤖 **Smart Status** management
- 💾 **CSV Export/Import** optimized

---
