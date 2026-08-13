# README 1: Arduino Master-Slave Sensor Monitoring System

## Project Overview
This project involves designing a master-slave microcontroller system using [Tinkercad](https://www.tinkercad.com/). The system utilizes two Arduino boards to monitor environmental conditions and control physical outputs based on a predefined state machine. 

## Reference Materials
Before beginning, please review the following tutorials on Arduino basics and circuit design:
*   [Short Tutorial Video](https://www.youtube.com/watch?v=BLrHTHUjPuw)
*   [Comprehensive Tutorial Playlist](https://www.youtube.com/playlist?list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP)

## Hardware Requirements
*   **2x Arduino Boards** (Configured to communicate in a Master-Slave setup)
*   **Sensor Arduino (Master):** 
    *   Photoresistor (LDR)
    *   Gas Sensor
    *   Temperature Sensor
    *   Servo Motor
    *   Piezo Buzzer
*   **Display/Control Arduino (Slave):**
    *   LCD Screen
    *   IR Sensor and Remote

## System Logic: State Machine
The system must operate strictly based on the following states and conditions:

### State 0: Standby
*   **Trigger:** System is powered on.
*   **Action:** Sensors are powered but inactive. The system waits for an activation command via the IR remote.
*   **LCD Output:** `"AWAITING RITUAL"`

### State 1: Active Monitoring
*   **Trigger:** Activation command received from the IR remote.
*   **Action:** The Master Arduino actively polls the photoresistor and gas sensor. 
*   **Controls:** The user can press a button on the IR remote to toggle the LCD display between showing ambient light levels and gas/air purity percentages.

### State 2: Gas Alert
*   **Trigger:** Gas sensor reads >130 units of volatile compounds.
*   **Action:** Overrides standard LCD telemetry. 
*   **LCD Output:** `"TOXIC PURGE"`
*   **Resolution:** The system stays in this state until gas levels drop below 130 units, returning automatically to State 1.

### State 3: Blackout Alert
*   **Trigger:** Photoresistor detects a sudden, absolute drop in ambient light.
*   **Action:** The system ignores standard IR commands until lighting is restored and logs the event.
*   **LCD Output:** `"NOCTIS PROTOCOL"`

### State 4: Temperature Emergency (Highest Priority)
*   **Trigger:** Temperature sensor registers above 45°C.
*   **Action:** This state strictly overrides States 0, 1, 2, and 3. The servo motor actuates to 180 degrees (simulating emergency venting).
*   **LCD Output:** `"COOKED"`
*   **Resolution:** Requires a manual reset command sent via the IR Remote to exit this state.

### Multi-Fault State
*   **Trigger:** A combination of State 2 and State 3 occurring simultaneously (excluding State 4).
*   **Action:** The Piezo buzzer sounds continuously.
*   **LCD Output:** `"MULTIPLE PROBLEMS DETECTED"`
*   **Resolution:** Automatically resolves to the appropriate state if the sensor readings change.
