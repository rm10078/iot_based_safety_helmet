void take_sensors_data(){
  mq_135_data=digitalRead(mq_135);
  mq_2_data=digitalRead(mq_2);
  mpu6050_read_data();
  bmp280_read_data();
  tem=dht.readTemperature();
   Serial.print("MQ_135=");
   Serial.println(mq_135_data);
   Serial.print("MQ_2=");
   Serial.println(mq_2_data);
  delay(10);
}

void mpu6050_read_data(){
  if(status_mpu6050){
  //Serial.println("mpu6050_ok");
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  ax=a.acceleration.x;
  ay=a.acceleration.y;
  az=a.acceleration.z;
  //m/s^2
  gx=g.gyro.x;
  gy=g.gyro.y;
  gz=g.gyro.z;
  //rad/s
  magnitude=sqrt(pow(ax,2)+pow(ay,2)+pow(az,2));
  Serial.print("Magnitude=");
  Serial.println(magnitude);
  float t=temp.temperature;
  }
}

void bmp280_read_data(){
  if(status_bmp280){
  //Serial.println("bmp280_ok");
  tem_inC=bmp.readTemperature();
  pressure_in_pa=bmp.readPressure();
  altitude=bmp.readAltitude(1013); /* Adjusted to local forecast! */
  Serial.print("altitude=");
  Serial.println(altitude);
}
}

//void make_it_json(int cd){
//  // float lpg = getGasConcentration(mq_2_data, 3.5, -0.25);      // LPG
//  // float smoke = getGasConcentration(mq_2_data, 2.3, -0.14);     // Smoke
//  // float alcohol = getGasConcentration(mq_2_data, 2.0, -0.1);    // Alcohol
//  // float propane = getGasConcentration(mq_2_data, 2.0, -0.15);    // Propane
//  // float hydrogen = getGasConcentration(mq_2_data, 4.0, -0.2);    // Hydrogen
//  // float methane = getGasConcentration(mq_2_data, 3.0, -0.1);     // Methane
//  // float co = getGasConcentration(mq_2_data, 2.0, -0.1);          // Carbon Monoxide
//  // float ammonia = getGasConcentration(mq_135_data, 2.3, -0.14); // Ammonia (NH3)
//  // float sulfur = getGasConcentration(mq_135_data, 3.6, -0.18);   // Sulfur (S)
//  // float benzene = getGasConcentration(mq_135_data, 4.0, -0.24);   // Benzene (C6H6)
//  // float co2 = getGasConcentration(mq_135_data, 1.7, -0.1);        // CO2
//
//  StaticJsonDocument<200> doc;
//  // doc["lpg"]=lpg;
//  // doc["smoke"]=smoke;
//  // doc["alcohol"]=alcohol;
//  // doc["propane"]=propane;
//  // doc["hydrogen"]=hydrogen;
//  // doc["methane"]=methane;
//  // doc["co"]=co;
//  // doc["ammonia"]=ammonia;
//  // doc["sulfur"]=sulfur;
//  // doc["benzene"]=benzene;
//  // doc["co2"]=co2;
//  doc["mq135"] = mq_135_data;
//  doc["mq2"] = mq_2_data;
//  doc["temp"] = tem_inC;
//  doc["pressure"] = pressure_in_pa;
//  doc["altitude"] = altitude;
//  JsonArray accel = doc.createNestedArray("accel");
//  accel.add(ax);
//  accel.add(ay);
//  accel.add(az);
//  // JsonArray gyro = doc.createNestedArray("gyro");
//  // gyro.add(gx);
//  // gyro.add(gy);
//  // gyro.add(gz);
//  doc["code"]=cd;
//  doc["magnitude"]=magnitude;
//  serializeJson(doc, mySerial); //for send
//  serializeJsonPretty(doc, Serial);//for check
//}
//
//
//float getGasConcentration(float rawValue, float mValue, float bValue) {
//  float voltage = rawValue * (5.0 / 1023.0); // Convert raw ADC value to voltage
//  float ratio = voltage / 5.0; // Calculate ratio of voltage to maximum voltage
//  float concentration = pow(10, ((log10(ratio) - bValue) / mValue)); // Use the equation to calculate gas concentration
//  return concentration;
//}



void send_tel_alart(String text){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = "https://api.telegram.org/bot"+token+"/sendMessage?chat_id="+chat_id+"&text="+text;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        display_write("Sending Alart",2,0,0);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        display_write("Error",2,0,0);
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
      display_write("Check WiFi",2,0,0);
    }
  }

void send_alart(int code){
 dht.begin();
 delay(500);
 for(int i=0;i<5;i++){
 hum=dht.readHumidity();
 tem=dht.readTemperature();
 delay(500);
  }
 hum=dht.readHumidity();
 tem=dht.readTemperature();
  Serial.print("tem=");
  Serial.print(tem);
  Serial.print("hum=");
  Serial.println(hum);
  if(code==2){
    String text="The+MQ2+gas+sensor+has+detected+the+presence+of+gas.";
    text+="Name=xyz_abc,ID=9999,Alt="+String(altitude)+"m,Tem="+String(tem)+"C,Hum="+String(hum)+"+%";
    send_tel_alart(text);
    send_gps();
    }
  else if(code==135){
    String text="The+MQ135+gas+sensor+has+detected+the+presence+of+gas.";
    text+="Name=xyz_abc,ID=9999,Alt="+String(altitude)+"m,Tem="+String(tem)+"C,Hum="+String(hum)+"+%";
    send_tel_alart(text);
   send_gps();
    }
  else if(code==6050){
    String text="The+MPU6050+accelerometer+has+detected+a+sudden+impact+or+accident.";
    text+="Name=xyz_abc,ID=9999,Alt="+String(altitude)+"m,Tem="+String(tem)+"C,Hum="+String(hum)+"+%";
    send_tel_alart(text);
    send_gps();
    }
  }

int sendLocation() {
      int c=0;
    while (Serial2.available() > 0 && c<1000) {
    gps.encode(Serial2.read());
    c++;
  }
  delay(500);
  c=0;
   while (Serial2.available() > 0 && c<1000) {
    gps.encode(Serial2.read());
    c++;
  } 
delay(500);
  c=0;
   while (Serial2.available() > 0 && c<1000) {
    gps.encode(Serial2.read());
    c++;
  }
delay(250);
  if (gps.location.isValid()) {
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);
    String googleMapsLink = "https://maps.google.com/?q=" + latitude + "," + longitude;
    String message = "Current location: " + googleMapsLink;
//    bot.sendMessage(CHAT_ID, message, "");
    send_tel_alart(message);
    Serial.println("Location sent: " + googleMapsLink);
    return 1;
  } else {
    Serial.println("No valid GPS data available.");
    //send_tel_alart("No valid GPS data available.");
    return 0;
  }
}

void send_gps(){
  display_write("Sending GPS",2,0,0);
  int gpsc=0;
    while((sendLocation()==0) && gpsc<5){
      delay(1000);
      gpsc++;
      }
    if(gpsc>4){
      send_tel_alart("No valid GPS data available.");
      display_write("No valid GPS",2,0,0);
      delay(1000);
      }
  }
