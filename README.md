


#  Traffic Light Simulator using Queue Data Structure

**Course:** Data Structure and Algorithms (COMP202)  
**Assignment:** #1 – Queue Implementation for Traffic Light Problem  

---

##  Student Information

| Field | Details |
|------|--------|
| **Name** | Sakar Dhimal |
| **Roll No** | 22 |
| **Program** | B.Sc. Computer Science |
| **Year / Semester** | 2nd Year, 1st Semester |
| **Instructor** | Rupak Ghimere |

---

##  Overview

This project implements a **traffic junction simulator** to demonstrate the practical use of the **Queue (linear data structure)** in solving a real-world traffic management problem.

The simulator models a **four-road intersection** where vehicles arrive, wait in queues, and are dispatched based on traffic light conditions.

The system supports:
- **Normal traffic conditions** with fair vehicle servicing  
- **Priority traffic conditions** for a designated lane  
- **Graphical visualization using SDL3**

---

##  Problem Statement

At a traffic junction connecting four major roads (A, B, C, and D), vehicles must be managed efficiently to avoid congestion and deadlock.

The system must handle:

- **Normal Condition:**  
  Vehicles from all lanes are served fairly based on average queue size.

- **Priority Condition:**  
  If the priority lane accumulates more than **10 vehicles**, it is served first until the count drops below **5**, after which normal conditions resume.

---

##  Features

### Core Features
- Queue-based vehicle management for each lane
- Priority handling for a designated lane
- Traffic light coordination to avoid deadlock
- Continuous vehicle generation
- Multi-process execution
- SDL3-based visualization

---

##  System Architecture

┌────────────────────┐
│ Traffic Generator │
│ (generator.exe) │
└─────────┬──────────┘
│
▼
┌────────────────────┐
│ Receiver │
│ (receiver.exe) │
└─────────┬──────────┘
│
▼
┌────────────────────┐
│ Simulator │
│ (simulator.exe) │
│ - Queue logic │
│ - Traffic lights │
│ - SDL3 rendering │
└────────────────────┘

yaml
Copy code

Each component runs independently and communicates to maintain the simulation flow.

---

## ⚙ Technologies Used

- **C Programming Language**
- **Queue Data Structure**
- **Simple DirectMedia Layer (SDL3)**
- **GCC Compiler**

---

## ▶ How to Run the Project

**Each program must be run in a separate terminal**

### 🔹 Terminal 1 – Vehicle Generator
```bash
gcc traffic_generator.c -o generator.exe
.\generator.exe
🔹 Terminal 2 – Receiver
bash
Copy code
gcc receiver.c -o receiver.exe
.\receiver.exe
🔹 Terminal 3 – Simulator (SDL3)
bash
Copy code
gcc simulator.c -o simulator.exe -Iinclude -Llib -lSDL3 -lm
.\simulator.exe
📁 Project Structure
makefile
Copy code
Traffic-Light-Simulator/
├── simulator.c          # Main simulation & SDL visualization
├── traffic_generator.c  # Vehicle generation logic
├── receiver.c           # Queue update logic
├── include/             # SDL3 header files
├── lib/                 # SDL3 library files
├── README.md
 Data Structures Used
Data Structure	Purpose
Queue	Store vehicles waiting in each lane
Struct	Represent vehicles, lanes, and traffic lights

Queue Implementation
c
Copy code
typedef struct {
    Vehicle vehicles[MAX_VEHICLES];
    int front;
    int rear;
    int count;
} Queue;
Operations:

Enqueue → O(1)

Dequeue → O(1)

Empty/Full check → O(1)

 Algorithm Overview
Initialize queues for all lanes

Generate vehicles continuously

Monitor the priority lane

Activate priority mode if threshold exceeded

Serve vehicles based on current traffic condition

Update traffic lights

Render output using SDL3

⏱ Time Complexity
Operation	Complexity
Enqueue	O(1)
Dequeue	O(1)
Priority Check	O(1)
Lane Selection	O(n)

🎥 Demo Video
A video demonstrating:

Compilation of all modules

Multi-terminal execution

SDL3 visualization



🔗 Repository Link
https://github.com/007Sakar/Traffic-Light-Simulator.git

📜 License
This project is submitted as part of academic coursework for
COMP202 – Data Structure and Algorithms.
 Acknowledgments
Rupak Ghimere – Course Instructor

Department of Computer Science

Last Updated: December 2025
























