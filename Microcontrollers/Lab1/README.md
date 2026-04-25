# 🧪 Lab 1 – GPIO & 7-Segment Display

## 📌 Description

This lab introduces the **GPIO (General Purpose Input/Output)** module of a microcontroller and basic hardware interaction.

The focus is on controlling output pins and displaying values using a **7-segment display**.

---

## 🔧 Topics Covered

* GPIO configuration (input/output)
* Writing to and reading from digital pins
* Working with **hexadecimal values**
* Binary representation of data
* Controlling a **7-segment BCD display**
* Mapping numbers to segment patterns

---

## ⚙️ Key Concepts

### 🔹 GPIO (General Purpose Input/Output)

* Used to control external hardware (LEDs, displays, buttons)
* Configured as input or output pins

### 🔹 Hexadecimal Representation

* Used to represent binary values in a compact form
* Common in embedded programming for register manipulation

### 🔹 7-Segment Display

* Displays digits using 7 LEDs (segments)
* Controlled by setting GPIO pins HIGH/LOW
* Uses **BCD (Binary-Coded Decimal)** or custom segment mapping

---

## 🔢 Example Concept

Each digit on a 7-segment display corresponds to a specific binary/hex pattern.

Example:

```txt
Digit 0 → 0x3F
Digit 1 → 0x06
Digit 2 → 0x5B
```

---

## 🎯 Learning Outcome

* Understanding how software interacts with hardware
* Using GPIO to control physical devices
* Working with hexadecimal and binary values
* Implementing display logic using embedded systems

---

## ⚠️ Notes

* Requires a microcontroller board to run
* Output depends on correct wiring of the 7-segment display
