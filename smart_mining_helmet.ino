#include <DHT11.h>              //Isı ve nem sensörü kütüphanesi
#include <LiquidCrystal_I2C.h>  //LCD ekran kütüphanesi

//Sensör pinlerimizi tanımlıyoruz.
#define sensor_methan A0
#define sensor_monoxide A1
#define buzzer_pin 9
DHT11 sensor_temp_hum(8);

//LCD ekranımızın çıkış pinlerini tanımlıyoruz.
LiquidCrystal_I2C lcd(0x27, 20, 4);

//RGB LED'imizin çıkış pinlerini tanımlıyoruz.
#define led_r 5
#define led_g 6
#define led_b 7

//Sensörlerin çalışması için gerekli ön ısıtma süresini 5sn olarak belirliyoruz.
#define preheat_time 5000

//Alarmın çalması için gerekli eşik değerlerini belirliyoruz.
#define tresholdMethan 350     //Metan eşik değeri 400
#define thresholdMonoxide 150  //Karbonmonoksit eşik değeri 150

void setup() {
  //Alarm için kullanacağımız buzzer ve LED'leri çıkış olarak tanımlıyoruz.
  pinMode(buzzer_pin, OUTPUT);
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);

  //Varsayılan olarak LED'in sönük kalmasını sağlıyoruz.
  digitalWrite(led_r, HIGH);
  digitalWrite(led_g, HIGH);
  digitalWrite(led_b, HIGH);

  //Sensör değerini seri monitörden görebilmemiz için seri haberleşmeyi başlatıyoruz.
  Serial.begin(9600);

  //İlk 5sn boyunca sensörün ısınmasını bekliyoruz. Bu esnada LED mavi renkte yanıyor.
  Serial.println("Sensor isitiliyor...");
  digitalWrite(led_b, LOW);
  delay(preheat_time);

  //Isıma işlemi için gereken 5sn süre tamamlandığında mavi LED'i söndürüyoruz.
  digitalWrite(led_b, HIGH);
  delay(1000);

  //LCD ekranımızı başlatıyoruz.
  lcd.init();
  //LCD ekranımızın arka ışığını yakıyoruz.
  lcd.backlight();
}

void loop() {
  //analogRead() fonksiyonu ile sensör değerini ölçüyoruz ve bir değişken içerisinde tutuyoruz.
  int sensorValueMethan = analogRead(sensor_methan);
  int sensorValueMonoxide = analogRead(sensor_monoxide);

  //Isı ve nem sensörü ile değerleri ölçüp bir değişkene atıyoruz.
  int sensorValueTemperature = sensor_temp_hum.readTemperature();
  int sensorValueHumidity = sensor_temp_hum.readHumidity();

//Verilerin ekrana yazdırılması
  //Monoksit ve metan değerlerinin sınır değer altında olduğu durum.
  if (sensorValueMonoxide <= thresholdMonoxide && sensorValueMethan <= tresholdMethan) {
    for (int i = 1; i <= 4; i++) {
      switch (i)
        ;
      lcd.clear();

      switch (i) {
        case 1:
          methan();
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
    // //Alarmın çalmadığı durumda LED'in yeşil yanmasını istiyoruz:
    digitalWrite(led_g, LOW);
    delay(2000);
  }

  //Metan değerinin sınır değer üstünde olduğu durum.
  else if (sensorValueMethan >= thresholdMethan) {
    methan();
    //Sensör değeri belirlediğimiz eşik değerinden yüksek ise alarm() fonksiyonunu çağırıyoruz:
    alarm(100);
    delay(5);
  }

  //Monoksit değerinin sınır değer üstünde olduğu durum.
  else if (sensorValueMonoxide >= thresholdMonoxide) {
    monoxide();
    //Sensör değeri belirlediğimiz eşik değerinden yüksek ise alarm() fonksiyonunu çağırıyoruz:
    alarm(100);
    delay(5);
  }

  else {
    
  }
}
void methan() {
  int sensorValueMethan = analogRead(sensor_methan);
  Serial.print("Metan Değeri (ppm): ");
  Serial.println(sensorValueMethan);
  lcd.setCursor(0, 0);
  lcd.print("Metan Degeri :");
  lcd.setCursor(0, 1);
  lcd.print(sensorValueMethan);
  lcd.setCursor(13, 1);
  lcd.print("ppm");
}

void monoxide() {
  int sensorValueMonoxide = analogRead(sensor_monoxide);
  Serial.print("Karbonmonoksit Değeri (ppm): ");
  Serial.println(sensorValueMonoxide);
  lcd.setCursor(0, 0);
  lcd.print("Monoksit Degeri :");
  lcd.setCursor(0, 1);
  lcd.print(sensorValueMonoxide);
  lcd.setCursor(13, 1);
  lcd.print("ppm");
}

void temperature() {
  int sensorValueTemperature = sensor_temp_hum.readTemperature();
  Serial.print("Sıcaklık Değeri (°C): ");
  Serial.println((float)sensorValueTemperature);
  lcd.setCursor(0, 0);
  lcd.print("Sicaklik Degeri :");
  lcd.setCursor(0, 1);
  lcd.print((float)sensorValueTemperature);
  lcd.setCursor(10, 1);
  lcd.print("Derece");
}

void humidity() {
  int sensorValueHumidity = sensor_temp_hum.readHumidity();
  Serial.print("Nem Değeri (%): ");
  Serial.println((float)sensorValueHumidity);
  lcd.setCursor(0, 0);
  lcd.print("Nem Degeri :");
  lcd.setCursor(0, 1);
  lcd.print("%");
  lcd.setCursor(1, 1);
  lcd.print((float)sensorValueHumidity);
}

//Alarm fonksiyonumuzu tanımlıyoruz. Bu fonksiyon parametre olarak buzzerın ötüp ve LED'in yanıp söneceği süreyi almakta.
void alarm(unsigned int duration) {
  //Buzzer'ın 440Hz'te (la notası) ses üretmesini istiyoruz:
  tone(buzzer_pin, 440);
  //Normal durumda yeşil yanmakta olan LED'i söndürüp kırmızı renkte yakıyoruz:
  digitalWrite(led_r, LOW);
  digitalWrite(led_g, HIGH);
  delay(duration);
  noTone(buzzer_pin);
  digitalWrite(led_r, HIGH);
  digitalWrite(led_g, HIGH);
  delay(duration);
}