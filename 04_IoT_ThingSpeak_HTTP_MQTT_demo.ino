// Import all of Libraries
#include <Wire.h>
#include <WiFi.h>
#include <ThingSpeak.h>
// Initialize for WiFi client
WiFiClient client;
// Initialize for ThingSpeak API
unsigned long CH_NUMBER  = xxxxx;
const char*   CH_APIkey  = "xxx";
// Initialize for Temperature Sensor (LM73-1)
#define TEMP_SDA    (4)     // LM73-1 Serial Data Line in IO4
#define TEMP_SCL    (5)     // LM73-1 Serial Clock Line in IO5
#define TEMP_ADDR   (0x4D)  // LM73-1 Slave Address in 0x4D
// LDR is GPIO 36 or SENSOR_VP in ESP-WROOM-32
#define LDR_PIN     (36)

void CONNECT_WIFI() {
  const char* WIFI_NAME = "xxx";        // Wifi's Name
  const char* WIFI_PASS = "xxx";  // Wifi's Password
  WiFi.mode( WIFI_STA );              // define WiFi mode
  WiFi.begin( WIFI_NAME, WIFI_PASS ); // connect WiFi
  // try to connect the WiFi
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(1000);
    Serial.print(".");
  }
  // connect WiFi successful
  Serial.println("");
  Serial.println("!!!!!!!!!! WiFi Connected !!!!!!!!!!");
}

// Read Temperature Sensor (LM73-1)
float READ_TEMP() {
  // use TEMP_DATA array to getting temp sensor value
  byte TEMP_DATA[2];  // getting 2 bytes 
  // start temp sensor and write its address
  Wire.beginTransmission(TEMP_ADDR);
  // write temp address
  Wire.write(0);
  Wire.endTransmission();
  // start and read temp sensor
  Wire.requestFrom(TEMP_ADDR, 2); // 2 is data 2 bytes
  TEMP_DATA[1] = Wire.read();     // getting MSB temp data
  TEMP_DATA[0] = Wire.read();     // getting LSB temp data
  // initialize TEMP_VALUES into 0.0 Celsius
  float TEMP_VALUES = 0.0;
  // condition for converting from HEX into DEC
  // convert in MSB // 0010 0000
  if ( TEMP_DATA[1] & 0x40 ) TEMP_VALUES += 128;    // bit 6
  if ( TEMP_DATA[1] & 0x20 ) TEMP_VALUES += 64;     // bit 5
  if ( TEMP_DATA[1] & 0x10 ) TEMP_VALUES += 32;     // bit 4
  if ( TEMP_DATA[1] & 0x08 ) TEMP_VALUES += 16;     // bit 3
  if ( TEMP_DATA[1] & 0x04 ) TEMP_VALUES += 8;      // bit 2
  if ( TEMP_DATA[1] & 0x02 ) TEMP_VALUES += 4;      // bit 1
  if ( TEMP_DATA[1] & 0x01 ) TEMP_VALUES += 2;      // bit 0
  // convert in LSB
  if ( TEMP_DATA[0] & 0x80 ) TEMP_VALUES += 1;      // bit 7
  if ( TEMP_DATA[0] & 0x40 ) TEMP_VALUES += 0.5;    // bit 6
  if ( TEMP_DATA[0] & 0x20 ) TEMP_VALUES += 0.25;   // bit 5
  if ( TEMP_DATA[0] & 0x10 ) TEMP_VALUES += 0.125;  // bit 4
  if ( TEMP_DATA[0] & 0x08 ) TEMP_VALUES += 0.0625; // bit 3
  if ( TEMP_DATA[0] & 0x04 ) TEMP_VALUES += 0.03125;  // bit 2
  // checking in MSB byte bit 7 is sign bit
  if ( TEMP_DATA[1] & 0x80 ) TEMP_VALUES *= (-1);
  return TEMP_VALUES;
}

int READ_LDR() {
  int LDR_VALUES = analogRead(LDR_PIN);
  return LDR_VALUES;
}

void setup() {
  // begin to use Wire lib with temp sensor address
  Wire.begin(TEMP_SDA, TEMP_SCL);
  // connect to WiFi
  CONNECT_WIFI();
  // start sending data to ThingSpeak
  ThingSpeak.begin(client);
  // set baud rate 115200
  Serial.begin(115200);
}

void loop() {
  // reading temperature
  float TEMP = READ_TEMP();
  // reading LDR
  int LDR = READ_LDR();
  // sending data to ThingSpeak
  ThingSpeak.setField( 1, TEMP );   // Field 1 for Temperature values
  ThingSpeak.setField( 2, LDR );    // Field 2 for LDR values
  // writing the both of TEMP and LDR values into ThingSpeak
  int STATUS_SENDING = ThingSpeak.writeFields(CH_NUMBER, CH_APIkey);
  // if STATUS = 200, the sent data are successfully.
  if ( STATUS_SENDING == 200 ) {
    Serial.println("++++++++++ SUCCESSFUL ++++++++++");
  } else {
    Serial.println("---------- ERROR " + String(STATUS_SENDING) + "----------");
  }
  Serial.println(TEMP);
  Serial.println(LDR);
  delay(5000);
}
