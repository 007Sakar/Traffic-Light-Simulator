# Traffic Light Simulator using SDL3

**Course:** Computer Graphics / Programming  
**Project:** Traffic Light Simulation  
**Student:** Sakar Dhimal  
**Roll Number:** 22  
**Program:** B.Sc. Computer Science  
**Instructor:** Rupak Ghimere  
**Institution:** Kathmandu University  
**Date:** 2025  

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Logic](#system-logic)
- [Installation & Setup](#installation--setup)
- [How to Run](#how-to-run)
- [Project Structure](#project-structure)
- [Technologies Used](#technologies-used)
- [Author](#author)
- [License](#license)

---

## Overview

- Simulation of a traffic light system using SDL3
- Demonstrates traffic signal control and lane management
- Designed for academic and learning purposes

---

## Features

- Red and Green traffic signals
- Multi-lane traffic simulation
- Priority given to the second lane
- Time-based signal switching
- Graphical visualization using SDL3

---

## System Logic

- Only one lane receives green signal at a time
- Other lanes remain stopped
- Signals rotate in a fixed sequence
- Priority lane is served first when active

---

## Installation & Setup

- Install a C / C++ compiler (GCC – MinGW)
- Download and configure SDL3
- Clone the project repository

---

## How to Run


Each program must be run in a separate terminal.

## Terminal 1 – Vehicle Generator
- gcc traffic_generator.c -o generator.exe
- .\generator.exe

## Terminal 2 – Receiver

- gcc receiver.c -o receiver.exe
- .\receiver.exe


## Terminal 3 – Simulator

- gcc simulator.c -o simulator.exe -Iinclude -Llib -lSDL3 -lm
- .\simulator.exe
---

## Project Structure

- src/ – Source code files
- README.md – Project overview
- Documentation/ – Detailed report

---

## Technologies Used

| Field | Details |
|------|--------|
| Programming Language | C / C++ |
| Graphics Library | SDL3 |
| Compiler | GCC (MinGW) |
| Platform | Windows |

---

---
## Demo Video
- [![Image](https://github.com/user-attachments/assets/d5d2f853-3cf8-47df-a742-799ff1050fad)](https://github.com/user-attachments/assets/efd9c7e2-d5fc-47f0-84e4-e13d978b77c2)
---

## Author

| Field | Details |
|------|--------|
| Name | Sakar Dhimal |
| Roll Number | 22 |
| Program | B.Sc. Computer Science |
| Institution | Kathmandu University |

---

## License

- This project is submitted for academic purposes only

---

**Last Updated:** 2025/12/27
<<<<<<< HEAD
=======

>>>>>>> d6f3af60884e5bc49487b01f8cfb3a0df2a901fb
