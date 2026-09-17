# 🏦 Comprehensive C++ Banking & ATM Suite

<div align="center">

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-11%2F17-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://en.cppreference.com/)
[![Architecture](https://img.shields.io/badge/Architecture-Modular%20%2F%20CLI%20Suite-orange?style=for-the-badge&logo=codeforces)](https://github.com/Abdullah-Shalgam/Bank-Management-and-ATM-System-CPP)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg?style=for-the-badge&logo=windows)](https://github.com/Abdullah-Shalgam/Bank-Management-and-ATM-System-CPP)
[![License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)](LICENSE)

<p align="center">
  A production-grade, modular, console-based banking ecosystem engineered in Modern C++. This repository features two fully integrated applications operating on a shared, synchronized flat-file database (<code>Clients.txt</code>), offering a complete simulation of enterprise administrative controls and customer-facing self-service ATM workflows.
</p>

</div>

---

## 🗂️ Table of Contents

1. [Architectural Overview & Ecosystem](#%EF%B8%8F-architectural-overview--ecosystem)
2. [Module Deep-Dive](#-module-deep-dive)
3. [Key C++ & Software Engineering Concepts](#-key-c--software-engineering-concepts)
4. [Data Serialization & Database Layout](#-data-serialization--database-layout)
5. [Repository Structure](#-repository-structure)
6. [Default Credentials](#-default-credentials)
7. [Build & Execution Guide](#-build--execution-guide)
8. [Contact & Developer Info](#-contact--developer-info)

---

## 🏛️ Architectural Overview & Ecosystem

The suite consists of two decoupled applications that work together through a shared transactional data protocol:

```text
               ┌─────────────────────────────────────────┐
               │         Shared Flat-File Storage        │
               │        (Clients.txt & Users.txt)        │
               └────────────────────┬────────────────────┘
                                    │
           ┌────────────────────────┴────────────────────────┐
           ▼                                                 ▼
┌──────────────────────────────┐                 ┌──────────────────────────────┐
│  📂 Bank Management Extension│                 │      📂 Self-Service ATM     │
│       (Admin & Employee)     │                 │      (Customer Terminal)     │
├──────────────────────────────┤                 ├──────────────────────────────┤
│ • Bitwise RBAC System        │                 │ • Masked PIN Capture         │
│ • Client Accounts CRUD       │                 │ • Interactive Quick Withdraw │
│ • User IAM Management        │                 │ • Instant Receipt Rendering  │
│ • Bank Capital Aggregation   │                 │ • Session Security Lockouts  │
└──────────────────────────────┘                 └──────────────────────────────┘
```

---

## 🌟 Module Deep-Dive

### 1️⃣ Bank Extension System (Admin & Employee Console)
Designed for financial staff to manage client accounts, configure role-based access, and execute administrative operations.

* **🔐 Authentication & Bitwise RBAC Engine:**
  * Secure multi-user login and session persistence.
  * Granular Permission Control using bitwise masks (`eAll`, `pListClients`, `pAddNewClient`, `pDeleteClient`, `pUpdateClient`, `pFindClient`, `pTransactions`, `pManageUsers`).
  * Admin Guardrails preventing accidental lockout or modification of primary administrative accounts.
* **👥 Complete User IAM Management (CRUD):** Add, update, delete, search, and assign dynamic permission bitmasks to bank operators.
* **💼 Client Accounts & Capital Control:** Full account lifecycle management, real-time total bank liquidity aggregation, deposits, and withdrawal management.

### 2️⃣ ATM System (Customer Terminal Console)
Designed for bank clients to perform self-service cash operations securely and intuitively.

* **🛡️ Security & Input Sanitation:**
  * Real-time masked PIN entry (`****`) with full Backspace and Enter handling using unbuffered console input.
  * Automatic Security Lockout System (temporary terminal freeze after 3 consecutive invalid PIN attempts).
* **💵 Financial Operations:**
  * **Quick Withdraw:** Preset standard cash amounts ($20 to $1000) for fast operations.
  * **Normal Withdraw:** Custom amount entry constrained to multiples of $5 with live balance validation.
  * **Deposit Engine:** Instant cash acceptance with persistent record serialization.
  * **Balance Inquiry:** Clear financial overview with printable timestamped receipts (`__DATE__` & `__TIME__`).
* **🎨 UI/UX & Visual Styling:**
  * Dynamic console UI elements (spinners, animated progress indicators, warning flags).
  * Smooth typewriter text animations.
* **👤 Profile Customization:** Self-service updates for customer name, phone number, and secure PIN changes.

---

## 🛠️ Key C++ & Software Engineering Concepts

* **Bitwise Operations (`|`, `&`):** Ultra-fast $\mathcal{O}(1)$ permission validation and role-based access bitmasking.
* **File I/O Stream & Data Persistence (`std::fstream`):** Line-by-line database parsing, delimiter-based data serialization (`#//#`), and multi-file synchronization.
* **Low-Level Unbuffered Console Input (`<conio.h>` / `_getch`):** Custom input stream capturing for star-masked PINs and dynamic terminal controls.
* **Input Sanitization & Validation (`<regex>`):** Strict input checking for account names, phone numbers, and financial entry limits.
* **Multi-Threading & Temporal Control (`<thread>`, `<chrono>`):** Asynchronous delays, smooth animations, and timed lockouts.
* **Modular Code Architecture:** Clean separation of concerns using custom headers (`.h`) and implementation files (`.cpp`).

---

## 💾 Data Serialization & Database Layout

All data is stored in text-based flat files using `#//#` as the record field delimiter.

### `Clients.txt` Layout
```text
AccountNumber#//#PinCode#//#Name#//#Phone#//#AccountBalance
```

### `Users.txt` Layout
```text
Username#//#Password#//#PermissionsBitmask
```

---

## 📂 Repository Structure

```text
Bank-Management-and-ATM-System-CPP/
│
├── 📂 Bank-System/                  # Admin & Staff Management Engine
│   ├── 📂 include/                  # System Header Files
│   ├── 📂 src/                      # Source Code Implementations
│   │   ├── InputLib.cpp
│   │   ├── MathLib.cpp
│   │   ├── MyStringLib.cpp
│   │   └── main.cpp                 # Bank System Entry Point
│   ├── Clients.txt                  # Client Database File
│   ├── Users.txt                    # System Users & Permissions
│   └── README.md                    # Module Documentation
│
├── 📂 ATM-System/                   # Self-Service ATM Customer Terminal
│   ├── 📂 include/                  # Terminal Header Files
│   ├── 📂 src/                      # Terminal Implementations
│   │   ├── InputLib.cpp
│   │   ├── MathLib.cpp
│   │   ├── MyStringLib.cpp
│   │   └── main.cpp                 # ATM Terminal Entry Point
│   ├── Clients.txt                  # Synchronized Client Database
│   └── README.md                    # Module Documentation
│
└── README.md                        # Primary Suite Documentation
```

---

## 🔑 Default Credentials

### Administrative Access (Bank System)

| Field | Default Value | Notes |
| :--- | :--- | :--- |
| **Username** | `Admin` | Case-insensitive lookup |
| **Password** | `1234` | System administrator |
| **Permissions** | Full Access (`-1`) | Bypasses all RBAC checks |

### Sample Client ATM Accounts (from `Clients.txt`)

| Account Number | PIN Code | Account Holder |
| :--- | :--- | :--- |
| `CL100` | `1861` | Hassan Tarhouni |
| `CL100` | `3359` | Zaid Al-Warfali |
| `CL100` | `9518` | Zaid Tarhouni Shlgoom |

---

## 🚀 Build & Execution Guide

### Toolchain Requirements
* C++11 or higher compliant compiler (`g++`, `clang++`, or `MSVC`).
* Windows OS (Required for low-level console features: `<conio.h>` and `system("color")`).

### 1️⃣ Building & Running the Bank System

```bash
cd Bank-System
g++ -std=c++17 main.cpp src/InputLib.cpp src/MathLib.cpp src/MyStringLib.cpp -Iinclude -o BankSystem
./BankSystem
```

### 2️⃣ Building & Running the ATM Terminal

```bash
cd ATM-System
g++ -std=c++17 main.cpp src/InputLib.cpp src/MathLib.cpp src/MyStringLib.cpp -Iinclude -o ATMSystem
./ATMSystem
```

---

## 📬 Contact & Developer Info

<p align="left">
  <a href="https://github.com/Abdullah-Shalgam"><img src="https://img.shields.io/badge/GitHub-Abdullah--Shalgam-181717?style=for-the-badge&logo=github" alt="GitHub"></a>
  <a href="mailto:bdallhshlghwm500@gmail.com"><img src="https://img.shields.io/badge/Gmail-bdallhshlghwm500%40gmail.com-D14836?style=for-the-badge&logo=gmail" alt="Gmail"></a>
  <a href="https://instagram.com/abdullah_shalgam"><img src="https://img.shields.io/badge/Instagram-@abdullah__shalgam-E4405F?style=for-the-badge&logo=instagram" alt="Instagram"></a>
  <a href="https://wa.me/218931364346"><img src="https://img.shields.io/badge/WhatsApp-%2B218%2093%20136%204346-25D366?style=for-the-badge&logo=whatsapp" alt="WhatsApp"></a>
</p>
