# IoT Gas Monitoring and Prediction

An ESP8266 + MQ gas sensor system that uploads readings to ThingSpeak every 15 seconds, with a Python Linear Regression model that forecasts future gas levels from the exported data.

## How It Works

```
MQ gas sensor -> ESP8266 (Wi-Fi) -> ThingSpeak cloud -> CSV export -> Linear Regression (Python) -> forecast
```

1. The MQ sensor outputs an analog voltage that rises with gas concentration.
2. The ESP8266 reads it on pin `A0` and posts it to ThingSpeak (`field1`) every 15 s, the free-tier limit.
3. Readings are exported from ThingSpeak as CSV.
4. `predict_gas.py` fits Linear Regression on elapsed time and forecasts the next readings.

## Repository Contents

| File | Description |
|------|-------------|
| `gas_monitoring.ino` | Arduino sketch: reads the sensor and uploads to ThingSpeak |
| `gas_data.csv` | 60 readings collected on 3 Dec 2025 |
| `predict_gas.py` | Linear Regression training, evaluation and forecast |
| `gas_forecast.png` | Output chart: actual readings, linear fit, forecast |

## Hardware

- ESP8266 (NodeMCU)
- MQ gas sensor (VCC to power, GND to ground, analog out to `A0`)
- Wi-Fi hotspot and a free ThingSpeak account

## Setup

**1. Upload the sketch**
- Open `gas_monitoring.ino` in Arduino IDE (with ESP8266 board support installed).
- Replace `YOUR_THINGSPEAK_WRITE_API_KEY`, `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD`.
- Select your COM port, upload, and open the Serial Monitor at 9600 baud.
- Ignore the first ~20 seconds of readings while the sensor settles.

**2. Run the prediction**
```bash
pip install pandas numpy scikit-learn matplotlib
python predict_gas.py
```

## Results

- Time-ordered 80/20 split (48 train, 12 test)
- Test MAE: about 46 sensor units (11.7% MAPE)
- The linear trend is about -0.18 units per second, mostly the sensor settling after warm-up

The dataset includes spikes from a perfume spray used to trigger the sensor, which pull the fit away from the underlying trend.

## Limitations and Next Steps

- A straight line can't capture the curved decay of the readings. Exponential or polynomial models would fit better.
- Short run (60 readings, about 35 minutes). More data across different conditions would give a fairer test.
- Raw analog values are not calibrated to ppm.
- Possible additions: DHT temperature/humidity sensor, threshold alerts, a live dashboard.

## Author

Tadepalli Haneesh, B.Tech ECE (IoT), SR Gudlavalleru Engineering College
