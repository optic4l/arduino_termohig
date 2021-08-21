#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 11     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  delay(2000);
}

void onDisplay(float t, float h, float hic, int porcSuelo){
  //clear display
  display.clearDisplay();

  //outlines
  display.drawRect(1,1,63, 31, 1);
  display.drawRect(65,1,63, 31, 1);
  display.drawRect(1,33,63, 31, 1);
  display.drawRect(65,33,63, 31, 1);

  // display temperature
  display.setTextSize(1);
  display.setCursor(3,3);
  display.print("Temp");
  display.setTextSize(2);
  display.setCursor(2,13);
  display.print(t);
  display.setCursor(49,3);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(1);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(3, 35);
  display.print("Hum");
  display.setTextSize(2);
  display.setCursor(3, 48);
  display.print(h);
  display.setCursor(49,35);
  display.setTextSize(1);
  display.print(" %");

  //display heat index
  display.setTextSize(1);
  display.setCursor(68,3);
  display.print("Heat Ind");
  display.setCursor(68,15);
  display.setTextSize(2);
  display.print(hic);

  //display moisture soil
  display.setTextSize(1);
  display.setCursor(68,35);
  display.print("Soil");
  display.setCursor(68,48);
  display.setTextSize(2);
  display.print(porcSuelo);


  
  
  display.display();
}

void loop() {

  //read temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float hic = dht.computeHeatIndex(t, h, false);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  int humedadSuelo = analogRead(A0);
  int porcSuelo = map(humedadSuelo, 1023, 0, 0, 100);
 
  onDisplay(t, h, hic, porcSuelo);
   
  delay(5000);
}