/*  
 * This code is for SETTING and READING date&time.
 * I2C Communication in RTC MP79411
 */
#include <Wire.h>          // Wire Library for I2C Communication
#define I2C_SDA   (4)     // IO4 >> SDA_1 on kidBright
#define I2C_SCL   (5)     // IO5 >> SCL_1 on kidBright
#define RTC_ADDR  (0x6F)  // define I2C address for RTC
// variable for keeping thousand and hundred digit from year
int thous_hund = 0;

/* reference : http://www.tic.co.th/index.php?op=tips-detail&id=293 */
byte bcd_to_dec( byte bcd_value ) {
  byte  ten_digit = bcd_value >> 4;   // shift right 4 bits for read 4 front bits
  byte  one_digit = bcd_value & 0x0F; // operation AND with 1111 for read 4 last bits
  byte  dec_value = ( ten_digit * 10 ) + one_digit; // convert to decimal value
  return dec_value;
}

byte dec_to_bcd( byte dec_value ) {
  byte  ten_digit = dec_value / 10;   // divide decimal by 10 for get ten digit
  byte  one_digit = dec_value % 10;   // modulate decimal by 10 for get one digit
  byte  bcd_value = ( ten_digit << 4 ) | one_digit; // convert to bcd value
  return bcd_value;
}

boolean is_leap_year( int year ) {
  if ( (year % 4) == 0 ) {
    return true;
  } else if ( (year % 400) == 0 ){
    return true;
  } else {
    return false;
  }
}

int weekday(int date, int month, int year){
  // reference reading and formula
  // https://artofmemory.com/blog/how-to-calculate-the-day-of-the-week/
  // month and century code from Gregorian Calendar
  int month_codes_mapping[12] = {0,3,3,6,1,4,6,2,5,0,3,5};
  int century_codes_mapping[7] = {4,2,0,6,4,2,0};
  thous_hund = year / 100;          // thousand and hundred digit of year
  int two_digit_year = year % 100;  // ten and one digit of year
  // calculate year code
  int year_code = (two_digit_year + (two_digit_year/4)) % 7;
  int month_code = month_codes_mapping[month-1];  // month_code from array
  int century_code = 0;             // century_code from array
  // because of calendar system change
  // from the Julian Calendar to 
  // the Gregorian Calendar on 2 September 1752.
  if ( thous_hund * 100 < 1700 ) { // for year before 1700s
    century_code = ( 18 - thous_hund ) % 7;
  } else { // for 1700s - 2300s
    century_code = century_codes_mapping[thous_hund % 17];
  }
  // variable for weekday in integer 0-6 [Sunday-Saturday]
  int day_of_date = 0;
  if ( is_leap_year(year) ) { // FEB has 29 days
    day_of_date = (year_code + month_code + century_code + date - 1) % 7;
  } else { // FEB has 28 days
    day_of_date = (year_code + month_code + century_code + date) % 7;
  }
  return day_of_date;
}

void set_rtc_datetime( int date, int month, int year, int hour, int minute, int second ) {
  int ten_one_year = year % 100;                  // getting ten and one digit from year
  int day_of_week = weekday(date, month, year);   // calculating weekday
  Wire.beginTransmission(RTC_ADDR);               // start to communicate with RTC
  Wire.write(0x00);                               // write date and time start address
  // write all of datetime argument
  Wire.write( dec_to_bcd( second ) );
  Wire.write( dec_to_bcd( minute ) );
  Wire.write( dec_to_bcd( hour ) );
  Wire.write( dec_to_bcd( day_of_week+1 ) );
  Wire.write( dec_to_bcd( date ) );
  Wire.write( dec_to_bcd( month ) );
  Wire.write( dec_to_bcd( ten_one_year ) );
  Wire.endTransmission();
}

String get_rtc_datetime() {
  Wire.beginTransmission(RTC_ADDR); // start to communicate with RTC
  Wire.write(0x00);                 // write date and time start address
  Wire.endTransmission();           // stop for writing LOW
  Wire.requestFrom(RTC_ADDR, 7);    // request data from addr 0x00 to 0x06
  int second  =   bcd_to_dec(Wire.read());   // second
  int minute  =   bcd_to_dec(Wire.read());   // minute
  int hour    =   bcd_to_dec(Wire.read());   // hour
  int weekday =   bcd_to_dec(Wire.read())-1; // weekday
  int date    =   bcd_to_dec(Wire.read());   // date
  byte month  =   Wire.read();               // month
  int year    =   bcd_to_dec(Wire.read());   // year
  // 7 days of week in array
  char days[7][10]   =   {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  // 12 months of year in array
  char months[12][10] =  {"January", "February", "March", "April", "May", "June", "July", "August", "September", 
                    "October", "November", "December"}; 
  if ( month & 0x20 ) {
    month = ( month & 0x1F )-1;
  } else {
    month -= 1;
  }
  // getting date and time into conclusion string
  String DATETIME = "";
  DATETIME += days[weekday];
  DATETIME += (" " + String(date) + " ");
  DATETIME += months[month];
  DATETIME += (" " + String(thous_hund) + String(year));
  DATETIME += ("   " + String(hour) + 
               ":" + String(minute) + 
               ":" + String(second));
  return DATETIME;
}

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL); // start wiring RTC
  Serial.begin(115200);         // set baud rate 
  set_rtc_datetime(12, 2, 1992, 22, 42, 32);
}

void loop() {
  Serial.println(get_rtc_datetime());
  delay(3000);
}
