# 📘 Software Construction 1

This repository contains lab work completed as part of the **Software Construction 1** course at HAW Hamburg.

The course focuses on building a strong foundation in **C programming**, covering basic syntax, algorithms, mathematical computation, and structured program design.

---

## 📂 Structure

```
Software-Construction-1/
│── Lab1/
│── Lab2/
│── Lab3/
│── Lab4/
│── Lab5/
```

Each lab introduces new concepts and builds on previous knowledge.

---

## 🧪 Labs Overview

### 🔹 Lab 1 – Basics of C

Introduction to C programming and console output using `printf()`.

### 🔹 Lab 2 – Arrays & Functions

Working with arrays, functions, and basic computations such as Fibonacci, min values, and array operations.

### 🔹 Lab 3 – Mathematical Algorithms

Implementation of mathematical concepts like trigonometric functions, differentiation, and root finding (bisection method).

### 🔹 Lab 4 – Matrix Operations

Matrix creation, determinant calculation, and matrix multiplication.

### 🔹 Lab 5 – Advanced Applications

Structures, command-line input, numerical integration, and Mandelbrot set visualization.

---
## ⚙️ How to Compile & Run

Make sure you have a C compiler installed (e.g., **GCC**).

### 🔧 Compile a program

Navigate to the lab folder and compile:

```bash
gcc main.c -o program
```

### ▶️ Run the program

```bash
./program
```

---

## 📂 Example

```bash
cd Lab2
gcc main.c -o lab2
./lab2
```

---

## 🧩 Multi-file Projects (Lab 3–5)

Some labs use multiple `.c` files. Compile them like this:

```bash
gcc main.c file1.c file2.c -o program -lm
```

> `-lm` links the math library (required for functions like `sqrt`, `sin`, etc.)

---

## 🖼️ Mandelbrot Output (Lab 5)

Running the Mandelbrot program will generate:

* Console output (0s and 1s)
* A `.ppm` image file:

```bash
mandelbrot.ppm
```

You can open it with:

* Image viewers (GIMP, IrfanView)
* Or convert it:

```bash
convert mandelbrot.ppm mandelbrot.png
```

---

## ⚠️ Notes

* Ensure all `.c` and `.h` files are in the same directory when compiling
* Use `-lm` when working with math functions
* On Windows, run executables as:

```bash
program.exe
```


## 🎯 Key Skills Gained

* C programming fundamentals
* Memory management
* Algorithm design
* Mathematical computations
* Multi-file project structure
* Debugging and testing

---

## 👤 Author

**Oshini Jayaweera**
HAW Hamburg – Information Engineering
