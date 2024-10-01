#define BLYNK_TEMPLATE_ID "TMPL69TpjEtve"
#define BLYNK_TEMPLATE_NAME "Sunithi"
#define BLYNK_AUTH_TOKEN "GEWS_jHuijfhh7SvQrLK0I4i0HTqBKmn"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
// กำหนดขาเชื่อมต่อสำหรับ A และ B ของ Encoder
const int encoderPinA = 32;
const int encoderPinB = 33;

// ตัวแปรสำหรับการนับจำนวนพัลส์
volatile long encoderValue = 0;

// กำหนดขาเชื่อมต่อสำหรับมอเตอร์
#define ena 26
#define in1 27
#define in2 14

// กำหนดช่วงค่าของแต่ละชั้น
const int floor1_min = 0;
const int floor1_max = 240;
const int floor2_min = 680;
const int floor2_max = 800;
const int floor3_min = 1302;
const int floor3_max = 1500;

// WiFi credentials
char ssid[] = "IRETUTOR_AZ-TRUE_2.4G";
char pass[] = "5687862020";

BlynkTimer timer;

void setup() {
  // ตั้งค่า pinMode สำหรับขา A และ B เป็น input พร้อมใช้ Pullup Resistor
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  // ตั้งค่า pinMode สำหรับขามอเตอร์
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // เปิด Serial Monitor สำหรับการแสดงผลค่า
  Serial.begin(9600);

  // ตั้งค่า interrupt สำหรับ Encoder
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoderA, CHANGE);

  // เริ่มต้นการเชื่อมต่อกับ Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

}

void loop() {
  Blynk.run();
  Blynk.virtualWrite(V1, encoderValue);
  Serial.print("Encoder Value: ");
  Serial.println(encoderValue);
}

// ฟังก์ชันสำหรับการอ่านค่าจาก Encoder
void readEncoderA() {
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
    encoderValue++;
  } else {
    encoderValue--;
  }
}


// ฟังก์ชันสำหรับการเคลื่อนมอเตอร์ไปยังตำแหน่งที่ต้องการ
void moveToPosition(int targetMin, int targetMax) {
  while (encoderValue < targetMin || encoderValue > targetMax) {
    if (encoderValue < targetMin) {
      analogWrite(ena, 255);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    } else if (encoderValue > targetMax) {
      analogWrite(ena, 255);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }
    delay(10);
  }
  stopMotor();
}

// ฟังก์ชันสำหรับการหยุดมอเตอร์
void stopMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ena, 0);
}

// ฟังก์ชันสำหรับการเลือกชั้นผ่าน Slider
BLYNK_WRITE(V0) {
  int floor = param.asInt();
  if (floor == 1) {
    moveToPosition(floor1_min, floor1_max);
  } else if (floor == 2) {
    moveToPosition(floor2_min, floor2_max);
  } else if (floor == 3) {
    moveToPosition(floor3_min, floor3_max);
  }
}
