# Time and Humidity Measurement (Arduino Simulation)

**Sharif University of Technology (SUT)**
**Computer Structure and Machine Language Course — Semester 3 (Fall 2024) - Final Project**

![Language](https://img.shields.io/badge/Language-C++%20%7C%20Python-blue)
![Simulation](https://img.shields.io/badge/Simulator-Wokwi-yellow)
![License](https://img.shields.io/badge/license-MIT-purple)
![Repo Size](https://img.shields.io/github/repo-size/Amirmohammad-sharbati/computer-structure-course-mips?color=green)


## 📌 Project Overview  
This project implements a **time and humidity measurement system** using an Arduino board and sensors.  
Instead of using physical hardware, the project is **fully simulated** in [Wokwi](https://wokwi.com/), allowing development and testing without actual microcontrollers. 
Although the course topics were not directly about embedded systems, this project was completed as part of the final coursework effort, focusing on:  
- Writing structured C++ code for Arduino using PlatformIO.
- Simulating hardware behavior with Wokwi.
- Using Python for data collection and plotting diagrams.


## 📂 Repository Structure
- docs/  
    - circuit.png  ------> Image of ciruit.
    - python_plot.png ---> Image of python script result.
    - project_text.pdf --> Project description.
    - report.pdf  -------> Report of project, done by LaTeX. It is formal and structural report for project based on rules.
    - notes.pdf  --------> This is some notes I collect for start of project.
- programs/  
    - src/
        - main.cpp ------> The **main source file and core** of our project
    - plotting_code.py --> Python script for plotting sensor data logs
    - diagram.json ------> Wokwi circuit diagram file. Defines how the virtual Arduino and components are wired.
    - platformio.ini ----> PlatformIO project configuration file (board type, environment, libraries, etc).
    - wokwi.toml  -------> Wokwi simulation configuration file: Contains settings like which diagram.json to load, simulation options, etc.
- LICENSE  
- README.md
- .gitignore


## 🚀 How to Run
#### Option 1 - Run on Wokwi
1. Open [Wokwi](https://wokwi.com/)
1. Upload `diagram.json`, `wokwi.toml`, and `src/main.cpp`.
1. Start the simulation.

#### Option 2 - Run Locally with PlatformIO
1. Install VSCode and the PlatformIO extension.  
2. clone this repository:  
```bash
git clone https://github.com/AmirMohammad-Sharbati/computer-structure-course-project.git
cd arduino-time-humidity
```  
3. Open the project in VSCode.
4. Build and upload to the simulator

## 🔗 Related
- Exercises of course which are about MIPS assembly programming is available in a separate repository: https://github.com/AmirMohammad-Sharbati/computer-structure-course-mips.git

