# 🚗 Smart Vehicle Control ECU (Software-in-the-Loop)

![C++](https://img.shields.io/badge/C++-14-blue.svg)
![Python](https://img.shields.io/badge/Python-3.8+-yellow.svg)
![Architecture](https://img.shields.io/badge/Architecture-AUTOSAR_Inspired-success.svg)
![Testing](https://img.shields.io/badge/Testing-Automated_SiL-orange.svg)

## 📖 Overview
This project simulates an Automotive **Body Control Module (BCM)** running in a **Software-in-the-Loop (SiL)** environment. It is designed to demonstrate modern embedded software engineering principles, specifically focusing on **Layered Architecture (inspired by AUTOSAR)**, **Non-blocking RTOS Task Management**, and **Requirement-Driven Automated Testing**.

Instead of running on a physical microcontroller, the C++ ECU runs on a Linux environment, communicating with a Python-based Test Automation Framework via TCP/IP sockets (simulating a CAN bus).

## 🏗️ System Architecture

The project strictly follows the **Separation of Concerns** principle to prevent "Spaghetti Code". The system is divided into three main layers:

1. **Application Layer (`app/`):** Contains the "Brain" of the system (`DoorControl`, `WiperControl`, `LightControl`). These modules handle pure business logic and know *nothing* about hardware.
2. **Runtime Environment (`services/Rte_*.h`):** The "Bulletin Board" of the system. Application tasks do not call each other directly. Instead, they write to and read from the RTE. This enables loose coupling (e.g., Wiper doesn't call Light directly; Wiper sets a flag in RTE, Light reads it).
3. **Hardware Abstraction / Mock Layer (`hal/mock/`):** The "Hands and Legs". Since this is an SiL simulation, real hardware drivers (GPIO, CAN controllers) are mocked. When the Application asks the RTE to turn on an LED, the Mock layer simply logs the action to the console.

## ✨ Core Product Requirements (Features)

The system is designed to fulfill specific, real-world Automotive Functional Requirements (FR):

* **[FR-001] Speed-Dependent Auto Door Lock:** Doors automatically lock when vehicle speed exceeds 20 km/h and unlock when the vehicle stops (0 km/h) using State Machine logic.
* **[FR-002] Crash Override (Functional Safety):** Upon receiving a critical Crash Event (CAN ID `0x050`), the system immediately overrides all tasks to unlock doors for escape and safely halt non-essential actuators.
* **[FR-003] Wiper-Light Synchronization (Inter-Task Communication):** If the wipers run continuously for 5 seconds, the system detects "Heavy Rain" and automatically turns on the headlights via the RTE flag, without the Wiper task directly invoking the Light task.
* **[FR-004] Interior Light Fade-Out (Non-blocking Timer):** When a door is closed, the interior light waits for a 5-second countdown before turning off. This uses a non-blocking time-slicing method, ensuring the MCU never "sleeps" and remains responsive to emergency events.
* **[SEC-01] Anti-DoS Protection:** The `SecurityFilter` drops CAN frames if flooded excessively, preventing the ECU from being overloaded by network noise.

## 🧪 Automated Testing Environment (SiL)

Testing embedded software shouldn't require manual button pressing on a physical board. This project includes a Python-based automated testing framework (`simulator/auto_test.py`).

* **How it works:** The Python script acts as a QA Engineer. It connects to the C++ ECU via TCP/IP, injects simulated CAN frames (e.g., Speed = 25km/h, Door = Open), waits for the ECU's State Machine to process the data, and then asserts the C++ console logs to grade the system.
* **Result:** Generates an automated `Test_Report.txt` grading every functional requirement (`PASS`/`FAIL`).

## 🚀 How to Build and Run

### Prerequisites

- Linux environment (Ubuntu recommended)
- `g++` compiler and `make`
- Python 3.x

### 1. Build the ECU Software

```bash
# Clean previous builds and compile the C++ application
make clean && make
```

### 2. Run the Automated Test Suite

```bash
# Navigate to the simulator directory
cd simulator

# Execute the QA Test Script
python3 auto_test.py
```

> **Note:** The script will automatically launch the C++ BCM in the background, send CAN commands, wait for the non-blocking timers, and generate a functional requirement test report.

### 3. Run Manual Dashboard (Optional)

If you want to manually inject CAN frames and observe the ECU behavior in real time:

```bash
# Terminal 1: Run the ECU
./bcm_sil_app

# Terminal 2: Run the interactive Python client
cd simulator
python3 client.py
```

---

## 📂 Project Structure

```text
Smart-BCM-SiL/
├── app/                  # Application Layer (Door, Wiper, Light Logic)
├── services/             # Runtime Environment (RTE) & OS Mock Services
├── hal/                  # Hardware Abstraction Layer (Mock CAN, UART, GPIO)
├── simulator/            # Python Test Scripts & Reports
├── Makefile              # Build Configuration
└── README.md             # Project Documentation
```