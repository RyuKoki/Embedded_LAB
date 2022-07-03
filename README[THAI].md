# วิธีตั้งค่าการใช้ ESP8266 กับโปรแกรม Arduino IDE
## ขั้นตอนการดาวน์โหลด Arduino IDE
1. เข้าเว็บไซต์ ```https://www.arduino.cc/en/software```
2. เลื่อนลงมาจะเจอคำว่า Downloads 
3. ใต้หัวข้อ Download Options ให้กดเลือก __Windows__ Win 7 and newer
4. กดเลือก Just Download
5. รอการดาวน์โหลดจนเสร็จสิ้น
6. เปิดไฟล์ดาวน์โหลดแล้วทำการคลิก Next จนคอมพิวเตอร์ติดตั้งจนเสร็จสิ้น

## ขั้นตอนการเพิ่ม ESP8266 ไปที่ Arduino IDE
1. เปิดโปรแกรม Arduino IDE
2. เข้าไปที่เมนู ```File >> Preferences```
3. ที่ช่อง ```Additional Board Manager URLs``` ให้ใส่คำสั่ง ```http://arduino.esp8266.com/stable/package_esp8266com_index.json``` แล้วกด __OK__
4. ทำการปิด และ เปิดโปรแกรม Arduino IDE ขึ้นมาใหม่
5. เข้าไปที่เมนู ```Tools >> Board: "..." >> Boards Manager...```
![image](https://user-images.githubusercontent.com/68011889/177040751-95ca2619-59e7-408a-ac8f-62085230fab9.png)
6. พิมพ์ค้นหาคำว่า ```esp8266``` เมื่อเจอแล้วให้กดคำว่า ```install```
![image](https://user-images.githubusercontent.com/68011889/177040837-21ee0e63-c5a6-49bc-86ef-cc757a211a82.png)
7. ทำการปิด และ เปิดโปรแกรม Arduino IDE ขึ้นมาใหม่
8. เข้าไปที่เมนู ```Tools >> Board: "..." >> ESP8266 (3.0.2) >> NodeMCU 1.0 (ESP-12E Module)```
![image](https://user-images.githubusercontent.com/68011889/177041113-bb431220-2477-4e25-b5db-e2c102c98766.png)

## ขั้นตอนการติดตั้ง Libraries
* ESP8266 จะเชื่อมต่อ WiFi ผ่าน Library ที่มีชื่อว่า ```ESP8266WiFi.h``` ซึ่งมีอยู่แล้วไม่ต้องติดตั้งเพิ่มเติม
* การใช้ LINE Notify 
  1. เข้าไปที่เมนู ```Sketch >> Include Library >> Manage Libraries...```
  ![image](https://user-images.githubusercontent.com/68011889/177041686-baa201ed-d44d-465f-aff8-3fffe35ec0af.png)
  2. พิมพ์ค้นหาคำว่า ```TridentTD_Linenotify``` แล้วกด ```install```
  ![image](https://user-images.githubusercontent.com/68011889/177041959-a63197d0-6efa-4d18-9c62-dc98700ba715.png)
  3. จากนั้นทำการ ปิด และ เปิดโปรแกรม Arduino IDE ขึ้นมาใหม่

## ข้อสังเกตต่างๆ
* เวลาที่เสียบ USB เข้าคอมพิวเตอร์แล้ว ให้สังเกตที่เมนู ```Tools >> Port >> ``` เมื่อมีชื่อ __COM__ ไหนขึ้นมาเพิ่ม ให้เลือก COM นั้น ซึ่งแต่ละเครื่องจะมีเลข COM ที่ไม่เหมือนกัน
![image](https://user-images.githubusercontent.com/68011889/177041214-89b49caa-38d9-479f-96ad-4021c4a391a7.png)
* เวลาที่ทำการ Upload Code แล้ว ต้องการเรียกใช้ Serial Monitor แต่ใน ```void setup()``` มีคำสั่ง ```Serial.begin(115200);```
  * เมื่อเปิด Serial Monitor ที่เมนูรูปแว่นขยายทางด้านบนขวามือแล้ว จะมีเมนู drop down ให้เลือกทางด้านล่างขวามือของ Serial Monitor ให้ตั้งค่าโดยการเลือกเป็น 115200 แทน
* การเชื่อมต่อ WiFi นั้น ที่ WiFi จะต้องเป็นความถี่ 2.4GHz เท่านั้น

