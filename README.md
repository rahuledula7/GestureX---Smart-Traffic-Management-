# Smart Adaptive Traffic Management for Efficient Urban Transport

## Team GestureX

**Smart India Hackathon 2026**  
**PSID:** SIH26222  
**Category:** Student Innovation – Hardware

---

## Problem Statement

Urban traffic networks face increasing pressure due to congestion, uneven traffic flow, and emergency situations. Fixed-time traffic signals can cause unnecessary delays on low-traffic roads while heavily congested roads require more efficient management.

This creates a need for safer and more responsive traffic management to improve urban mobility and reduce pressure on transport networks.

---

## Proposed Solution

Our project is a smart traffic signal management system that uses real-time road occupancy information to adapt signal timings according to changing traffic conditions.

Multiple IR sensors detect vehicle presence and estimate traffic density on different roads. Based on the detected traffic density, the system dynamically adjusts the green-light duration.

For emergency situations, an authorized traffic police officer can temporarily control the traffic signal using face authentication and predefined hand gestures.

After the emergency override, the system automatically returns to adaptive traffic management.

---

## Key Features

- Real-time traffic density detection using IR sensors
- Adaptive traffic signal timing
- Two-Arduino architecture
- I²C communication between Arduinos
- Face-authenticated emergency access
- Gesture-based traffic signal override
- Yellow-light safety transition
- All-Red safety interval
- Automatic return to adaptive mode

---

## System Architecture

```text
        IR Sensors
             ↓
       Arduino 2
             ↓
      I²C Communication
             ↓
       Arduino 1
             ↓
   Adaptive Traffic Control
             ↑
             |
   Face Authentication
             ↓
    Gesture Recognition
             ↓
     Emergency Override