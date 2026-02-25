#include <Adafruit_Sensor.h> 
#include <DHT.h> 
#include <DHT_U.h> 
#include <MQUnifiedsensor.h> 
#include "OneWire.h" 
#include "DallasTemperature.h" 
#include "AnalogUVSensor.h"

#include <WiFiS3.h>  
#include <WiFiClient.h> 
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <RTC.h>
#include <time.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>


// ********* FIREBASE CONFIG ********* //
  #define FIREBASE_PROJECT_ID "sokaba-8426f" 
  #define FIREBASE_ACCESS_TOKEN "ya29.c.c0ASRK0GbjSJlQ_Z8Xy4RUfySe6fdCP57U3784L9a3bOa4NO5awMsQKtC79d_abMlQEyhPeKtqONU__ssJA8UUExKSTayt71bAcmDV-Tfko2lH_DvXAsM_vLNIRFEsA-qokL2VQ10HpokGEn04vE8WcTceDkmgbTtszsEB1MS1gpJ8Ue9EV-d_jxeDcvGgbvQsUQ5OMDSb4X4xHFZTRVkWvAhO_Ob-NPSiA1WgbomERfoNksmHj-r1kZ4m7HhQOndc3xnAlny-rzgR0OAmu23iiTRCZWM9isobEndsAks23tsYyuAoHTHqx72YllKCWXNEcoA1aAuri869Cbl8ENcgoy3-WKFSz711Vtlp5ytjcZ1BGF9omjqj8eAH384CjztMs_VdUiSw0I6WY7k-xUxQIw3ofaZgvOXWvk6wgw64wYquBJre36Jt59V-OnWrdO80iyI_Q1zOJ8qOOZBcl0jvjzUhIr08rlp4r1RWy7lBoqJc2ye2doYkpz3Mjw9_JWs744tsoQcdtWdQZt2wwo8Qi2QszsinjR733tFZmgOwmpYjbUy7hIkstFQMU1RvbsM_br0vQfYZhRjsMnxjaf7wyd93yZ7g0w9u-4xw1jseYb216YZMnz5zzBoaavShZY582lFxarUF7Ichmzxlc9Mm2lrrqn6ho1vXYbm8S-gpOoUtRdeeWOkQ5wWruotWxikJhkYlUdbW2w0BRFWhrVFI4hktp0q0FgfwQXfS6VnJQ75_QrcMbX8SnRgSfjBdQWbYB9XFpti7-nm56qVRUU-775sjhqmuckzz0hk386lMgws_QBaI8k6U8FcYjzaRr4JMdQmgFIO27aVYM6b1czqr434xROXqB8n5i07VyXIBeZopMh5Q4hwl-YRc3g-oXaIVVV3vdUZbIsIwUjn_ng-Uofojj-gecyjgOogrtjy07-UYrze0SbVcRapIwWtbt7UBWzjOnwiSdrXfYc-gF1J-59gkMqessdthxukrsWtlWuivJI-3kw-gy_k"
  String FIRESTORE_COLLECTION_PATH = "data";
  String DOCUMENT_ID = "07-13-25";
// ********* FIREBASE CONFIG ********* //

// ********* PINS ********* //
  #define DHT_PIN 7
  #define MQ_PIN A0
  #define TDS_PIN A1
  #define DS_PIN A3
  #define FAN_PIN A5
  #define cycle 1000
  #define GPS_TX_PIN 3
  #define GPS_RX_PIN 4
  #define UV_PIN A4
// ********* PINS ********* //

// ********* LED RING ********* //
  #define Pin1 2  
  #define Pin2 6
  #define Pin3 9  
  #define Pin4 11  
  #define Pin5 12  

// ********* LED RING ********* //

// ********* LED RING CONFIG ********* //
  const int ledPins[] = {Pin1, Pin2, Pin3, Pin4, Pin5};
  const int numLeds = 5;
  bool After18 = true;
  bool Before6 = true;
// ********* LED RING CONFIG ********* //

