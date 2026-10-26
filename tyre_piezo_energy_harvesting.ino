#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// Voltage sensor input
const int sensorPin = A0;

// Example voltage divider:
// R1 = 100k ohm
// R2 = 10k ohm
const float R1 = 100000.0;
const float R2 = 10000.0;

// Approximate Arduino ADC reference.
// Actual reference may differ from 5.0 V.
const float ADC_REFERENCE = 5.0;

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

float readPinVoltage() {
  long total = 0;

  for (int i = 0; i < 16; i++) {
    total += analogRead(sensorPin);
    delay(2);
  }

  float averageADC = total / 16.0;

  return averageADC * ADC_REFERENCE / 1023.0;
}

void setup() {
  Serial.begin(9600);
  analogReference(DEFAULT);

  Wire.begin();

  Serial.println("Piezo Energy Harvesting");
  Serial.println("Capacitor Voltage Monitor");

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS)) {
    Serial.println("OLED initialization failed!");
    Serial.println("Check OLED interface and address.");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("PIEZO ENERGY");
  display.println("HARVESTING");
  display.println();
  display.println("Voltage Monitor");
  display.display();

  delay(1500);
}

void loop() {
  // Voltage at Arduino A0
  float pinVoltage = readPinVoltage();

  // Estimate voltage at the capacitor
  float capacitorVoltage =
    pinVoltage * (R1 + R2) / R2;

  // Serial Monitor output
  Serial.print("A0 Voltage: ");
  Serial.print(pinVoltage, 3);
  Serial.print(" V | Capacitor: ");
  Serial.print(capacitorVoltage, 2);
  Serial.println(" V");

  // OLED output
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("PIEZO ENERGY");

  display.drawLine(0, 12, 127, 12,
                   SSD1306_WHITE);

  display.setCursor(0, 22);
  display.println("Stored Voltage:");

  display.setTextSize(2);
  display.setCursor(0, 36);
  display.print(capacitorVoltage, 2);
  display.println(" V");

  display.display();

  delay(300);
}
