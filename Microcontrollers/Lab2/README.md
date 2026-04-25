# 🧪 Lab 2 – Ultrasonic Sensor & LED Pendulum

## 📌 Description

This lab focuses on combining **hardware interaction and timing** using a microcontroller.

The main objective is to measure distance using an **ultrasonic sensor (HC-SR04)** and display the results using an **LED pendulum module**.

---

## 🎯 Objectives

* Understand and use **general-purpose timers**
* Perform **distance measurement** using an ultrasonic sensor
* Display patterns and values on an **LED pendulum**
* Combine measurement and visualization in a single system 

---

## 🔧 Topics Covered

* GPIO configuration
* General-purpose timers
* Ultrasonic distance measurement
* Signal timing (Trigger & Echo)
* LED control and visualization
* Embedded system integration

---

## ⚙️ Laboratory Tasks

### 🔹 1. LED Pendulum

* Display vertical bars at left and right edges
* Use **software delay** for LED timing
* Detect pendulum direction using signal polling 

---

### 🔹 2. Ultrasonic Distance Measurement

* Initialize and use the **HC-SR04 sensor**
* Measure distance continuously using a timer
* Observe **Trigger and Echo signals** using an oscilloscope
* Output measured distance via console (`printf`) 

---

### 🔹 3. Display Distance on LED Pendulum

* Visualize distance as:

  * A horizontal bar
    **or**
  * A numeric value (e.g., "65 cm")
* Use one pendulum direction for measurement and the other for display 

---

## 📡 Ultrasonic Sensor (HC-SR04)

The ultrasonic module works using **sound wave reflection**:

* A signal is sent via the **Trigger pin**
* The **Echo pin** goes HIGH when the signal is received back
* The time between rising and falling edges represents travel time 

### 📏 Distance Formula

Distance is calculated using:

```id="ultra1"
Distance = (Time × Speed of Sound) / 2
```

* Division by 2 because the signal travels **to the object and back**

---

## ⏱️ LED Pendulum Behavior

* Oscillates at ~8.3 Hz
* Direction change detected via signal edges
* Used to synchronize measurement and display 

---

## 🎯 Learning Outcome

* Understanding real-time measurement using timers
* Interfacing sensors with microcontrollers
* Working with signal timing and hardware modules
* Combining input (sensor) and output (LED display)
* Designing embedded systems with multiple components

---

## ⚠️ Notes

* Requires microcontroller board and ultrasonic module
* Correct wiring is essential for accurate measurements
* Timing precision is critical for correct distance calculation

---
