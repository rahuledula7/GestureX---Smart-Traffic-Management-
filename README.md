# GestureX---Smart-Traffic-Management-
Smart Adaptive Traffic Management for Efficient Urban Transport — SIH 2026 project by Team GestureX (PSID: SIH26222), featuring IR-based traffic density detection, adaptive signal timing, and secure face-authenticated gesture-based emergency override.

# Smart Adaptive Traffic Management for Efficient Urban Transport

## Team GestureX

**SIH 2026 | PSID: SIH26222**

## Problem Statement

Urban traffic networks face increasing pressure due to congestion, uneven traffic flow, and emergency situations. Fixed-time traffic signals can cause unnecessary delays on low-traffic roads while heavily congested roads require more efficient management.

## Our Solution

Our project proposes a smart traffic signal management system that uses real-time road occupancy information to adapt signal timings according to changing traffic conditions.

The system uses multiple IR sensors to estimate traffic density on different roads and dynamically adjusts the green-light duration.

For emergency situations, an authorized traffic police officer can temporarily control the signal using face authentication and predefined hand gestures.

The system ensures safe signal transitions through:

- Yellow-light transition
- All-Red safety interval
- Controlled direction changes
- Automatic return to adaptive mode

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
Adaptive Traffic Signal Control
    ↑
    |
Face Authentication
    ↓
Gesture Recognition
    ↓
Emergency Override