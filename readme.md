# 🔥 Embedded Systems Project
**Module Atmega128**

This project was developed as part of a university coursework, focusing on embedded systems programming and hardware-software integration. It demonstrates advanced use of the **ATmega128 microcontroller** to control a motor with encoder feedback, a potentiometer for speed adjustment, a 4-digit 7-segment display module, and buttons for user input. The project leverages both C and Assembly languages to achieve optimized performance and precise control.

---

## 🌟 **Features**

- **Motor Control**: Implements precise speed and direction control using encoder feedback.
- **Speed Adjustment**: Uses a potentiometer to dynamically adjust motor speed.
- **7-Segment Display**: Displays real-time motor speed and system status on a 4-digit 7-segment display.
- **User Interaction**: Buttons for start, stop, and mode selection.

### **Technical Highlights**

- Developed for the **ATmega128** microcontroller, demonstrating its advanced features such as timer/counter, ADC, and interrupt handling.
- Real-time task management and optimized performance through a combination of C and Assembly programming.
- Seamless integration of hardware components for synchronized operation.

---

## 📚 **Code Overview**

### **C Code**
- **Initialization**: Sets up the microcontroller peripherals, including ADC for potentiometer input, timers for PWM generation, and interrupts for encoder handling.
- **Main Loop**:
  - Reads potentiometer input to determine desired motor speed.
  - Updates the PWM signal to control motor speed dynamically.
  - Displays the current speed on the 7-segment display.
  - Handles button presses for mode selection and system control.

### **Assembly Code**
- Handles low-level, time-critical tasks such as:
  - Encoder pulse counting for precise speed measurement.
  - Direct control of 7-segment display multiplexing.
  - Efficient interrupt service routines to minimize latency.

---

## 🚀 **How It Works**

1. **Motor Control**:
   - The motor's speed is adjusted in real-time using a potentiometer connected to the ADC of the ATmega128.
   - Feedback from the motor's encoder is processed to ensure accurate speed regulation.

2. **7-Segment Display**:
   - Displays the current motor speed and operational status.
   - The display is updated dynamically through multiplexing controlled by the microcontroller.

3. **Buttons**:
   - Allow the user to start, stop, and switch between operational modes.

---

## 🎯 **Applications**

This project showcases principles and techniques applicable to various fields, including:
- Robotics and automation systems requiring precise motor control.
- Real-time embedded systems for industrial machinery.
- Educational demonstrations of embedded system design and microcontroller programming.

---

## 💡 **Learning Outcomes**

Through this project, students gained hands-on experience with:
- Programming in both C and Assembly for embedded systems.
- Interfacing with hardware components such as motors, encoders, and 7-segment displays.
- Utilizing the advanced features of the ATmega128 microcontroller.
- Designing and debugging real-time systems.

---

## Photos

![20230918_212841](https://github.com/user-attachments/assets/8106f324-1d1f-461d-97e1-59b748532ce4)
![20230918_212850](https://github.com/user-attachments/assets/38a02375-8179-480d-96c5-74798aa93bd9)

## Github Link

[Embedded_Systems](https://github.com/manoper93/Embedded_Systems)
