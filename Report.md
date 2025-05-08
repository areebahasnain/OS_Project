# 🏛️ Advanced Library Management System with OS-Level Threading

## 📚 Overview

This project implements a **multi-threaded Library Management System** in C, demonstrating key **Operating Systems concepts** such as:

- Reader-Writer Synchronization
- Dining Philosophers Problem
- POSIX Threading (`pthread`)
- Mutex-based concurrency control

It supports:
- User login (admin and member)
- Book borrowing/returning
- Book reservation
- Admin-only functions like add/remove books and generate reports
- Interactive CLI menus
- Simulation of concurrency problems

---

## 🔧 Features

### ✅ Library Operations
- Login/Register with role-based access
- Borrow and return books (2-week auto due date)
- Reservation if book is unavailable
- View available books with status
- Admins can:
  - Add or remove books
  - Generate overdue report

### ✅ OS Simulations
#### 1. Readers-Writers Problem
- Multiple readers view the catalog concurrently
- Writers update book records
- Threads synchronized with mutexes
- Clean 15-second simulation with controlled thread termination

#### 2. Dining Philosophers Problem
- Demonstrates deadlock prevention using alternate fork grabbing order
- 5 philosophers eat/think 3 times each

---

## 🧵 Threading & Synchronization

- All shared resources (books/users) are protected with `pthread_mutex_t`
- `volatile int stop_execution` used to safely control simulation loops
- Threads terminate gracefully and are `pthread_join()`-ed

---

## 🛠️ Build Instructions

```bash
# Compile the program
gcc -o library_system main.c lib.c din.c -lpthread

# Run the system
./library_system
```

---

## 📂 File Structure

```
├── main.c           # Main menu, user interactions
├── lib.c            # Library operations (borrow, return, add, etc.)
├── lib.h / library.h
├── din.c            # Dining philosophers implementation
├── dining.h
└── Makefile         # (optional) to simplify compilation
```

---

## 📈 Example Output

```bash
==========================================
   Advanced Library Management System     
==========================================

Main Menu:
1. Login
2. Register
3. Run Simulation Mode
4. View Library as Guest
5. Exit
```

```bash
[Reader 1] Viewing books:

Current Library Status:
--------------------------------------------------
Title                    | Author             | Status     | Borrowed By   | Reserved By    | Due Date
Atomic Habits            | James Clear        | Available  |               |                | N/A
...
```

---

## 📌 Dependencies

- Standard C Libraries
- POSIX Threads (`-lpthread`)
- GCC compiler

---

## 🧑‍💻 Authors

- ✍️ Developed by Areeba Hasnain . Emman Ali
- 🛠️ OS concepts implemented manually for learning

---

