🎓 Student Management System (C++)
📌 Overview

A C++ role-based Student Management System that supports secure login and controlled access for Admin, Student, and Staff users. The system manages student records with file-based persistence and enforces privacy and access control.

✨ Features

• Secure login with hashed passwords
• Role-based access control (Admin / Student / Staff)
• Add, update, delete student records (Admin only)
• View and search student profiles
• File-based data storage
• Clean separation of logic and file handling

🧩 UML & Workflow

UML Class Diagram 

<img width="979" height="586" alt="image" src="https://github.com/user-attachments/assets/d0decf40-f3a8-4a11-980c-c1dd436a32c0" />

Workflow Diagram 

<img width="1093" height="1600" alt="image" src="https://github.com/user-attachments/assets/f7db7402-8aac-4de8-bf4b-07c1fd6cd187" />

🏗️ Core Components

• AuthManager – Handles authentication
• User – Stores logged-in user role and identity
• Student – Represents student data
• StudentManager – Business logic and access control
• FileManager – File input/output handling

💻 Tech Stack

Language: C++
Concepts: OOP, File Handling, STL

▶️ How to Run

g++ main.cpp -o student
./student