// ********* WIFI ********* //
  const char* ssid = "Tp"; 
  const char* password = "987654321mm"; 

  int status = WL_IDLE_STATUS;

  WiFiSSLClient wifi;
  HttpClient client = HttpClient(wifi, "firestore.googleapis.com", 443);

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800, 60000);
// ********* WIFI ********* //

// ********* DHT22 ********* //
  #define DHTTYPE DHT22   
  DHT dht(DHT_PIN, DHTTYPE);
// ********* DHT22 ********* //

// ********* MQ135 ********* //
  #define Board "Arduino UNO"
  #define Voltage_Resolution 5   
  #define Sensor_Type "MQ-135"
  #define ADC_Bit_Resolution 10  
  #define RatioMQ135CleanAir 3.6  
  MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ_PIN, Sensor_Type);
// ********* MQ135 ********* //

// ********* TDS ********* //
  const float VREF = 5.0;             
  const float CALIBRATION_FACTOR = 0.5;
  const float CONVERSION_FACTOR = 133.42; 
// ********* TDS ********* //

// ********* DS ********* //
  OneWire oneWire(DS_PIN);
  DallasTemperature sensors(&oneWire);
// ********* DS ********* //

// ********* UV ********* //
  AnalogUVSensor AUV;
// ********* UV ********* //

// ********* FAN ********* //
  const float maxVoltage = 5.0;      
  const float turbineMaxVoltage = 7.2;
  const float maxWindSpeed = 15;     
  const float normalizationFactor = 10; 
// ********* FAN ********* //

// ********* GPS ********* //
  SoftwareSerial mySerial(GPS_TX_PIN, GPS_RX_PIN); 
  TinyGPSPlus gps;

  unsigned long lastReadTime = 0; 
  const unsigned long readInterval = 1000; 
  const uint8_t ubxPowerSave[] = {
    0xB5, 0x62,
    0x06, 0x3B, 
    0x08, 0x00, 
    0x02,
    0x01,
    0x00, 0x00, 
    0x00, 0x00, 
    0x00,
    0x00,
    0x00, 0x00
  };
// ********* GPS ********* //


StaticJsonDocument<200> payload;
StaticJsonDocument<200> data;
static bool dataCollected = false;

float ArrWindSpeed[5];             

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }
  payload["payload"] = payload.createNestedArray("payload"); 

  // ********* WIFI ********* //
    while (status != WL_CONNECTED) {
      Serial.print("Connecting to SSID: ");
      Serial.println(ssid);
      status = WiFi.begin(ssid, password);
      delay(10000); 
    }
  
    Serial.println("Connected to WiFi");
    wifi.setTimeout(5000); 
  // ********* WIFI ********* //

  // ********* LED RING ********* //
    for (int i = 0; i < numLeds; i++) {
      pinMode(ledPins[i], OUTPUT);
    }
  // ********* LED RING ********* //

  // ********* DHT ********* //
    dht.begin();
  // ********* DHT ********* //

  // ********* MQ ********* //
    MQ135.setRegressionMethod(1);  
    MQ135.setA(110.47);          
    MQ135.setB(-2.862);
    MQ135.init();

    float calcR0 = 0;
    for (int i = 1; i <= 10; i++) {  
      MQ135.update();
      calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    }
    MQ135.setR0(calcR0 / 10);       
  // ********* MQ ********* //

  // ********* TDS ********* //
    pinMode(TDS_PIN, INPUT);        
  // ********* TDS ********* //

  // ********* UV ********* //
    AUV.begin(UV_PIN, 5, 4095);
  // ********* UV ********* //

  // ********* DS ********* //
    sensors.begin();
  // ********* DS ********* //

  // ********* NTP ********* //
    while(!timeClient.isTimeSet()) {
    timeClient.begin();
      timeClient.update(); 
      if (!timeClient.isTimeSet()) { 
        Serial.println("Failed to sync with NTP time");
        delay(1000);
      } else {
        Serial.println("NTP time synced"); 
        break;
      }
    }
  // ********* NTP ********* // 

  // ********* RTC ********* // 
    RTC.begin();

    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime(&epochTime);
    
    RTCTime startTime(
      ptm->tm_year,  
      static_cast<Month>(ptm->tm_mon + 1),
      ptm->tm_mday,
      ptm->tm_hour,
      ptm->tm_min,
      ptm->tm_sec,
      static_cast<DayOfWeek>(ptm->tm_wday),
      SaveLight::SAVING_TIME_ACTIVE
    );

    RTC.setTime(startTime);
    Serial.println("RTC set from NTP time");
  // ********* RTC ********* //

  // ********* GPS ********* //
    mySerial.begin(9600);
    sendUBX(ubxPowerSave, sizeof(ubxPowerSave));
  // ********* GPS ********* //
}


