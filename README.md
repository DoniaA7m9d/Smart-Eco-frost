
# 🌾 Smart Eco-Frost & Fungus Alert System for Open-Field Agriculture

An intelligent, low-power IoT solution designed to protect open-field crops from devastating winter frost and humidity-induced fungal diseases. Built using the **ESP32**, **DHT11**, **Blynk**, and **Telegram API**, this system utilizes Edge Computing concepts to process environmental data locally, optimize battery life, and deliver eco-friendly recommendations directly to farmers.

---

##  Key Features

* **Intelligent Frost Prediction:** Alerts the farmer via Telegram when the temperature drops to or below 4°C, utilizing mathematical calculations of the **Dew Point** to measure absolute frost formation risk.
* **Fungal Disease Predictor:** Monitors consecutive high humidity (>85%) and moderate temperature windows (15°C–25°C) to alert the farmer about potential fungal risks (e.g., Powdery Mildew) before visible symptoms appear.
* **Chilling Hours Counter:** Automatically tracks accumulated hours where temperatures fall below 7°C, which is essential for tracking dormancy completion in fruit crops.
* **Eco-Friendly Recommendations:** Instead of traditional air-polluting methods (like burning agricultural waste), the system sends automated green recommendations (e.g., smart drip irrigation timing, organic potassium/silica shielding).
* **Ultra Low-Power Architecture:** Implements ESP32 **Deep Sleep Mode** combined with **RTC Memory Storage** (`RTC_DATA_ATTR`) to preserve counters, allowing the system to run on battery power for months.
* **Dual Dashboard Integration:** Real-time monitoring using the **Blynk Mobile App** and comprehensive analytical graphing via the **Blynk Web Dashboard**.

---

##  Hardware Requirements

* **ESP32** NodeMCU Development Board
* **DHT11** Temperature & Humidity Sensor
* 5V Power Bank or 18650 Li-ion Battery Setup
* Jumper Wires & Waterproof Enclosure (IP65 recommended for field deployment)

### Hardware Wiring

| DHT11 Pin | ESP32 Pin | Description |
| :--- | :--- | :--- |
| **VCC (+)** | **3.3V** | Power Supply |
| **GND (-)** | **GND** | Ground |
| **Data (Out)**| **GPIO4 (D4)** | Digital Signal (RTC-enabled pin) |

---

##  Software Architecture & Edge Logic

The system utilizes an internal **Finite State Machine (FSM)** inside the `setup()` loop to maximize energy saving:

1.  **Wake-up:** ESP32 wakes up from Deep Sleep via the internal RTC Timer.
2.  **Read & Process:** Reads DHT11 sensor data and calculates the **Dew Point** using the formula:
    $$T_d = T - \left(\frac{100 - RH}{5}\right)$$
3.  **Analyze Risk:** Checks thresholds for frost or long-term high humidity.
4.  **Transmit:** Streams data via Virtual Pins to **Blynk** cloud and sends urgent markdown-formatted warnings via the **Telegram Bot API**.
5.  **Sleep:** Deep sleeps for 15 minutes (or 5 minutes if a frost emergency is currently active).

---

##  Setup and Installation

1.  **Clone the Repository:**
    ```
    git clone https://github.com/DoniaA7m9d/Smart-Eco-frost.git
    ```
2.  **Library Dependencies:** Install the following libraries via the Arduino IDE Library Manager:
    * `WiFi.h` & `WiFiClientSecure.h`
    * `BlynkSimpleEsp32.h`
    * `UniversalTelegramBot.h`
    * `DHT.h` (Adafruit)
3.  **Configuration:** Update the placeholder credentials in the `.ino` file with your specific network info:
    ```cpp
    char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
    char ssid[] = "YOUR_WIFI_SSID";
    char pass[] = "YOUR_WIFI_PASSWORD";
    #define BOTtoken "YOUR_TELEGRAM_BOT_TOKEN"
    #define CHAT_ID "YOUR_TELEGRAM_CHAT_ID"
    ```
4.  **Blynk Datastreams Configuration:**
    * **V1:** Temperature (°C)
    * **V2:** Humidity (%)
    * **V3:** Dew Point (°C)
    * **V4:** Chilling Hours Counter (Integer)

---

##  Sustainability & Impact
By substituting open combustion practices with precise, preventative, microclimate alerts, this project aligns with **Sustainable Development Goals (SDGs)** for responsible climate action and sustainable agriculture.

---

##  License
This project is open-source 
