#include <Wire.h>
#include <ArduinoJson.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_I2C_ADDR 0x3C // or 0x3C
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RST_PIN -1      // Reset pin (-1 if not available)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST_PIN);

#define DHTTYPE DHT11

const byte rxPin = 16;
const byte txPin = 17;
int p_count=0;
// Set up a new SoftwareSerial object
//EspSoftwareSerial::UART mySerial(rxPin, txPin);
Adafruit_BMP280 bmp; // I2C
Adafruit_MPU6050 mpu;

TinyGPSPlus gps;

const String token="7177429160:AAHX4fFwza3O0yxvqQjSsUbdifWc8LkNZUA";    //tonken for telegram bot
String chat_id="6087838277";  //your chat id

const char* ssid     = "Rajib Manna"; // Change this to your WiFi SSID
const char* password = "rajib@7224"; // Change this to your WiFi password

//------------------------------------------pin_declearation-------------------------------------------
uint8_t mq_135=18,mq_2=19,cancel_button=26,buzzer=25,dht_11=27;
unsigned status_bmp280=0,status_mpu6050=0;
DHT dht(dht_11, DHTTYPE);
//---------------------------------------------end------------------------------------------------
//-----------------------------------------------threshold value---------------------------
int mpu6050_thre=25,temp_thre=60,set_time=5000;
//------------------------------------------------end--------------------------------------

//--------------------------------------------------------sensor local data declearation---------------------
float magnitude=0;
int mq_135_data=0,mq_2_data=0;
  float ax=0,ay=0,az=0;
  float gx=0,gy=0,gz=0;
  float tem_inC=0,pressure_in_pa=0,altitude=0,tem=0,hum=0;
//---------------------------------------------------------------end--------------------------------------
void setup() {
Serial.begin(115200);
Serial2.begin(9600, SERIAL_8N1,rxPin, txPin);
dht.begin();
display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);
display.clearDisplay();
display.display();
status_bmp280 = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
if(status_bmp280){
bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */}
status_mpu6050=mpu.begin();
if(status_mpu6050){
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

pinMode(mq_135,INPUT);
pinMode(mq_2,INPUT);
pinMode(buzzer,OUTPUT);
pinMode(cancel_button,INPUT_PULLUP);
delay(2000);
while(digitalRead(mq_135)==0 || digitalRead(mq_2)==0){
  digitalWrite(buzzer,HIGH);
  delay(20);
  digitalWrite(buzzer,LOW);
  delay(2000);
}

WiFi.begin(ssid, password);
  display_write("Connecting wifi....",2,0,0);
  while (WiFi.status() != WL_CONNECTED) {
  digitalWrite(buzzer,HIGH);
  delay(250);
  digitalWrite(buzzer,LOW);
  delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
display_write("Connected",2,0,0);
digitalWrite(buzzer,HIGH);
delay(500);
digitalWrite(buzzer,LOW);
display_write("Ready to  use",2,0,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //mq_135_thre=400,mq_2_thre=200,mpu6050_thre=20;
  if(p_count>50){
    p_count=0;
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);
  float p=pressure_in_pa/1000;// Start at top-left corner
  display.println(String(p)+" kPa");
  display.setCursor(0,21);             // Start at top-left corner
  display.println(String(altitude)+" m");
  display.setCursor(0,43);             // Start at top-left corner
  display.println(String(tem)+" C");
  display.display();
    }
  take_sensors_data();
  if(mpu6050_thre<magnitude){
    alart_display(6050);
    delay(250);
    int pre=millis();
    int current=millis();
    while((current-pre)<set_time){
      current=millis();
    digitalWrite(buzzer,HIGH);
    delay(5);
    digitalWrite(buzzer,LOW);
    delay(3);
    if(digitalRead(cancel_button)==0){
      break;
    }
    }
    if((current-pre)>=set_time){
      //make_it_json(6050);-------------------------------------------------------
      send_alart(6050);
      }
  }
  else if(mq_2_data==0){
     alart_display(2);
    int pre=millis();
    int current=millis();
    while((current-pre)<set_time){
      current=millis();
    digitalWrite(buzzer,HIGH);
    delay(5);
    digitalWrite(buzzer,LOW);
    delay(3);
    if(digitalRead(cancel_button)==0){
      break;
    }
    }
    if((current-pre)>=set_time){
      //make_it_json(2);---------------------------------------------------------
     
      send_alart(2);
      }

  }
  else if(mq_135_data==0){
    alart_display(135);
    int pre=millis();
    int current=millis();
    while((current-pre)<set_time){
      current=millis();
    digitalWrite(buzzer,HIGH);
    delay(5);
    digitalWrite(buzzer,LOW);
    delay(3);
    if(digitalRead(cancel_button)==0){
      pre=current;
      break;
    }
    }
    if((current-pre)>=set_time){
      //make_it_json(135);-------------------------------
      send_alart(135);
      }
  }
  //make_it_json(0);//only for debug
p_count++;
}
