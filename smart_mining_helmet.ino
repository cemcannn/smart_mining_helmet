#include <DHT11.h>
#include <LiquidCrystal_I2C.h>
#include <MQ7.h>

#define sensor_methane A0
#define sensor_monoxide A1
#define buzzer_pin 9
DHT11 sensor_temp_hum(8);
#define led_r 5
#define led_g 6
#define led_b 7

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define preheat_time 5000
#define voltage 5
#define treshold_methane 500
#define treshold_monoxide 150

// init MQ7 device
MQ7 mq7(sensor_monoxide, voltage);

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);

  digitalWrite(led_r, HIGH);
  digitalWrite(led_g, HIGH);
  digitalWrite(led_b, HIGH);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  digitalWrite(led_b, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Isitiliyor...");
  delay(preheat_time);

  digitalWrite(led_b, HIGH);
  delay(1000);

	mq7.calibrate();	

}

void loop() {
  int sensor_value_methane = analogRead(sensor_methane);
  int sensor_value_monoxide = analogRead(sensor_monoxide);

  int sensor_value_temperature = sensor_temp_hum.readTemperature();
  int sensor_value_humidity = sensor_temp_hum.readHumidity();

  if (sensor_value_methane <= treshold_methane && sensor_value_monoxide <= treshold_monoxide) {
    for (int i = 1; i <= 4; i++) {
      switch (i)
        ;
      lcd.clear();

      switch (i) {
        case 1:
          methane();
          break;
        case 2:
          monoxide();
          break;
        case 3:
          temperature();
          break;
        case 4:
          humidity();
          break;
      }
      delay(2000);
    }
    digitalWrite(led_g, LOW),
      delay(2000);
  }

  else if (sensor_value_methane >= treshold_methane) {
    methane();
    alarm(100);
    delay(5);
  }

  else if (sensor_value_monoxide >= treshold_monoxide) {
    monoxide();
    alarm(100);
    delay(5);
  }
}

void methane() {
  int sensor_value_methane = analogRead(sensor_methane);
  lcd.setCursor(0, 0);
  lcd.print("Metan Degeri : ");
  lcd.setCursor(0, 1);
  lcd.print(sensor_value_methane);
  lcd.setCursor(13, 1);
  lcd.print("ppm");
}

void monoxide() {
  int sensor_value_monoxide = mq7.readPpm();
  lcd.setCursor(0, 0);
  lcd.print("Monoksit Degeri : ");
  lcd.setCursor(0, 1);
  lcd.print(sensor_value_monoxide);
  lcd.setCursor(13, 1);
  lcd.print("ppm");
}

void temperature() {
  int sensor_value_temperature = sensor_temp_hum.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Sicaklik Degeri : ");
  lcd.setCursor(0, 1);
  lcd.print(sensor_value_temperature);
  lcd.setCursor(10, 1);
  lcd.print("Derece");
}

void humidity() {
  int sensor_value_humidity = sensor_temp_hum.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("Nem Degeri : ");
  lcd.setCursor(0, 1);
  lcd.print("%");
  lcd.setCursor(1, 1);
  lcd.print(sensor_value_humidity);
}

void alarm(unsigned int duration) {
  tone(buzzer_pin, 440);
  digitalWrite(led_r, LOW);
  digitalWrite(led_g, HIGH);
  delay(duration);
  noTone(buzzer_pin);
  digitalWrite(led_r, HIGH);
  digitalWrite(led_g, HIGH);
  delay(duration);
}