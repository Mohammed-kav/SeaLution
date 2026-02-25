# SeaLution

## Overview
SeaLution is an innovative, capsule-shaped, floating device designed to collect vital marine ecosystem data from remote water bodies. 

**Award:** This project won **2nd place in the IEEEDuino 2024 competition in Region 8** (covering Asia, Europe, and Africa).

## Mission & Impact
By combining advanced environmental sensing technologies with a robust, self-powered design, SeaLution offers a reliable and efficient solution for monitoring marine ecosystems. The collected data is transmitted to a cloud-based database, contributing directly to valuable environmental research, expert analysis, and conservation efforts.

## Key Features & Technologies
* **Environmental Sensing:** * **DHT Sensor:** Measures air temperature, humidity, and atmospheric pressure.
    * **MQ Sensor:** Detects gas concentrations and pollutants in the surrounding air.
    * **DS18B20 Sensor:** Measures water temperature.
    * **TDS Sensor:** Measures the Total Dissolved Solids in the water.
* **Wind Power & Anemometry:** Features a vertical-axis wind turbine that serves as an anemometer to measure wind speed and direction. The Arduino's analog pin is protected from potential voltage spikes using a voltage divider circuit and a 5.1V Zener diode.
* **Solar Power & Energy Storage:** Completely self-sustaining. It harnesses solar energy via a solar panel, managed by an MPPT solar charger module, and stores it in a 3.7V lithium-ion battery. A DC-DC step-up converter boosts the voltage to power the system.

## Device Architecture
The hardware is structurally organized into three layers, powered by an **Arduino Uno Rev4**:

* **Top Layer:** Solar panel, vertical-axis wind turbine, DHT sensor, and MQ sensor.
* **Middle Layer:** Arduino Uno Rev4 microcontroller, MPPT solar charger, 3.7V lithium-ion battery, battery holder, DC-DC step-up converter, and a PCB-mounted voltage divider circuit.
* **Bottom Layer:** DS18B20 water temperature sensor and TDS sensor.
