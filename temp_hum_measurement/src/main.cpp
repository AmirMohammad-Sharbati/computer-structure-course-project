#include "DHT.h" // DHT sensor library
#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // Library for I2C LCD
#include <Wire.h>              // For I2C communication

#define DHTPIN 2      // Pin where the DHT22 is connected
#define DHTTYPE DHT22 // DHT22 sensor type
#define LEDPIN 8      // Pin where LED is connected
#define SWITCHPIN 3   // Pin where switch connected

DHT dht(DHTPIN, DHTTYPE);           // Initialize the DHT sensor
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize I2C LCD at address 0x27, 16 columns and 2 rows

bool showTemp = true; // Show status, true for temp and false for humidity
bool showTemp_warning = false;
unsigned long lastSwitchTime = 0; // last time of chagne
unsigned long lastSwitchTime_warning = 0;
const unsigned long interval = 1000; // time interval of change status
const unsigned long interval_warning = 4000;

const int dataPoints = 15;      // Number of data points for prediction
float tempData[dataPoints];     // Array to store temperature data
float humidityData[dataPoints]; // Array for humidity
unsigned long lastDataCollection = 0;
int currentDataIndex = 0;

float predictQuadratic(float y[]);

void setup() {
    lcd.init();                       // LCD initialization
    lcd.backlight();                  // Turn on the LCD backlight
    dht.begin();                      // dht sensor starting
    pinMode(LEDPIN, OUTPUT);          // set the LED as output
    pinMode(SWITCHPIN, INPUT_PULLUP); // set the switch
    lcd.print("Initializing...");     // first message on LCD
    delay(2000);                      // Pauses the program execution for 2000 milliseconds.
    Serial.begin(9600);               // Initialize serial communication at a baud rate of 9600
}

void collectData(float temp, float humidity) {
    if (millis() - lastDataCollection >= interval) {
        tempData[currentDataIndex] = temp;
        humidityData[currentDataIndex] = humidity;
        currentDataIndex = (currentDataIndex + 1) % dataPoints; // Keep rotating the data
        lastDataCollection = millis();
    }
}

void loop() {
    // get temp and humidity data from environment
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // check the correctness of data
    if (isnan(humidity) || isnan(temperature)) {
        lcd.clear();
        lcd.print("Sensor Error");
        delay(2000);
        return;
    }

    collectData(temperature, humidity);

    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);

    // Switch pressed: enter prediction mode
    if (digitalRead(SWITCHPIN) == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Predicting...");

        // Fetch past data (collected over time)
        float predictedTemp = predictQuadratic(tempData);
        float predictedHum = predictQuadratic(humidityData);

        if (millis() - lastSwitchTime >= interval) {
            showTemp = !showTemp;      // change status
            lastSwitchTime = millis(); // update the status time
        }

        lcd.clear();

        if (showTemp) {
            lcd.setCursor(0, 0);
            lcd.print("P_Temp: ");
            lcd.print(predictedTemp);
            lcd.print(" C");
        } else {
            lcd.setCursor(0, 0);
            lcd.print("P_Hum: ");
            lcd.print(predictedHum);
            lcd.print("%");
        }
        delay(1000);
        return;
    }

    // check some conditions for warning
    if (Serial.available()) {
        String warning = Serial.readStringUntil('\n'); // Read the warning from Python
        if (warning == "WARNING") {
            if (millis() - lastSwitchTime_warning >= interval) { // shows message just one second
                showTemp_warning = false;
            }
            if (millis() - lastSwitchTime_warning >= interval_warning) {
                lastSwitchTime_warning = millis();
                showTemp_warning = true;
            }
        }
    }
    else {
        digitalWrite(LEDPIN, LOW); // LED will turn off
        showTemp_warning = false;
    }

    // chagne the status each one second interval
    if (millis() - lastSwitchTime >= interval) {
        showTemp = !showTemp;      // change status
        lastSwitchTime = millis(); // update the status time
    }

    lcd.clear();
    if (showTemp_warning) {         // check the warning
        digitalWrite(LEDPIN, HIGH); // turn on the LED
        lcd.setCursor(0, 1);
        lcd.print("Warning! Temp>50");
        Serial.println("Warning received: High Temp!");
    }

    if (showTemp) {
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print(" C");
    }

    else {
        lcd.setCursor(0, 0);
        lcd.print("Humidity: ");
        lcd.print(humidity);
        lcd.print("%");
    }

    delay(1000); // delay for fix the LCD showing
}

float predictQuadratic(float y[]) {
    unsigned long futureX = 30.0;
    // calculation of coefficients => a*x^2 + b*x + c
    float x1 = 3, x2 = 8, x3 = 13;
    float z[3];

    for (int j = 0; j < 3; j++) {
        int sum = 0;
        for (int i = 5 * j; i < 5 * j + 5; i++)
            sum += y[(currentDataIndex + i) % dataPoints];
        z[j] = sum / 5;
    }

    float y1 = z[0], y2 = z[1], y3 = z[2];

    float denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
    float a = (y1 * (x2 - x3) + y2 * (x3 - x1) + y3 * (x1 - x2)) / denom;
    float b = (y1 * (x3 * x3 - x2 * x2) + y2 * (x1 * x1 - x3 * x3) + y3 * (x2 * x2 - x1 * x1)) / denom;
    float c = (y1 * (x2 * x3 * (x2 - x3)) + y2 * (x3 * x1 * (x3 - x1)) + y3 * (x1 * x2 * (x1 - x2))) / denom;

    // prediction
    return (a * futureX * futureX) + (b * futureX) + c;
}