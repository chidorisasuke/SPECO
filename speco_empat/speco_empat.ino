#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <WiFi.h>

// Definisi PIN
#define PIR_PIN 19
#define TRIG_PIN 13
#define ECHO_PIN 12
#define SOIL_MOISTURE 23
#define BUZZER_PIN 14
#define TOPLEFT 36
#define TOPRIGHT 39
#define DOWNLEFT 35
#define DOWNRIGHT 34

static const int servo_base = 25;
static const int servo_atas = 26;
static const int servo_samping = 27;

// Variabel LCD
int lcdColumns = 16;
int lcdRows = 2;

// Variabel servo
int servobase_pos = 90;
int servoatas_pos = 90;
int servosamping_pos_awal = 0;
int servosamping_pos_gerak = 100;

// Variabel ultrasonik
long duration;
float distance;

// Variabel lainnya
const int threshold = 10;

// Inisialisasi LCD I2C
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
Servo servobase;
Servo servoatas;
Servo servosamping;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi PIN
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();

  // Inisialisasi Servo
  analogSetAttenuation(ADC_11db);
  servobase.attach(servo_base);
  servoatas.attach(servo_atas);
  servosamping.attach(servo_samping);

  servobase.write(servobase_pos);
  servoatas.write(servoatas_pos);

  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Membaca jarak dari sensor ultrasonik
  aktivasi_jarak();

  // Menentukan mode berdasarkan jarak
  if (distance > 100) {
    lcd.setCursor(0, 0);
    lcd.print("Mode A: Soil");
    cek_air();
  } else if (distance > 40 && distance <= 100) {
    lcd.setCursor(0, 0);
    lcd.print("Mode B: Hama ");
    deteksi_benda();
  } else if (distance <= 40) {
    lcd.setCursor(0, 0);
    lcd.print("Mode C: Solar");
    solar_tracker();
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Invalid Data");
  }

  // Delay untuk pembaruan
  delay(200);
}

void aktivasi_jarak() {
  // Membaca jarak dari Ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) {
    Serial.println("No echo received. Check sensor alignment!");
    distance = 0.0;
    return;
  } else {
    distance = (duration / 2.0) * 0.0343; // Konversi ke cm
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

void cek_air() {
  int moisture, soil_sensor;
  soil_sensor = analogRead(SOIL_MOISTURE);
  moisture = (100 - ((soil_sensor / 4095.00) * 100));

  lcd.setCursor(0, 1);
  if (moisture < 20) {
    lcd.print("Need water!   ");
  } else {
    lcd.print("Soil OK       ");
  }

  Serial.print("Soil Moisture: ");
  Serial.print(moisture);
  Serial.println("%");
}

void deteksi_benda() {
  int pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH) {
    digitalWrite(BUZZER_PIN, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motion Detected");
    Serial.println("Motion detected!");
    servosamping.write(servosamping_pos_awal);
    delay(500);
    servosamping.write(servosamping_pos_gerak);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("No Motion     ");
    Serial.println("No motion detected!");
  }
  delay(200);
}

void solar_tracker() {
  int topleft = readFilteredADC(TOPLEFT);
  int topright = readFilteredADC(TOPRIGHT);
  int downleft = readFilteredADC(DOWNLEFT);
  int downright = readFilteredADC(DOWNRIGHT);

  int avgTop = (topleft + topright) / 2;
  int avgBottom = (downleft + downright) / 2;
  int avgRight = (topright + downright) / 2;
  int avgLeft = (topleft + downleft) / 2;

  if (abs(avgTop - avgBottom) > threshold) {
    if (avgTop > avgBottom && servoatas_pos > 0) {
      servoatas_pos--;
    } else if (avgBottom > avgTop && servoatas_pos < 180) {
      servoatas_pos++;
    }
    servoatas.write(servoatas_pos);
  }

  if (abs(avgLeft - avgRight) > threshold) {
    if (avgLeft > avgRight && servobase_pos > 0) {
      servobase_pos--;
    } else if (avgRight > avgLeft && servobase_pos < 180) {
      servobase_pos++;
    }
    servobase.write(servobase_pos);
  }

  Serial.print("Horizontal Pos: ");
  Serial.print(servobase_pos);
  Serial.print(" | Vertical Pos: ");
  Serial.println(servoatas_pos);
}

int readFilteredADC(int pin) {
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(2); // Jeda antar pembacaan
  }
  return sum / 10;
}
