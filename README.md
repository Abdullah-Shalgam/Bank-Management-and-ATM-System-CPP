# 🏦 Comprehensive C++ Banking & ATM Suite

A production-grade, modular, console-based banking ecosystem written in modern C++. This repository contains two fully integrated applications operating on a shared flat-file client database (`Clients.txt`):
1. **Bank Extension System** (Admin & Employee Console)
2. **ATM System** (Customer Service Self-Terminal Console)

---

## 📑 Repository Structure & Architecture

This repository adopts a clean **Mono-Repo Layout** where every system operates as an isolated module while sharing data models and utilities.

```text
Bank-Management-and-ATM-System-CPP/
│
├── 📂 Bank-System/                  # Admin Management System Module
│   ├── 📂 include/                  # Library Header Files (InputLib.h, etc.)
│   ├── 📂 src/                      # Helper Source Files & Entry Point
│   │   ├── InputLib.cpp
│   │   ├── MathLib.cpp
│   │   ├── MyStringLib.cpp
│   │   └── main.cpp                 # Core Bank Extension Entry Point
│   ├── Clients.txt                  # Shared Client Flat-File Database
│   ├── Users.txt                    # System Users & Permissions Database
│   └── README.md                    # Module Documentation
│
├── 📂 ATM-System/                   # Self-Service ATM Module
│   ├── 📂 include/                  # Shared Helper Headers
│   ├── 📂 src/                      # Helper Source Files & Entry Point
│   │   ├── InputLib.cpp
│   │   ├── MathLib.cpp
│   │   ├── MyStringLib.cpp
│   │   └── main.cpp                 # Core ATM System Entry Point
│   ├── Clients.txt                  # Shared Client Flat-File Database Sync
│   └── README.md                    # Module Documentation
│
└── README.md                        # Primary Suite Overview Documentation
```

---

## 🌟 Modules Overview

### 1. 📂 Bank Extension System (Admin Console)
Designed for bank staff and system managers to control customer records, internal user access, and global transactions.

* **🔐 Authentication & Bitwise Permissions:**
  * User Login/Logout with password checking.
  * Bitwise Access Control Engine (`eAll`, `pListClients`, `pAddNewClient`, `pDeleteClient`, `pUpdateClient`, `pFindClient`, `pTranactions`, `pManageUsers`).
  * Admin Guardrails to protect active user sessions and primary admin roles.
* **👥 Full User Management (CRUD):** Add, Edit, Delete, List, and Find system users with custom permission sets.
* **💼 Client Accounts & Capital Control:** Full CRUD operations on client accounts, total bank balance aggregation, deposit, and withdrawal routines.

### 2. 📂 ATM System (Customer Terminal Console)
Designed for bank clients to perform self-service cash operations securely and intuitively.

* **🛡️ Security Guards & Masking:**
  * Real-time masked PIN entry (`****`) with full `Backspace` and `Enter` handling using low-level console inputs.
  * Security Lockout Protection (locks session for a cooldown period after 3 failed PIN attempts).
* **💵 Financial Operations:**
  * **Quick Withdraw:** One-touch presets ($20 to $1000).
  * **Normal Withdraw:** Custom amounts constrained to multiples of $5 with balance verification.
  * **Deposit Engine:** Instant cash acceptance with instant file serialization.
  * **Balance Inquiry & Account Details:** Real-time balance and customer info presentation.
* **🎨 UI/UX & Visual Effects:**
  * Interactive UI components (console spinners, progress bars, flashing alert indicators).
  * Smooth Typewriter Text Effect rendering.
  * Printable Transaction Receipts timestamped with live build/system times (`__DATE__` & `__TIME__`).
* **👤 Profile Updates:** Name changing with regex validation, phone number updating, and secure PIN modification.

---

## 🛠️ Key C++ Concepts Applied

- **Bitwise Operators (`|`, `&`):** Efficient bitmasking for reading, assigning, and validating permission flags.
- **File Stream & Data Persistence (`fstream`):** Fast line-by-line parsing and delimiter serialization (`#//#`) across multi-file databases.
- **Low-Level Console Input (`<conio.h>`):** Unbuffered key processing for masked PINs and interactive controls.
- **Regular Expressions (`<regex>`):** Strict input sanitization for client account names.
- **Multi-Threading & Time Controls (`<thread>`, `<chrono>`):** Asynchronous delays and smooth animations.
- **Modular Library Architecture:** Custom `InputLib` header/implementation separation for reusability.

---

## 🔑 Default Credentials

### Bank System Admin Credentials
| Field | Value |
| :--- | :--- |
| **Username** | `Admin` |
| **Password** | `1234` |
| **Permissions** | Full Access (`-1`) |

### Sample ATM Client Credentials (from shared `Clients.txt`)
| Account Number | PIN Code | Client Name |
| :--- | :--- | :--- |
| `CL1001` | `8614` | Hassan Tarhouni |
| `CL1003` | `3592` | Zaid Al-Warfali |
| `CL1009` | `5182` | Zaid Tarhouni Shlgoom |

---

## 🚀 How to Run

### Prerequisites
- C++11 compliant compiler (`g++`, `clang++`, or MSVC).
- **Windows OS** (required for `<conio.h>` and `system("color")` UI functionality).

### 1. Running the Bank Extension System
```bash
# Navigate to Bank System directory
cd Bank-System

# Compile the application
g++ main.cpp src/InputLib.cpp -Iinclude -o BankSystem

# Execute
./BankSystem
```

### 2. Running the ATM System
```bash
# Navigate to ATM System directory
cd ATM-System

# Compile the application
g++ main.cpp src/InputLib.cpp -Iinclude -o ATMSystem

# Execute
./ATMSystem
```