void sendUBX(const uint8_t *msg, uint8_t len) {
  uint8_t ck_a = 0, ck_b = 0;

  for (uint8_t i = 2; i < len - 2; i++) {
    ck_a += msg[i];
    ck_b += ck_a;
  }

  for (uint8_t i = 0; i < len; i++) {
    mySerial.write(msg[i]);
  }
  mySerial.write(ck_a);
  mySerial.write(ck_b);
}
void loop() {

  RTCTime currentTime;
  RTC.getTime(currentTime);

  int seconds = currentTime.getSeconds();
  int minutes = currentTime.getMinutes();
  int hour = currentTime.getHour();
  
    
  static int lastCollectedSecond = -1;

   if ((hour >= 18 && hour <= 23 || (hour >= 0 && hour <= 6)) && After18) {
    After18 = false;
    Before6 = true;
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
  }

  if (hour >= 7 && hour < 18 && Before6) {
    Before6 = false;
    After18 = true;
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }


  if ((seconds % 10 == 0) && seconds != lastCollectedSecond) {
    lastCollectedSecond = seconds;
  
    data = {};  
    String s =   "t_" + String(hour) + "_" + String(minutes) + "_" + String(seconds);
    data["id"] = s;
    Serial.println("Data Collection" + String(s));

    // ********* GPS ********* //
      while (mySerial.available()) {
        char c = mySerial.read();
        if (gps.encode(c)) {
          if (gps.location.isValid()) {
            float latDecimal = gps.location.lat();
            float lonDecimal = gps.location.lng();

            String latString = String(latDecimal, 8);
            String lonString = String(lonDecimal, 8);

            data["latitude"] = latDecimal;
            data["longitude"] = lonDecimal;

            break;
          }
        }
      }
    // ********* GPS ********* //

    // ********* DHT22 ********* //
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      data["air_humidity_rh"] = h;
      data["air_temp_c"] = t;
      data["air_heat_index_c"] = dht.computeHeatIndex(t, h, false); 
    // ********* DHT22 ********* //

    // ********* MQ 135 ********* //
      MQ135.update();
      float ppm = MQ135.readSensor();  
      data["air_quality_ppm"] = ppm; 
    // ********* MQ 135 ********* //

    // ********* TDS ********* //
      int sensorValue = analogRead(TDS_PIN); 
      float voltage = (sensorValue / 1024.0) * VREF; 
      float tds = (voltage - CALIBRATION_FACTOR) * CONVERSION_FACTOR; 
      tds = max(tds, 0);   

      data["water_tds_ppm"] = tds;  
    // ********* TDS ********* //

    // ********* DS ********* //
      sensors.requestTemperatures();
      float tempC = sensors.getTempCByIndex(0);

      data["water_ds_c"] = tempC; 
    // ********* DS ********* //

    // ********* UV ********* // 
      int uv = analogRead(UV_PIN);
      float uvi = (uv * 5.0 * 1000.0) / 4096.0;
      data["uvi"] = uvi;
    // ********* UV ********* // 

    // ********* FAN ********* //
      int fan_value = analogRead(FAN_PIN); 
      float fan_voltage = (fan_value / 1023.0) * maxVoltage;  
      float windSpeed = (fan_voltage / turbineMaxVoltage) * maxWindSpeed; 
      data["wind_speed"] = windSpeed;
    // ********* FAN ********* //

    payload["payload"].as<JsonArray>().add(data); 
  }

  // ********* WIFI ********* //
    if (!(seconds % 20)) {

      Serial.println("Storing Data");

      String url = "/v1/projects/" + String(FIREBASE_PROJECT_ID) +
                    "/databases/(default)/documents/" +
                    String(FIRESTORE_COLLECTION_PATH) + "/" + DOCUMENT_ID;

      JsonArray array = payload["payload"].as<JsonArray>();

      bool firstField = true;

      for (JsonObject obj : array) {
        String id = String(obj["id"]);
        if (firstField) {
          url += "?updateMask.fieldPaths=" + id;
          firstField = false;
        } else {
          url += "&updateMask.fieldPaths=" + id;
        }
      }

      StaticJsonDocument<2048> doc;  
      JsonObject fields = doc.createNestedObject("fields");

      for (JsonObject obj : array) {
        String id = String(obj["id"]);

        JsonObject idMap = fields.createNestedObject(id);
        JsonObject mapValue = idMap.createNestedObject("mapValue");
        JsonObject mapFields = mapValue.createNestedObject("fields");

        mapFields["longitude"]["stringValue"] = String(obj["longitude"]);
        mapFields["latitude"]["stringValue"] = String(obj["latitude"]);
        mapFields["air_humidity_rh"]["stringValue"] = String(obj["air_humidity_rh"]);
        mapFields["air_temp_c"]["stringValue"] = String(obj["air_temp_c"]);
        mapFields["air_heat_index_c"]["stringValue"] = String(obj["air_heat_index_c"]);
        mapFields["air_quality_ppm"]["stringValue"] = String(obj["air_quality_ppm"]);
        mapFields["water_tds_ppm"]["stringValue"] = String(obj["water_tds_ppm"]);
        mapFields["water_ds_c"]["stringValue"] = String(obj["water_ds_c"]);
        mapFields["wind_speed"]["stringValue"] = String(obj["wind_speed"]);
        mapFields["uvi"]["stringValue"] = String(obj["uvi"]);

        // ************** DELETE ****************** //
          Serial.println("***************** READ ****************");
          
          Serial.print("Longitude: ");
          Serial.println(String(obj["longitude"])); 

          Serial.print("Latitude: ");
          Serial.println(String(obj["latitude"]));

          Serial.print("air_humidity_rh: ");
          Serial.println(String(obj["air_humidity_rh"]));

          Serial.print("air_temp_c: ");
          Serial.println(String(obj["air_temp_c"]));

          Serial.print("air_heat_index_c: ");
          Serial.println(String(obj["air_heat_index_c"]));

          Serial.print("air_quality_ppm: ");
          Serial.println(String(obj["air_quality_ppm"]));

          Serial.print("water_tds_ppm: ");
          Serial.println(String(obj["water_tds_ppm"]));

          Serial.print("water_ds_c: ");
          Serial.println(String(obj["water_ds_c"]));

          Serial.print("wind_speed: ");
          Serial.println(String(obj["wind_speed"]));
          
          Serial.print("uvi: ");
          Serial.println(String(obj["uvi"]));
  
          Serial.println("***************** READ ****************");
        // ************** DELETE ****************** //

      }

      String payloadReq;
      serializeJson(doc, payloadReq);

      Serial.println("Payload JSON:");
      Serial.println(payloadReq);

      client.beginRequest();
      client.patch(url);
      client.sendHeader("Authorization", "Bearer " + String(FIREBASE_ACCESS_TOKEN));
      client.sendHeader("Content-Type", "application/json");
      client.sendHeader("Content-Length", payloadReq.length());
      client.sendHeader("Connection", "close");
      client.beginBody();
      client.print(payloadReq);
      client.endRequest();

      int statusCode = client.responseStatusCode();
      String response = client.responseBody();

      Serial.print("Status code: ");
      Serial.println(statusCode);
      // Serial.print("Response: ");
      // Serial.println(response);

      while (client.available()) {
        char c = client.read();
      }
      client.stop();


      payload.clear();
      payload["payload"] = payload.createNestedArray("payload");
      data.clear();
    }

  // ********* WIFI ********* //
}
