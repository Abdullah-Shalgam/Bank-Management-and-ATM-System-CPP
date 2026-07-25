# 🏦 CLI Bank Client Management System (C++)

A comprehensive Console-based Banking System written in C++ that manages client records, persistent file storage, and financial transactions safely.

---

## ✨ Features
- **Full CRUD Operations:** Add, Show, Update, Delete, and Find client accounts seamlessly.
- **Persistent Data Storage:** Saves and loads client records automatically using a flat-file database (`Clients.txt`) with custom record delimiters (`#//#`).
- **Transaction System:**
  - **Deposit Money:** Increase account balances with instant persistent file sync.
  - **Withdraw Money:** Safe withdrawal system with insufficient balance checks.
  - **Total Balances Screen:** View summary of all clients and overall bank capital.
- **Strict Input Validation:** Custom validation for PIN codes (4-digit format), account uniqueness, and numeric values.

---

## 🛠️ Concepts & Architecture Applied
- **File I/O Stream (`fstream`):** Parsing text streams into C++ `vector<stClient>` objects and vice-versa.
- **Custom Parsing Engine:** Built `SplitText()` and formatting utilities to convert records between single-line formatted strings and C++ structures.
- **Modular Library Integration:** Built with reusable header libraries (`InputLib.h`) for clean UI/UX interaction.
- **Pass-by-Reference Optimization:** Utilizing `const vector<stClient>&` to maximize memory performance and execution speed.

---

## 🚀 How to Run

### Prerequisites
- C++ Compiler (`g++` / MSVC).
- Ensure `Clients.txt` and `InputLib.h` reside in the same root directory.

### Compilation & Execution
Using `g++` in Terminal:

```bash
# Compile the main file
g++ main.cpp -o BankSystem

# Run the system
./BankSystem