#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 8;
const int buzzerPin = 7;

// Initialize I2C LCD (Address 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Distance Monitor");
  lcd.setCursor(0, 1);
  lcd.print("System Ready    ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a 10 microsecond pulse to trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin (travel time of sound wave in microseconds)
  long duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters (Speed of sound is 0.034 cm/us, divided by 2 for round trip)
  float distance = duration * 0.034 / 2;

  // Print results to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Display data on LCD Screen
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance, 1);
  lcd.print(" cm     "); // Clear trailing characters

  lcd.setCursor(0, 1);
  
  // Proximity Alarm Logic: Trigger alert if an object is closer than 50 cm
  // (You can change 50 to any threshold distance you prefer)
  if (distance > 0 && distance < 50) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000); // 1000Hz alert tone
    lcd.print("STATUS: WARNING!");
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    lcd.print("STATUS: CLEAR   ");
  }

  delay(500);
}