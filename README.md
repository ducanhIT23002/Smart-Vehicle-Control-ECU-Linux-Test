# 🚗 Smart BCM SiL (Body Control Module - Software-in-the-Loop)

![C++](https://img.shields.io/badge/C++-11%2F14-blue.svg)
![Python](https://img.shields.io/badge/Python-3.x-yellow.svg)
![RTOS](https://img.shields.io/badge/RTOS-CMSIS--OS2-green.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)

## 📖 Project Overview
**Smart BCM SiL** is a Software-in-the-Loop simulation environment for an Automotive Body Control Module. This project modernizes traditional C-based embedded systems by migrating to an **Object-Oriented C++ architecture**, strictly following the **AUTOSAR** layered model.

Beyond basic logic control (Doors, Wipers, Lighting), this project integrates critical automotive industry standards, including **UDS Diagnostics**, **Functional Safety (ISO 26262)**, and **Cybersecurity**.

---

## 🌟 Core Features

### 1. 🌐 Network Communication (CAN over TCP/IP)
- Simulates a high-speed CAN Bus using a TCP/IP Socket architecture (Non-blocking I/O).
- Capable of processing thousands of frames concurrently with sub-millisecond latency.

### 2. 🛠️ Diagnostics (UDS - ISO 14229)
- Implements standard UDS communication on ports `0x7E0` / `0x7E8`.
- Supports essential services: **Read DTC (0x19)** and **Clear DTC (0x14)**.
- **NvM (Non-volatile Memory):** Utilizes RAM Mirroring architecture to permanently store Diagnostic Trouble Codes (DTCs) and system states via File I/O.

### 3. 🛡️ Cybersecurity 
- Implements a Security Gate at the `CanDispatcher` level.
- Utilizes the **Token Bucket (Rate Limiting)** algorithm to block DoS (Denial of Service) attacks and prevent Message Queue overflow.

### 4. 🚑 Functional Safety (ISO 26262)
- **Watchdog Manager (WdgM):** Real-time Alive Supervision for all RTOS tasks.
- **Fail-Safe Mechanism:** Upon detecting a task deadlock, the Watchdog bypasses the OS Kernel to directly trigger the hardware (Force Headlight ON) to secure the driver before executing a system reset.

### 5. 🤖 Automation Test Framework
- A Python-based testing framework that interacts directly with the C++ process.
- Automatically injects normal operations, DoS attacks, and Deadlock malware, evaluates the system's response, and generates a professional `.txt` Test Report.

---

## 📂 Architecture Structure

```text
Smart-BCM-SiL/
├── AI_CONTEXT.md
├── ROADMAP.md
├── Makefile                  # Build script
├── eeprom_mock.dat           # Non-volatile memory storage (File I/O)
├── app/                      # APPLICATION LAYER (C++ Business Logic)
│   ├── main.cpp
│   ├── CanDispatcher.cpp/.hpp # Network router & Cybersecurity Shield
│   ├── DoorControl.cpp/.hpp   # Door operations
│   ├── InputMonitor.cpp/.hpp  # Sensor inputs & Event triggers
│   ├── LightControl.cpp/.hpp  # Headlight operations (Contains Crash target)
│   ├── WiperControl.cpp/.hpp  # Wiper operations
│   └── NvM.cpp/.hpp           # Non-volatile Memory Manager
├── hal/                      # HARDWARE ABSTRACTION LAYER (HAL)
│   └── mock/
│       ├── can_if.h/.c        # TCP/IP Socket driver
│       ├── gpio.h             # GPIO mock
│       └── uart.h/.c          # Terminal output logging
├── services/                 # SERVICES LAYER (RTOS & Base Services)
│   ├── EcuM.h                 # ECU State Manager
│   ├── Rte_*.h / rte_mock.c   # Runtime Environment (RTE)
│   ├── cmsis_os2.h/.c         # OS Kernel (Multi-threading & Queue)
│   └── wdg_manager.h/.c       # Watchdog Timer & Fail-Safe triggers
└── simulator/                # SIMULATION & TOOLING
    ├── auto_test.py           # Automated testing script
    ├── dashboard.py           # Python UDS Scanner & Attacker dashboard
    └── Test_Report*.txt       # Generated evaluation reports


# 🚀 Build & Run Instructions

## Step 1: Compile the C++ Source Code

Open a terminal, navigate to the project root directory, and build the project using the provided `Makefile`.

```bash
make clean
make
```

After the build completes successfully, the executable `bcm_sil_app` will be generated.

---

## Step 2: Run the Automated Test (Recommended)

This mode automatically performs the following tasks:

- Boots the C++ BCM application
- Simulates TCP/IP connections
- Injects predefined CAN payloads
- Evaluates Functional Safety responses
- Generates a test report

Run:

```bash
cd simulator
python3 auto_test.py
```

### Test Report

After execution, the test results can be found in:

```text
simulator/Test_Report.txt
```

The report contains the **PASS/FAIL** evaluation for each test scenario.

---

## Step 3: Run Manual Simulation Mode

Use this mode if you want to manually send UDS commands or test the Cybersecurity protection mechanisms.

### Terminal 1 — Start the BCM Server

```bash
./bcm_sil_app
```

### Terminal 2 — Start the Python Dashboard

```bash
cd simulator
python3 dashboard.py
```

---

# Available Dashboard Commands

| Command | Description |
|----------|-------------|
| `UDS READ` | Read DTC history from EEPROM. |
| `UDS CLEAR` | Clear all stored DTC records from EEPROM. |
| `SPAM 500` | Inject 500 spam CAN frames to trigger the **Token Bucket** protection mechanism. |
| `CRASH LIGHT` | Inject a Deadlock malware scenario to trigger the **Watchdog Fail-Safe** mechanism. |