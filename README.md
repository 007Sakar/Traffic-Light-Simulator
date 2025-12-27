# Traffic-Light-Simulator
this project the simulation of traffic lights of the streets to maintin and manage the traffic

Course: Data Structure and Algorithms (COMP202)

Student Name: Sakar Dhimal  
Roll Number: 22  
Program: B.Sc. Computer Science  
Year/Semester: 2nd Year, 1st Semester  

Lecturer: Rupak Ghimere  

Submission: November 2025


Project Overview

This project is a traffic junction simulator developed as part of Assignment #1 for Data Structures and Algorithms (COMP202).
The goal of the project is to demonstrate the use of Queue data structures to manage vehicles at a traffic intersection under normal and priority-based traffic conditions.

The simulator visually represents vehicle movement, lane congestion, and traffic light behavior using SDL graphics.

 Features

Queue-based vehicle management for each road

Priority handling for lane AL2 when congestion exceeds threshold

Fair vehicle dispatch under normal traffic conditions

Real-time graphical visualization using SDL3

Deadlock prevention by allowing only one green signal at a time

🛠 Technologies Used

C Programming Language

Queue (Linear Data Structure)

Simple DirectMedia Layer (SDL3)

GCC Compiler

📁 Project Structure
Traffic-Light-Simulator/
│
├── simulator.c          # Main simulation + SDL visualization
├── traffic_generator.c  # Generates vehicles for each lane
├── receiver.c           # Receives vehicle data and updates queues
├── include/              # SDL header files
├── lib/                  # SDL library files
├── README.md

▶️ How to Run the Project

⚠️ Important:
The project uses multiple processes, so you must run each component in a separate terminal.

🔹 Terminal 1 – Vehicle Generator
gcc traffic_generator.c -o generator.exe
.\generator.exe


This program generates vehicles and sends them to the system.

🔹 Terminal 2 – Receiver
gcc receiver.c -o receiver.exe
.\receiver.exe


This program receives vehicle data and manages lane queues.

🔹 Terminal 3 – Simulator (SDL Visualization)
gcc simulator.c -o simulator.exe -Iinclude -Llib -lSDL3 -lm
.\simulator.exe


This launches the graphical traffic simulation.

🚦 Traffic Logic Summary

Normal Condition:
Vehicles from all lanes are served fairly using average queue length.

Priority Condition:
If vehicles in AL2 > 10, it gets immediate green signal.
Priority continues until queue size drops below 5, then normal scheduling resumes.

⏱ Time Complexity
Operation	Complexity
Enqueue Vehicle	O(1)
Dequeue Vehicle	O(1)
Priority Check	O(1)
Lane Selection	O(n)
⚠️ Limitations

Priority handling is condition-based, not heap-based

Simplified traffic generation

No real-world randomness or sensor input

🔮 Future Improvements

Implement heap-based priority queue

Adaptive traffic light timing

Emergency vehicle handling

More realistic traffic patterns

🔗 Repository Link

GitHub:
https://github.com/007Sakar/Traffic-Light-Simulator.git










.\simulator.exe
gcc simulator.c -o simulator -Iinclude -Llib -lSDL3

To run the program

terminal 1:

gcc traffic_generator.c -o generator.exe
.\generator.exe

Tweminal 2:

gcc receiver.c -o receiver.exe
.\receiver.exe

Terminal 3:

gcc simulator.c -o simulator.exe -Iinclude -Llib -lSDL3 -lm
.\simulator.exe

# 🚦 Traffic Light Simulator using Queue Data Structure

**Course:** Data Structure and Algorithms (COMP202)  
**Assignment:** #1 – Queue Implementation for Traffic Light Problem  
**Student:** Sakar Dhimal  
**Roll Number:** 22  
**Program:** B.Sc. Computer Science  
**Year / Semester:** 2nd Year, 1st Semester  
**Instructor:** Rupak Ghimere  
**Date:** December 2025  

---

## 📑 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Installation & Setup](#installation--setup)
- [How to Run](#how-to-run)
- [Project Structure](#project-structure)
- [Data Structures Used](#data-structures-used)
- [Algorithm Explanation](#algorithm-explanation)
- [Screenshots](#screenshots)
- [Demo Video](#demo-video)
- [References](#references)

---

## 🧠 Overview

This project simulates a **four-way traffic junction** using the **Queue data structure** to manage vehicle flow efficiently.  
It is designed to demonstrate how **linear data structures** can solve real-world problems such as **traffic congestion and priority handling**.

The system supports:
- **Normal traffic conditions** with fair vehicle dispatch
- **Priority traffic handling** for a designated lane
- **Graphical visualization** using **SDL3**

---

## 🚧 Problem Statement

A traffic junction consists of **four major roads (A, B, C, D)**.  
Each road has incoming lanes where vehicles queue before passing through traffic lights.

The system must handle:

- **Normal Condition:**  
  Vehicles from all lanes are served fairly based on average queue size.

- **Priority Condition:**  
  Lane **AL2** becomes a priority lane when more than **10 vehicles** are waiting and is served continuously until the count drops below **5**.

---

## ✨ Features

### Core Functionality
- 🚗 Queue-based vehicle management
- 🚦 Traffic light control with deadlock prevention
- ⚠️ Priority lane detection and handling
- 🔄 Continuous vehicle generation
- 📂 Inter-process communication between programs
- 🎮 Real-time SDL3 visualization

### Visual Features
- Distinct lane markings
- Clearly visible vehicles
- Red/Green traffic light indicators
- Dynamic movement simulation

---

## 🏗 System Architecture

