# gesture_recognisation
# Gesture Recognition using Edge Impulse and ESP32

## Overview
This project involves training a machine learning model using Edge Impulse, deploying it on an ESP32, and recognizing gestures using WiFi provisioning.

## Steps to Run the Project

### 1. Train the Model using Edge Impulse CLI
1. Install the Edge Impulse CLI:
2. Log in to your Edge Impulse account:
3. Connect your device and start data acquisition.
4. Train the model with collected gesture data.
5. Download the model as an Arduino library or C++ source code.

### 2. Upload the Code to ESP32
1. Open the provided `ges_recog_website.ino` file in Arduino IDE.
2. Install the required libraries:
   - WiFi
   - Edge Impulse Inferencing
   - Adafruit MPU6050
3. Upload the code to your ESP32 board.

### 3. Connect WiFi using ESP BLE Provisioning App
1. Install the ESP BLE Provisioning App (available on Play Store/App Store).
2. Open the app and scan for your ESP32 device.
3. Enter the WiFi credentials and provision the ESP32.
4. The ESP32 will connect to the provided WiFi network.

### 4. Copy the IP Address from the Serial Monitor
1. Open the Arduino Serial Monitor.
2. Look for the assigned IP address.
3. Copy the IP address for further use.

### 5. Recognize the Gesture
1. Access the web interface using the copied IP address.
2. Perform the trained gesture.
3. The ESP32 will classify and display the recognized gesture.

## Project Files
- `ges_recog_website.ino`: Main Arduino code for gesture recognition.
- `output.mp4`: Sample video demonstrating the project.
- `Edge Impulse cli1.png`: Screenshot of Edge Impulse training.

## Requirements
- ESP32
- MPU6050 Accelerometer
- Edge Impulse Account
- Arduino IDE with necessary libraries

## Future Enhancements
- Improve gesture accuracy by collecting more training data.
- Add cloud integration for remote monitoring.
- Implement real-time feedback on the web interface.


