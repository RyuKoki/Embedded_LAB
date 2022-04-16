// จัดทำโดย 62-010126-2026-1 อรภิญญา โภคหัตถโกศล
// เสนอ อาจารย์ดนุชา ประเสริฐสม
// รายงานการอ่านค่า LM73 บนบอร์ด KidBright

// Wire library for I2C Communication 
#include <Wire.h>
// select temp sensor (LM73) in KidBright32
#define I2C_SDA (4)
#define I2C_SCL (5)
// define address of temp sensor (LM73-1)
#define I2C_ADDR (0x4D)

void setup() {
  // begin to use Wire lib with temp sensor address
  Wire.begin(I2C_SDA, I2C_SCL);
  // set baud rate 115200
  Serial.begin(115200);
}

float READ_TEMPERATURE() {
  // use TEMP_DATA array to getting temp sensor value
  byte TEMP_DATA[2];  // getting 2 bytes 
  // start temp sensor and write its address
  Wire.beginTransmission(I2C_ADDR);
  // write temp address
  Wire.write(0);
  Wire.endTransmission();
  // start and read temp sensor
  Wire.requestFrom(I2C_ADDR, 2);  // 2 is data 2 bytes
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

void loop() {
  // start reading temperature
  float TEMP = READ_TEMPERATURE();
  // show up temperature in Serial monitor
  Serial.print("Temperatures >> ");
  Serial.print(TEMP);
  Serial.println(" Celsius");
  delay(500);
}
