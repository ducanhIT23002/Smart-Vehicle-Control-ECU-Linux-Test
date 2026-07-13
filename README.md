# 🚗 Smart Vehicle Control ECU (Software-in-the-Loop)

![C++](https://img.shields.io/badge/C++-14-blue.svg)
![Linux](https://img.shields.io/badge/Linux-POSIX_API-success.svg)
![Architecture](https://img.shields.io/badge/Architecture-AUTOSAR_Inspired-orange.svg)
![Testing](https://img.shields.io/badge/Testing-Automated_SiL-yellow.svg)

## 📖 Background & Problem Statement
Modern vehicles contain dozens of Electronic Control Units (ECUs) communicating over the CAN network. The **Body Control Module (BCM)** is a critical node responsible for coordinating body-related features such as door locking, lighting, and wiper systems.

This project implements a **Software-in-the-Loop (SiL) simulation** of a BCM running on Linux. It is designed to demonstrate embedded software architecture, RTOS-inspired scheduling, requirement-driven development, and automated functional testing without requiring physical hardware.

---

## 🏗️ Software Architecture & Thread Model

The system strictly follows the **Separation of Concerns** principle via a layered architecture to ensure loose coupling and high portability.

### 1. Deep Architecture Diagram
```text
  [TCP/IP Socket] (Simulating Physical CAN Bus)
         │
         ▼
  [CAN Mock / RX Thread]
         │
         ▼
  [Security Filter]  <-- Prevents DoS, ensures deterministic execution
         │
         ▼
  [Dispatcher Queue]
         │
         ▼
  [Application Layer Scheduler]
   ├── DoorControl
   ├── LightControl
   └── WiperControl
         │
         ▼
  [RTE (Runtime Environment)] <-- Data Bulletin Board (Loose Coupling)
         │
         ▼
  [HAL (Hardware Abstraction)]
         │
         ▼
  [Logger / TX Thread]
```


### 2. Linux OS Integration

Although the project runs on Linux instead of a real microcontroller, it uses the Linux POSIX API to emulate a simplified RTOS execution model commonly found in embedded systems.

- **POSIX Threads (`pthread`)**  
  Implements concurrent execution for the Main, Mock CAN RX, Application Task Scheduler, and Logger threads.

- **Mutexes & Condition Variables**  
  Protect shared resources and synchronize communication between worker threads, ensuring thread-safe access to the shared data layer and message queues.

- **TCP Sockets**  
  Emulate CAN bus communication during Software-in-the-Loop (SiL) testing. The transport layer is abstracted so that the application logic remains independent of the underlying communication mechanism.

- **Periodic Task Scheduling**  
  Uses timer-based execution and non-blocking delays to periodically update application state machines without blocking other concurrent tasks.

- **File I/O**  
  Simulates Non-Volatile Memory (NvM) by storing persistent ECU data in local files, mimicking EEPROM behavior.

---

### 3. Thread Model

The software is organized into multiple cooperative threads to simulate the concurrent execution model of an embedded ECU.

- **Main Thread**  
  Initializes all software modules, loads simulated EEPROM data, creates worker threads, and starts the ECU runtime.

- **Mock CAN RX Thread**  
  Listens for incoming TCP messages representing simulated CAN frames, validates the received data, and forwards valid frames to the dispatcher.

- **Application Task Scheduler**  
  Executes periodically (e.g., every 50 ms) to update the state machines of `DoorControl`, `WiperControl`, and `LightControl`.

- **Logger / TX Thread**  
  Processes outgoing messages asynchronously and prints ECU events, status updates, and diagnostic information to the console.

---

## 🔄 Data Flow Example

The following example illustrates how a simulated vehicle event propagates through the software stack.

```text
[Speed Frame: 25 km/h]
        │
        ▼
[Security Filter]
 (Rate Limiting & Validation)
        │
        ▼
[Dispatcher]
 (Route by CAN ID)
        │
        ▼
[DoorControl State Machine]
        │
        ▼
[Simplified RTE / Shared Data Layer]
        │
        ▼
[Logger / TX Thread]
 (Door Locked → CAN ID 0x101)
```

**Execution Flow**

1. A simulated speed frame is received through the TCP socket.
2. The Security Filter validates the frame and applies rate limiting to prevent excessive traffic.
3. The Dispatcher routes the frame to the appropriate application module based on its CAN identifier.
4. `DoorControl` updates its state machine according to the current vehicle speed.
5. The new door status is published to the shared data layer (Simplified RTE).
6. The Logger/TX thread reports the updated ECU status to the console or simulated communication interface.

> **Security Filter**  
> The Security Filter serves as the first stage of the communication pipeline. It performs basic input validation and rate limiting using a Token Bucket algorithm to mitigate excessive CAN traffic. By preventing message bursts from overwhelming the dispatcher queue, it helps maintain predictable execution of time-critical application tasks during Software-in-the-Loop (SiL) testing.

## ✨ Requirement Traceability Matrix

The project follows a requirement-driven development approach inspired by automotive software engineering practices. Each Functional Requirement (FR) is mapped to its corresponding software module and verification test case, ensuring clear traceability from implementation to validation.

| Req ID | Description | Module | Test Case |
|--------|-------------|--------|-----------|
| **FR-001** | **Speed-Dependent Auto Door Lock:** Automatically locks doors when vehicle speed exceeds **20 km/h** and unlocks them when speed returns to **0 km/h**. | `DoorControl` | `TC-001` |
| **FR-002** | **Crash Override:** Immediately unlocks all doors and overrides normal actuator behavior upon receiving a crash event. | `DoorControl`, `LightControl` | `TC-002` |
| **FR-003** | **Wiper-Light Synchronization:** Automatically turns on the headlights when the windshield wipers remain active for more than **5 seconds**. | `WiperControl`, `LightControl` | `TC-003` |
| **FR-004** | **Interior Light Fade-Out:** Keeps the interior light on for **5 seconds** after the door is closed using a non-blocking software timer. | `DoorControl` | `TC-004` |
| **SEC-001** | **CAN Rate Limiting:** Applies a Token Bucket algorithm to limit excessive incoming CAN traffic during SiL simulation. | `SecurityFilter` | `TC-SEC` |

## 🧪 Automated Testing Environment (SiL)

Embedded software verification should be repeatable, automated, and independent of physical hardware. To achieve this, the project includes a Python-based Software-in-the-Loop (SiL) test framework.

### Test Workflow

The Python automation script acts as a lightweight test framework. It launches the C++ ECU application, establishes a TCP connection, injects predefined simulated CAN frames, waits for the application state machines and software timers to complete execution, and verifies the ECU responses against the expected functional requirements.

### Test Output

After all test cases have been executed, the framework automatically generates a `Test_Report.txt` summarizing the verification results for each requirement, including **PASS** and **FAIL** status.

## 🚀 How to Build and Run

### Prerequisites

Before building the project, ensure the following tools are installed:

- Linux environment (Ubuntu 22.04 LTS recommended)
- `g++` compiler (C++14 or later)
- `make`
- Python 3.x

---

### 1. Build the ECU Software

Compile the C++ Software-in-the-Loop (SiL) application.

```bash
# Clean previous builds
make clean

# Build the ECU application
make
```

---

### 2. Run the Automated Test Suite

Execute the Python-based Software-in-the-Loop test framework.

```bash
cd simulator

python3 auto_test.py
```

> **Note:**  
> The test framework automatically launches the ECU application, injects predefined simulated CAN frames through the TCP interface, waits for software timers and state machines to complete execution, and generates a `Test_Report.txt` summarizing the verification results.

---

### 3. Run Manual Simulation (Optional)

To manually interact with the ECU during development or debugging:

**Terminal 1 — Start the ECU**

```bash
./bcm_sil_app
```

**Terminal 2 — Start the Interactive Client**

```bash
cd simulator

python3 client.py
```

The interactive client allows manual transmission of simulated CAN messages to observe ECU behavior in real time.

---

# 📂 Project Structure

```text
Smart-BCM-SiL/
├── app/                  # Application layer (State Machines & Business Logic)
├── services/             # Simplified RTE & OS abstraction services
├── hal/                  # Hardware abstraction (Mock CAN, UART, GPIO)
├── simulator/            # Python SiL test framework & reports
├── Makefile              # Build configuration
└── README.md             # Architecture and project documentation
```