## Decentralized ESP32 Communication System using ESP-NOW

### 🧠 **What I Built**

This project is a **Decentralized, fail-safe, mesh-like wireless communication system** built using **three ESP32 microcontrollers** and the **ESP-NOW** protocol.

The system is designed to reliably transmit sensor data between devices without relying on Wi-Fi routers or internet connectivity. It mimics some features of a mesh network by dynamically rerouting data when certain nodes go offline, making it resilient and suitable for remote, real-time monitoring systems in low-power environments.

---

### 📶 **Why ESP-NOW?**

**ESP-NOW** is a low-power, connectionless communication protocol developed by Espressif (the makers of ESP32), which allows devices to exchange small packets of data directly, without establishing a Wi-Fi connection or using a router.

Unlike traditional networking methods that require Wi-Fi handshakes and routers (which introduce latency and single points of failure), ESP-NOW enables direct **peer-to-peer communication** with near-instant delivery and minimal power consumption. This makes it ideal for IoT systems, especially when **speed, simplicity, and robustness** are priorities.

---

### 🧩 **System Architecture**

The system is composed of three ESP32 devices, each playing a specific role:

#### 🔹 ESP3: Sensor Node (Data Originator)

* Reads data from an **LDR sensor** (Light Dependent Resistor).
* Tries to **send the LDR value to ESP2** as the primary target.
* If **ESP2 is offline**, it reroutes the data to **ESP1** instead.
* If **both are offline**, it logs the sensor data locally and continues checking for restored connections.

#### 🔹 ESP2: Aggregator & Forwarder

* Receives data from ESP3.
* Reads its **own LDR value**.
* Combines both values and **forwards the combined data to ESP1**.
* If ESP3 is unavailable, ESP2 still sends its own LDR data to ESP1.
* Constantly monitors the network and dynamically adapts based on availability.

#### 🔹 ESP1: Final Receiver

* Receives data from either ESP2 (preferred) or ESP3.
* **Logs or processes the received data.**
* If ESP2 is offline, ESP1 listens to ESP3 directly.
* If ESP2 comes back online, ESP1 **reconnects and prioritizes it again.**

> This fallback and prioritization logic ensures the network stays functional **even if one or two nodes temporarily go offline**.

---

### 🔧 **Technologies Used**

| Component                  | Purpose                                 |
| -------------------------- | --------------------------------------- |
| **ESP32 Dev Boards (x3)**  | Core microcontrollers running the logic |
| **ESP-NOW Protocol**       | Wireless peer-to-peer communication     |
| **Arduino IDE**            | Code development and flashing           |
| **LDR sensors (x2)**       | Analog input for real-time data         |
| **C++ (Arduino Language)** | Logic programming language              |

Key ESP-NOW features used:

* `esp_now_send()` for sending data
* `esp_now_register_recv_cb()` for receiving
* `esp_now_register_send_cb()` for monitoring delivery success
* Dynamic `peer add/remove` logic to simulate a mesh-like, self-healing network

---

### 🧠 **What I Learned**

* How to use **ESP-NOW** for fast, peer-to-peer communication between ESP32s.
* How to implement **dynamic fallback logic** when a node is unreachable.
* Managing a decentralized architecture without needing a central controller.
* How to write **modular, extensible code** that allows adding more nodes in the future.
* How to track and interpret **send success/failure callbacks** in ESP-NOW.

---

### 🏗️ **Project Highlights**

* 📡 **No Router Needed** — Fully offline, local communication system.
* 🔄 **Self-Healing Logic** — Reroutes data automatically based on availability.
* 🧩 **Modular Design** — Easily scalable by adding more ESP32 nodes.
* ⚡ **Low Latency & Power** — Minimal overhead thanks to ESP-NOW.
* 📈 **Real-Time Monitoring** — Transmits live analog data (LDR readings) between devices.

---

### 🚀 **Potential Applications**

* 🔍 Remote environmental sensing
* 🏕️ Off-grid sensor networks
* 🏭 Industrial monitoring in disconnected zones
* 🌱 Smart agriculture and greenhouse tracking
* 🚧 Smart construction sites (dust, light, temp, etc.)

## **Contributors**  
👨‍💻 **Muhammad Zaigham Ali** – [LinkedIn](https://www.linkedin.com/in/zaighamali005)
