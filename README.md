# **Automatic Indoor Grow Enclosure (A.I.G.E.)** 🌱💡  

## **Overview**  
The **Automatic Indoor Grow Enclosure (A.I.G.E.)** is a **PIC18F45K20-based smart system** designed to maintain **optimal growing conditions** for plants. The system **automates irrigation, temperature and humidity regulation, lighting control, and airflow management**, ensuring a stable environment. Real-time environmental data is displayed on an LCD for monitoring.  

## **Key Features**  
✅ **LCD Display**: Shows soil moisture, humidity, vapor pressure deficit (VPD), and temperature.  
✅ **Automated Environment Control**: Sensors manage **temperature, humidity, and soil moisture**, activating fans and pumps as needed.  
✅ **Grow Light Control**: Automatically turns **on/off** based on a preset schedule to simulate day and night cycles.  
✅ **User Configuration**: Allows users to set target **temperature, humidity, and moisture levels** during initial setup.  
✅ **Error Handling**: Alerts users when environmental conditions **fall outside the optimal range**.  

---

## **System Components & Operation**  

### **1️⃣ LCD Display**  
- **Type:** 4x20 backlit LCD  
- **Interface:** UART  
- **Displays:**  
  - 🌱 **Soil Moisture (%)**  
  - 🌡️ **Temperature (°C)**  
  - 💨 **Vapor Pressure Deficit (kPa)**  
  - ⚠️ **Error Messages for abnormal temperature/humidity levels**  
  - 🔧 **Initial setup prompts for user configuration**  

### **2️⃣ Automated Environment Control**  
**Sensors:**  
- 📏 **Temperature Sensor (PT100 RTD)**  
- 🌫️ **Humidity Sensor (HIH-5030)**  
- 🌿 **Soil Moisture Sensor**  

**Fan Control:**  
- ✅ Automatically circulates air when **temperature/humidity exceed set thresholds**  
- ✅ Controlled via **PIC18F45K20 microcontroller**  

**Watering System:**  
- 🚰 **Water Pump Activation** when soil moisture drops below the threshold  
- 💧 Water stored in **1-5L reservoir**  

### **3️⃣ Grow Enclosure**  
- 📏 **Dimensions:** 30cm x 30cm x 50cm  
- 🌞 **LED Grow Lights** (Full spectrum, independent circuit)  
- 🔧 **Mounts for**: Fans, lights, sensors, and tubing  
- 🌬️ **Ventilation for air circulation**  

### **4️⃣ Electronics Enclosure**  
- 🔋 **Power Supply:** 12V DC  
- 🔧 **Components stored inside for protection**  
- 🛠️ **Controls:**  
  - Power button **(On/Off)**  
  - **Up/Down buttons** for selecting values  
  - **OK button** for confirming selections  

---

## **Future Improvements** 🛠️  
🔹 Implement **wireless monitoring** via Bluetooth/WiFi  
🔹 Upgrade to **3D-printed enclosures** for better modularity  
🔹 Add **data logging functionality** for long-term analysis  

---

## **Demo & Photos** 📸  
![Image](https://github.com/user-attachments/assets/1c878a49-f330-4732-acbd-9f602b5cbb76)
![Image](https://github.com/user-attachments/assets/d4b68184-3207-4150-8d3d-c5a41308ba97)
![Image](https://github.com/user-attachments/assets/26ee0e85-187b-4010-b048-e4ea24c17aea)
---

## **Why This Project Matters?**  
This project **demonstrates expertise in embedded systems, automation, and electronics integration**—valuable skills for **electronics engineering and IoT applications**.  

🔗 **[View Source Code](#)** *(Replace with actual link to code folder)*  

---
