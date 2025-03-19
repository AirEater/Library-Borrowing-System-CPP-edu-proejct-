# Library Borrowing Management System

## Project Description
This project is a **console-based application** for managing **library book loans** and **student records**. It enables **librarians or library staff** to maintain an **up-to-date list of students** and the **books they have borrowed**. 

The system uses **dynamic memory management** with a **singly linked list** to store student records, allowing **efficient insertion and deletion** of entries without fixed-size limitations. All borrowing transactions, including **due dates** and **fine calculations** for overdue books, are tracked to ensure accurate library records.

---

## Key Features
✔ **Student Record Management**: Add new student records, search for students by name or ID, and remove student entries from the system.  
✔ **Book Borrowing & Returns**: Assign books to students when borrowed and update the record when books are returned. Each student record can track multiple borrowed books along with their due dates.  
✔ **Fine Calculation**: Automatically calculates **fines for overdue books** and maintains a running total of each student's outstanding fines.  
✔ **File I/O Support**: Load existing student and book records from a **file at startup**, and save all updated records back to the file upon exit. This ensures **data persistence** between sessions.  
✔ **Summary Reports**: Generate **summary statistics** (e.g., total students, books on loan, outstanding fines) to support librarians in **auditing records** and **making management decisions**.  

---

## Technologies Used
- **C++**: Core programming language used to build the application.  
- **Linked Lists**: Custom **singly linked list** data structure for managing student records dynamically in memory.  
- **File Handling**: Uses C++ file streams (`ifstream`/`ofstream`) to read and write data to files for **persistent storage** of records.  

---

## Authors
- Leong Yee Chung
- Ang Chin Siang
- Lee Jia Jie
