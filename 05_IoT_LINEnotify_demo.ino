// all libraries
#include <Wire.h>
#include <WiFi.h>
#include <TridentTD_LineNotify.h>
// switch1 pin for starting and resetting to read temperature
#define SW1_PIN     (16)
// initialize for temperature sensor (LM73) on KidBright
#define TEMP_SDA    (4)     // LM73-1 Serial Data Line in IO4
#define TEMP_SCL    (5)     // LM73-1 Serial Clock Line in IO5
#define TEMP_ADDR   (0x4D)  // LM73-1 Slave Address in 0x4D
// token api key from LINE Notity
#define TOKEN       "xxx"

void LINE_NOTIFY( String messages ) {
  // define LINE Notify Token API
  LINE.setToken( TOKEN );
  // send message to alert user(s) in LINE App
  LINE.notify( messages );
}

void CONNECT_WIFI() {
  const char* WIFI_NAME = "xxx";        // Wifi's Name
  const char* WIFI_PASS = "xxx";  // Wifi's Password
  WiFi.mode( WIFI_STA );              // define WiFi in station mode
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
  // convert in MSB
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
  return TEMP_VALUES;
}

void setup() {
  // begin to use Wire lib with temp sensor address
  Wire.begin(TEMP_SDA, TEMP_SCL);
  // connect to WiFi
  CONNECT_WIFI();
  // switch 1 pin mode
  pinMode( SW1_PIN, INPUT_PULLUP );
  // set baud rate 115200
  Serial.begin(115200);
}

void loop() {
  // waiting for pressing sw1 to start to read temperature
  Serial.println("Do you want to start? [press switch 1] ");
  while ( digitalRead(SW1_PIN) ) {
    if ( !digitalRead(SW1_PIN) ) {
      return;
    }
  }
  static float MEASURE_TEMP = 0;
  int COUNTING_VAL = 1;
  // counting the temperature for 15 times (30 seconds)
  while ( COUNTING_VAL <= 15 ) {
    float TEMP = READ_TEMP();
    if ( TEMP != MEASURE_TEMP ) {
      Serial.println("**********************************");
      // measure again
      MEASURE_TEMP = TEMP;
      COUNTING_VAL = 1;
    } else {
      // counting for 15 times
      Serial.println( String(COUNTING_VAL) + " " + String(MEASURE_TEMP) );
      COUNTING_VAL++;
    }
    delay(2000);
  }
  for ( int i=1; i<=10; i++ ) {
    Serial.print(".");
    delay(500);
  }
  // send LINE Notify
  LINE_NOTIFY("> " + String(MEASURE_TEMP) + " Celsius<");
  Serial.println("\n>>>>>>>>>Notify's sucessfully<<<<<<<<<");
  delay(5000);
}
