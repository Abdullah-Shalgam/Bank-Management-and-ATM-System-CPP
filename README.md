# 🏦 CLI Bank System Extension - V2 (C++)

A comprehensive, production-grade Console-based Banking Management System written in C++. Extended from the Course 7 foundational project to include **User Management**, **Authentication**, **Bitwise Access Control**, and **Multi-file Persistence**.

---

## ✨ Features

### 🔐 Authentication & Access Control (New in V2)
- **Login / Logout System:** Secure entry with username and password authentication screen.
- **Bitwise Permission Engine:** Fine-grained access control calculated using bitwise flags (`eAll`, `pListClients`, `pAddNewClient`, `pDeleteClient`, `pUpdateClient`, `pFindClient`, `pTranactions`, `pManageUsers`).
- **Access Denied Guard:** Rejects unauthorized feature access gracefully with warning prompts.
- **Admin Guardrails:** Protects critical administrative roles from accidental deletion and prevents logged-in users from deleting their own active accounts.

### 👥 User Management System (New in V2)
- **Full User CRUD Operations:** Add, Show, Update, Delete, and Find system users.
- **Dynamic Permission Setup:** Interactively configure custom permission sets for each new or updated user.
- **Persistent User Database:** Stores user access credentials and permission integers in `Users.txt`.

### 💼 Client Management & Financial Transactions
- **Full Client CRUD Operations:** Add, Show, Update, Delete, and Find client accounts seamlessly.
- **Transaction Engine:**
  - **Deposit Money:** Increase account balances with instant persistent file sync.
  - **Withdraw Money:** Safe withdrawal system with insufficient balance checks.
  - **Total Balances Screen:** View summary of all clients and total bank capital.
- **Data Persistence:** Automatic synchronization with `Clients.txt` using structured line delimiters (`#//#`).
- **Strict Input Validation:** Enforces 4-digit numeric PIN verification, account number uniqueness checks, and positive numeric inputs.

---

## 🛠️ Key C++ Concepts Applied

- **Bitwise Operators (`|`, `&`):** Efficient bitmasking for reading, assigning, and validating user permission flags.
- **Multi-file I/O Streams (`fstream`):** Parsing and serializing both `stClient` and `stUser` structs with file persistence (`Clients.txt` & `Users.txt`).
- **Structured Data Design:** Use of C++ `struct`, `enum`, and `vector` for clean state management and system execution.
- **Modular Library Architecture:** Integrated with `InputLib.h` for reliable input handling and validation.
- **Memory & Performance Optimization:** Clean pass-by-reference mechanisms to ensure efficient memory utilization.

---

## 📂 Project Structure

- `main.cpp` - Core application source code
- `InputLib.h` - Input validation and console helper library
- `Clients.txt` - Flat-file database for client records
- `Users.txt` - Flat-file database for users and permission flags

---

## 🔑 Default Login Credentials

For testing and demonstration, use the following administrator credentials:

| Field | Value |
| :--- | :--- |
| **Username** | `Admin` |
| **Password** | `1234` |
| **Permissions** | Full Access (`-1`) |

---

## 🚀 How to Run

### Prerequisites
- C++ Compiler (`g++`, `clang++`, or MSVC).
- Ensure `main.cpp`, `InputLib.h`, `Clients.txt`, and `Users.txt` are located in the same directory.

### Compilation & Execution
Using `g++` in Terminal:

```bash
# Compile the system
g++ main.cpp -o BankSystemV2

# Run the executable
./BankSystemV2