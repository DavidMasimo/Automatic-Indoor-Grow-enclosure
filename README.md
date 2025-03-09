# Automatic Indoor Grow Enclosure (A.I.G.E.)

## Concept of Operations:

The Automatic Indoor Grow Enclosure (A.I.G.E.) is designed to maintain optimal conditions for plant growth. The system automates watering, temperature and humidity regulation, lighting control, and airflow management. 
It also displays essential environmental metrics in real-time.

## Key Features:

LCD Display: Shows real-time values for soil moisture, humidity, vapor pressure deficit (VPD), and temperature.

Automated Environment Control: Sensors regulate temperature, humidity, and soil moisture, activating fans and pumps as needed.

Grow Light Control: Lights turn on/off based on a pre-set schedule to simulate day and night cycles.

User Configuration: Initial setup allows users to set desired temperature, humidity, and moisture levels.

Error Handling: Alerts users when temperature and humidity levels are out of optimal range.

System Requirements

### 1) LCD Display

Backlit LCD with 4 rows of 20 characters.

Displays:

Soil moisture percentage

Temperature (°C)

Vapor Pressure Deficit (kPa)

Initial configuration prompts for temperature, humidity, and moisture

Error messages for incompatible temperature and humidity levels

Interface: UART

### 2) Temperature, Humidity, and Moisture Regulation

Sensors:

Temperature sensor (°C)

Humidity sensor (%RH)

Moisture sensor (below soil warning line)

Fan Control:

Automatically circulates air when temperature or humidity exceeds set values.

Controlled by PIC microcontroller.

Watering System:

Activates water pump when soil moisture drops below set level.

Water stored in a 1-5L container.

### 3) Plant Enclosure:

Dimensions: 30cm x 30cm x 50cm

Equipped with LED grow lights (independent from other circuits).

Includes mounts for fans, lights, sensors, and tubing.

Ventilation for air circulation.

### 4) Electronics Enclosure:

Stores logic components.

Closed, easy-to-disassemble design.

Power: 12V DC power supply.

Controls:

Power button for turning on/off

Up/Down buttons for selecting values in initial setup

OK button for confirming selections

## Hardware Design:

### Components List
imagine here[table]
## Test Procedures:
image here[table]

Final Code[Code Repository]
