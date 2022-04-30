#include <Wire.h>
#include <WiFi.h>
#include <TridentTD_LineNotify.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
// GPIO Switch 1 and 2
#define SW1       (16)
#define SW2       (14)
// GPIO Switch USB
#define SW_USB    (25)
uint8_t fan_state = 0;    // turn on or off fan
// Temperature Sensor (LM73) I2C
#define TEMP_SDA  (4)     // LM73-1 Serial Data Line in IO4
#define TEMP_SCL  (5)     // LM73-1 Serial Clock Line in IO5
#define TEMP_ADDR (0x4D)  // LM73-1 Slave Address in 0x4D
// dot matrix (HT16K33) 8x16 I2C
Adafruit_8x16minimatrix matrix;
#define DOT_ADDR  (0x70)

static void CONNECT_WIFI() {
  const char* WIFI_NAME = "xxx"; // Wifi's Name
  const char* WIFI_PASS = "xxx"; // Wifi's Password
  WiFi.mode( WIFI_STA ); // define WiFi in station mode
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

static void LINE_NOTIFY( String msg ) {
  // token from line notigy api
  const char* TOKEN = "xxx";
  LINE.setToken( TOKEN );
  // send message to line app in line notify
  LINE.notify( msg );
}

// Read Temperature Sensor (LM73-1)
int READ_TEMP() {
  // use TEMP_DATA array to getting temp sensor value
  byte TEMP_DATA[2];  // getting 2 bytes 
  // start temp sensor and write its address
  Wire1.beginTransmission(TEMP_ADDR);
  // write temp address
  Wire1.write(0);
  Wire1.endTransmission();
  // start and read temp sensor
  Wire1.requestFrom(TEMP_ADDR, 2); // 2 is data 2 bytes
  TEMP_DATA[1] = Wire1.read();     // getting MSB temp data
  TEMP_DATA[0] = Wire1.read();     // getting LSB temp data
  // initialize TEMP_VALUES into 0.0 Celsius
  int TEMP_VALUES = 0;
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
  return TEMP_VALUES;
}

void setup() {
  // connect to WiFi
  CONNECT_WIFI();
  // Temperature (LM73) I2C
  Wire1.begin( TEMP_SDA, TEMP_SCL );
  // switch 1 and 2 pin mode
  pinMode( SW1, INPUT_PULLUP);
  pinMode( SW2, INPUT_PULLUP);
  // switch usb pin mode
  pinMode( SW_USB, OUTPUT );
  digitalWrite( SW_USB, HIGH );
  // set up DOT MATRIX (HT16K33)
  matrix.begin( DOT_ADDR );
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextColor(LED_ON);
  matrix.setTextWrap(false);
  // set baud rate
  Serial.begin(115200);
}

void loop() {
  // read temperatures
  int temp = READ_TEMP();
  // show temperatures on dot matrix
  matrix.clear();
  matrix.setCursor(2, 0);
  matrix.print(temp);
  matrix.writeDisplay();
  // if press SW2 send temperatures to LINE NOTIFY
  if ( !digitalRead(SW2) ) {
    LINE_NOTIFY("\nTemperature >> " + String(temp) + " Celsius");
  }
  /* condition for tuning on/off the fan cooler */
  // detecting the SW1
  uint8_t btn_state = digitalRead(SW1);
  if ( btn_state == 0 ) {
    // toggle btn state
    if ( fan_state == 0 ) {
      // if it turns off, turn it on
      fan_state = 1;
      digitalWrite( SW_USB, LOW );  // pull up switch
    } else { 
      // if it turns on, turn it off
      fan_state = 0;
      digitalWrite( SW_USB, HIGH );
    }
  }
  // send message to LINE NOTIFY if temperatures are more than 37 celsius
  if ( temp >= 37 ) {
    LINE_NOTIFY("\nTemperature >> " + String(temp) + " Celsius\nCooler's working...");
    // turn cooler fan on if temperatures are more than 37 celsius
    while ( temp >= 37 ) {
      digitalWrite( SW_USB, LOW );  // pull up switch
      delay(5000);  // delay for more cool!
      temp = READ_TEMP();
      // show temperatures on dot matrix
      matrix.clear();
      matrix.setCursor(2, 0);
      matrix.print(temp);
      matrix.writeDisplay();
      if ( temp < 37 ) {
        digitalWrite( SW_USB, HIGH );
        LINE_NOTIFY("\nTurning off cooler...\nTemperature >> " + String(temp) + " Celsius");
        return;
      }
    }      
  }
  delay(1000);
}
