
#include <TimerOne.h>
#include <EEPROM.h>
#include "math.h"
#include <avr/interrupt.h>
#include <Nextion.h>
NexButton AUTO = NexButton(1, 1, "bt0");
NexButton DEN = NexButton(1, 2, "bt0");
NexButton BOM = NexButton(1, 3, "bt0");
NexButton PHUNSUONG = NexButton(1, 4, "bt0");
NexButton QUAT = NexButton(1, 5, "bt0");
NexButton REM = NexButton(1, 6, "bt0");
NexButton TG_HETHONG = NexButton(1, 7, "bt0");
NexButton PAGE_NUM = NexButton(1, 8, "bt0");
NexButton TG_THIETBI = NexButton(1, 9, "bt0");
NexButton THIETBI = NexButton(1, 10, "bt0");
// NexVariable dienap = NexVariable(1, 7, "dienap");
// NexVariable dongdien = NexVariable(1, 8, "dongdien");
NexNumber n0 = NexNumber(2, 5, "n0");
NexNumber n1 = NexNumber(2, 3, "n1");
NexNumber n2 = NexNumber(2, 4, "n2");
NexNumber n3 = NexNumber(2, 6, "n3");
NexNumber n4 = NexNumber(2, 7, "n4");
NexNumber n5 = NexNumber(2, 8, "n5");
NexNumber n6 = NexNumber(2, 9, "n6");
NexNumber n7 = NexNumber(2, 10, "n7");
NexNumber n8 = NexNumber(2, 11, "n8");
NexNumber n9 = NexNumber(2, 12, "n9");
NexTouch *nex_listen_list[] = {
  &AUTO,
  &DEN,
  &BOM,
  &PHUNSUONG,
  &QUAT,
  &REM,
  &TG_HETHONG,
  &PAGE_NUM,
  &TG_THIETBI,
  &THIETBI,
  NULL
};
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <Wire.h>
#include <DS3231.h>

DS3231 myRTC;
byte year;
byte month;
byte date;
byte dow;
byte hour;
byte minute;
byte second;
bool statusb0 = false;
bool century = false;
bool h12Flag;
bool pmFlag;
#define DO_AM_DAT A0
#define CAM_BIEN_MUA 24
#define CAM_BIEN_QUANG 22
#define SYS_DEN 3
#define SYS_BOM 4
#define SYS_PS 5
#define SYS_QUAT 6

int page_number = 1;
int h, t, dat_value, mua_value, anhsang_value;
int h_set, t_set, dat_value_set, mua_value_set, anhsang_value_set;
int count;
int den_value, bom_value, ps_value, quat_value, auto_value;
int rem_value;
unsigned long time_post_to_HMI, time_read_sensor, time_post_to_ESP, time_auto, time_post_sensor;
int gio, phut, ngay, thang, nam;
int gio_on[5] = {};
int gio_off[5] = {};
int phut_on[5] = {};
int phut_off[5] = {};
int thiet_bi;
String chuoi;
int flag;
int pin_thiet_bi[5] = { 3, 4, 5, 6, 11 };

int in3 = 30;
int in4 = 32;
int ctht1 = 26;
int ctht2 = 28;

int trangthaict1;
int trangthaict2;

void setup() {
  Serial.begin(9600);
  Serial2.begin(38400);
  //Serial3.begin(38400);
  Serial.println("HELLO");
  AUTO.attachPush(AUTOPushCallback);
  DEN.attachPush(DENPushCallback);
  BOM.attachPush(BOMPushCallback);
  PHUNSUONG.attachPush(PHUNSUONGPushCallback);
  QUAT.attachPush(QUATPushCallback);
  REM.attachPush(REMPushCallback);
  TG_HETHONG.attachPush(TG_HETHONGPushCallback);
  PAGE_NUM.attachPush(PAGE_NUMPushCallback);
  TG_THIETBI.attachPush(TG_THIETBIPushCallback);
  THIETBI.attachPush(THIETBIPushCallback);
  pinMode(13, OUTPUT);
  pinMode(DO_AM_DAT, INPUT);
  pinMode(CAM_BIEN_MUA, INPUT);
  pinMode(CAM_BIEN_QUANG, INPUT);
  pinMode(SYS_DEN, OUTPUT);
  pinMode(SYS_BOM, OUTPUT);
  pinMode(SYS_PS, OUTPUT);
  pinMode(SYS_QUAT, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ctht1, INPUT);
  pinMode(ctht2, INPUT);
  for (int i = 0; i <= 4; i++) {
    pinMode(pin_thiet_bi[i], OUTPUT);
    digitalWrite(pin_thiet_bi[i], HIGH);
  }
  digitalWrite(SYS_DEN, HIGH);
  digitalWrite(SYS_BOM, HIGH);
  digitalWrite(SYS_PS, HIGH);
  digitalWrite(SYS_QUAT, HIGH);
  dht.begin();
  Wire.begin();
  // for (int i = 0; i <= 4; i++) {
  //   EEPROM.write(100 + i, 7 + i);
  //   EEPROM.write(105 + i, 12 + i);
  //   EEPROM.write(110 + i, 30 + i);
  //   EEPROM.write(115 + i, 36 + i);
  // }
  for (int i = 0; i <= 4; i++) {
    gio_on[i] = EEPROM.read(100 + i);
    gio_off[i] = EEPROM.read(105 + i);
    phut_on[i] = EEPROM.read(110 + i);
    phut_off[i] = EEPROM.read(115 + i);
  }
}
void AUTOPushCallback(void *ptr) {
  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  if (auto_value == 0) {
    auto_value = 1;
  } else {
    auto_value = 0;
  }
  den_value = 0;
  bom_value = 0;
  ps_value = 0;
  quat_value = 0;
  digitalWrite(SYS_DEN, HIGH);
  digitalWrite(SYS_BOM, HIGH);
  digitalWrite(SYS_PS, HIGH);
  digitalWrite(SYS_QUAT, HIGH);
  delay(1000);//--------------------------------
  Serial2.println("A");
  Serial2.flush();
}
void DENPushCallback(void *ptr) {
  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  if (den_value == 0) {
    den_value = 1;
    digitalWrite(SYS_DEN, LOW);
  } else {
    den_value = 0;
    digitalWrite(SYS_DEN, HIGH);
  }
  Serial2.println("B");
  Serial2.flush();
}
void BOMPushCallback(void *ptr) {
  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  if (bom_value == 0) {
    bom_value = 1;
    digitalWrite(SYS_BOM, LOW);
  } else {
    bom_value = 0;
    digitalWrite(SYS_BOM, HIGH);
  }
   Serial2.println("C");
   Serial2.flush();
}
void PHUNSUONGPushCallback(void *ptr) {
  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  if (ps_value == 0) {
    ps_value = 1;
    digitalWrite(SYS_PS, LOW);
  } else {
    ps_value = 0;
    digitalWrite(SYS_PS, HIGH);
  }
  Serial2.println("D");
  Serial2.flush();
}
void QUATPushCallback(void *ptr) {
  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  if (quat_value == 0) {
    quat_value = 1;
    digitalWrite(SYS_QUAT, LOW);
  } else {
    quat_value = 0;
    digitalWrite(SYS_QUAT, HIGH);
  }
  Serial2.println("E");
  Serial2.flush();
}
void REMPushCallback(void *ptr) {
  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  if(mua_value==1) // ở trạng thái không có mưa
  {
    rem_value = 1;
    xuong();
  }
  else
  {
    rem_value = 0;
    len();
  }
}
void TG_HETHONGPushCallback(void *ptr) {

  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  delay(500);
  uint32_t number51 = 0;
  n0.getValue(&number51);
  char temp1[10] = { 0 };
  utoa(number51, temp1, 10);
  phut = atoi(temp1);

  uint32_t number52 = 0;
  n2.getValue(&number52);
  char temp2[10] = { 0 };
  utoa(number52, temp2, 10);
  gio = atoi(temp2);

  uint32_t number53 = 0;
  n7.getValue(&number53);
  char temp3[10] = { 0 };
  utoa(number53, temp3, 10);
  ngay = atoi(temp3);

  uint32_t number54 = 0;
  n8.getValue(&number54);
  char temp4[10] = { 0 };
  utoa(number54, temp4, 10);
  thang = atoi(temp4);

  uint32_t number55 = 0;
  n9.getValue(&number55);
  char temp5[10] = { 0 };
  utoa(number55, temp5, 10);
  nam = atoi(temp5);
  myRTC.setHour(gio);
  myRTC.setMinute(phut);
  myRTC.setDate(ngay);
  myRTC.setMonth(thang);
  myRTC.setYear(nam);
}
void TG_THIETBIPushCallback(void *ptr) {

  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  delay(500);
  uint32_t number51 = 0;
  n3.getValue(&number51);
  char temp1[10] = { 0 };
  utoa(number51, temp1, 10);
  gio_on[thiet_bi] = atoi(temp1);

  uint32_t number52 = 0;
  n4.getValue(&number52);
  char temp2[10] = { 0 };
  utoa(number52, temp2, 10);
  phut_on[thiet_bi] = atoi(temp2);

  uint32_t number53 = 0;
  n5.getValue(&number53);
  char temp3[10] = { 0 };
  utoa(number53, temp3, 10);
  gio_off[thiet_bi] = atoi(temp3);

  uint32_t number54 = 0;
  n6.getValue(&number54);
  char temp4[10] = { 0 };
  utoa(number54, temp4, 10);
  phut_off[thiet_bi] = atoi(temp4);

  EEPROM.write(100 + thiet_bi, gio_on[thiet_bi]);
  EEPROM.write(105 + thiet_bi, gio_off[thiet_bi]);
  EEPROM.write(110 + thiet_bi, phut_on[thiet_bi]);
  EEPROM.write(115 + thiet_bi, phut_off[thiet_bi]);
}
void THIETBIPushCallback(void *ptr) {

  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  thiet_bi = thiet_bi + 1;
  if (thiet_bi > 4) thiet_bi = 0;
  Serial.print("page2.n3.val=");
  Serial.print(gio_on[thiet_bi]);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("page2.n4.val=");
  Serial.print(phut_on[thiet_bi]);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("page2.n5.val=");
  Serial.print(gio_off[thiet_bi]);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("page2.n6.val=");
  Serial.print(phut_off[thiet_bi]);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
void PAGE_NUMPushCallback(void *ptr) {
  if (page_number == 1) {
    page_number = 2;
    Serial.print("page2.n2.val=");  // GIỜ
    Serial.print(myRTC.getHour(h12Flag, pmFlag));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n0.val=");  //PHÚT
    Serial.print(myRTC.getMinute());
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n7.val=");  //NGÀY
    Serial.print(myRTC.getDate());
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n8.val=");  //THÁNG
    Serial.print(myRTC.getMonth(century));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n9.val=");  //NĂM
    Serial.print(myRTC.getYear() % 100);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n1.val=");
    Serial.print(1);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);

    Serial.print("page2.n3.val=");
    Serial.print(3);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n4.val=");
    Serial.print(4);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n5.val=");
    Serial.print(5);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n6.val=");
    Serial.print(6);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    thiet_bi = 0;
    Serial.print("page2.n3.val=");
    Serial.print(gio_on[thiet_bi]);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n4.val=");
    Serial.print(phut_on[thiet_bi]);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n5.val=");
    Serial.print(gio_off[thiet_bi]);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.n6.val=");
    Serial.print(phut_off[thiet_bi]);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);

  } else {
    page_number = 1;
  }
  if (digitalRead(13) == 1) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
}
void doc_tt()
{
  trangthaict1 = digitalRead(ctht1);
  trangthaict2 = digitalRead(ctht2);
  //---------------------------------------
}
void xuong() 
{
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void len() 
{
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void dung()
{
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}
void test()
{
  if(mua_value==1) // không mưa
  {
    len();
    if(trangthaict2 == 0)
    {
      dung();
      //
    }
  }
  else if(mua_value==0) // có mưa
  {
    xuong();
    if(trangthaict1 == 0)
    {
      dung(); 
    }
  }
  else
  {
    //
  }
}
void loop() {
  doc_tt();
  test();
  nexLoop(nex_listen_list);

  if ((unsigned long)(millis() - time_read_sensor) > 1000) {
    h = dht.readHumidity();
    t = dht.readTemperature();
    dat_value = map(analogRead(DO_AM_DAT), 0, 1023, 99, 0);
    
    //int hu = analogRead(DO_AM_DAT); // đọc giá trị analog từ cảm biến độ ẩm đất
    //dat_value = map(hu,0,1023,100,0); // đổi giá trị từ 0-100
    
    mua_value = digitalRead(CAM_BIEN_MUA);
    anhsang_value = digitalRead(CAM_BIEN_QUANG);
    time_read_sensor = millis();
  }
//  if ((unsigned long)(millis() - time_post_to_ESP) > 1000) 
//  {
//    String data = String(t) + ";" + String(h) + ";" + String(dat_value) + "," + String(mua_value) + ";" + String(anhsang_value);
//    Serial2.println(data); //Gửi giá trị nhiệt độ và độ ẩm qua Software serial
//    Serial.println(data); //In ra Serial
//    time_post_to_ESP = millis();
//  }
  //---------------------------------------------------
  if ((unsigned long)(millis() - time_post_sensor) > 1000) {
    count = count + 1;
    switch (count) {
      case 1:
        Serial2.println("F" + String(h));
        Serial2.flush();
        break;
      case 2:
        Serial2.println("G" + String(t));
        Serial2.flush();
        break;
      case 3:
        Serial2.println("H" + String(dat_value));
        Serial2.flush();
        break;
      case 4:
        Serial2.println("I" + String(mua_value));
        Serial2.flush();
        break;
      case 5:
        count = 0;
        Serial2.println("K" + String(anhsang_value));
        Serial2.flush();
        break;
    }
    time_post_sensor = millis();
  }
  //---------------------------------------------------
  if ((unsigned long)(millis() - time_post_to_HMI) > 1000 && page_number == 1) {
    Serial.print("page1.n0.val=");
    Serial.print(t);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page1.n1.val=");
    Serial.print(h);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page1.n2.val=");
    Serial.print(dat_value);
    
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page1.n3.val=");
    Serial.print(mua_value);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page1.n4.val=");
    Serial.print(anhsang_value);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page1.t1.txt=");
    if (myRTC.getMinute() < 10) {
      Serial.print("\"" + String(myRTC.getHour(h12Flag, pmFlag)) + ":0" + String(myRTC.getMinute()) + "\"");
    } else {
      Serial.print("\"" + String(myRTC.getHour(h12Flag, pmFlag)) + ":" + String(myRTC.getMinute()) + "\"");
    }
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    String S_day, S_month;
    if (myRTC.getDate() < 10) {
      S_day = "0" + String(myRTC.getDate());
    } else {
      S_day = String(myRTC.getDate());
    }
    if (myRTC.getMonth(century) < 10) {
      S_month = "0" + String(myRTC.getMonth(century));
    } else {
      S_month = String(myRTC.getMonth(century));
    }
    Serial.print("page1.t2.txt=");
    Serial.print("\"" + S_day + "/" + S_month + "/" + String(myRTC.getYear() % 100) + "\"");
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    time_post_to_HMI = millis();
  }
  if ((unsigned long)(millis() - time_auto) > 1000 && auto_value == 1) {
    if (anhsang_value == 1) {
      den_value = 1;
      digitalWrite(SYS_DEN, LOW);
      Serial.print("page1.bt1.val=");
      Serial.print(1);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    } else if (anhsang_value == 0) {
      den_value = 0;
      digitalWrite(SYS_DEN, HIGH);
      Serial.print("page1.bt1.val=");
      Serial.print(0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    }
    if (h > 60 && h <= 90) {
      //quat_value = 1;
      ps_value = 1;
      digitalWrite(SYS_PS, LOW);
      Serial.print("page1.bt4.val=");
      Serial.print(1);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    } else if (h >= 40 && h <= 60) {
      //quat_value = 0;
      ps_value = 0;
      digitalWrite(SYS_PS, HIGH);
      Serial.print("page1.bt4.val=");
      Serial.print(0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    }
    if (t >= 10 && t <= 31) {
      //ps_value = 0;
      quat_value = 0;
      digitalWrite(SYS_QUAT, HIGH);
      Serial.print("page1.bt3.val=");
      Serial.print(0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    } else if (t > 31 && t <= 40) {
      //ps_value = 1;
      quat_value = 1;
      digitalWrite(SYS_QUAT, LOW);
      Serial.print("page1.bt3.val=");
      Serial.print(1);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    }
    if (dat_value >= 53 && dat_value <= 73) {
      bom_value = 0;
      digitalWrite(SYS_BOM, HIGH);
      Serial.print("page1.bt2.val=");
      Serial.print(0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    } else if (dat_value >= 0 && dat_value <= 50) {
      bom_value = 1;
      digitalWrite(SYS_BOM, LOW);
      Serial.print("page1.bt2.val=");
      Serial.print(1);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    }
    //---------------------------------------------
    if (t >= t_set) {
    } else {
    }
    if (dat_value >= dat_value_set) {
    } else {
    }
    if (mua_value >= mua_value_set) {
    } else {
    }
    if (anhsang_value >= anhsang_value_set) {
    } else {
    }
    time_auto = millis();
  }
  for (int i = 0; i <= 4; i++) {
    // gio_on[i] = EEPROM.read(100 + i);
    // gio_off[i] = EEPROM.read(105 + i);
    // phut_on[i] = EEPROM.read(110 + i);
    // phut_off[i] = EEPROM.read(115 + i);
    if (gio_on[i] == myRTC.getHour(h12Flag, pmFlag) && phut_on[i] == myRTC.getMinute()) {
      digitalWrite(pin_thiet_bi[i], LOW);
      switch (i) {
        case 0:
          den_value = 1;
          Serial.print("page1.bt1.val=");
          Serial.print(1);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
        case 1:
          bom_value = 1;
          Serial.print("page1.bt2.val=");
          Serial.print(1);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
        case 2:
          ps_value = 1;
          Serial.print("page1.bt3.val=");
          Serial.print(1);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
        case 3:
          quat_value = 1;
          Serial.print("page1.bt4.val=");
          Serial.print(1);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
      }
    }
    if (gio_off[i] == myRTC.getHour(h12Flag, pmFlag) && phut_off[i] == myRTC.getMinute()) {
      digitalWrite(pin_thiet_bi[i], HIGH);
      switch (i) {
        case 0:
          den_value = 0;
          Serial.print("page1.bt1.val=");
          Serial.print(0);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
        case 1:
          bom_value = 0;
          Serial.print("page1.bt2.val=");
          Serial.print(0);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
        case 2:
          ps_value = 0;
          Serial.print("page1.bt3.val=");
          Serial.print(0);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
        case 3:
          quat_value = 0;
          Serial.print("page1.bt4.val=");
          Serial.print(0);
          Serial.write(0xff);
          Serial.write(0xff);
          Serial.write(0xff);
          break;
      }
    }
  }
  if (Serial2.available()) {
    chuoi = Serial2.readString();  //Serial đọc chuỗi
    flag = 1;
  }
  if (flag == 1) {
    flag = 0;
    // Serial.print(chuoi);
    // Serial.flush();
    switch (chuoi[0]) {
      case 'A':
        if (auto_value == 0) {
          auto_value = 1;
        } else {
          auto_value = 0;
        }
        Serial.print("page1.bt0.val=");
        Serial.print(auto_value);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.print("page1.auto_manual.val=");
        Serial.print(auto_value);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.print("page1.bt1.val=");
        Serial.print(0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.print("page1.bt2.val=");
        Serial.print(0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.print("page1.bt3.val=");
        Serial.print(0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.print("page1.bt4.val=");
        Serial.print(0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.print("page1.bt5.val=");
        Serial.print(0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        den_value = 0;
        bom_value = 0;
        ps_value = 0;
        quat_value = 0;
        digitalWrite(SYS_DEN, HIGH);
        digitalWrite(SYS_BOM, HIGH);
        digitalWrite(SYS_PS, HIGH);
        digitalWrite(SYS_QUAT, HIGH);
        // den_value = 0;
        // Serial.print("page1.bt1.val=");
        // Serial.print(0);
        // Serial.write(0xff);
        // Serial.write(0xff);
        // Serial.write(0xff);
        break;
      case 'B':
        if (den_value == 0) {
          den_value = 1;
          digitalWrite(SYS_DEN, LOW);
        } else {
          den_value = 0;
          digitalWrite(SYS_DEN, HIGH);
        }
        Serial.print("page1.bt1.val=");
        Serial.print(den_value);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        break;
      case 'C':
        if (bom_value == 0) 
        {
          bom_value = 1;
          digitalWrite(SYS_BOM, LOW);
        }
        else
        {
          bom_value = 0;
          digitalWrite(SYS_BOM, HIGH);
        }
        Serial.print("page1.bt2.val=");
        Serial.print(bom_value);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        break;
      case 'D':
        if (ps_value == 0) 
        {
          ps_value = 1;
          digitalWrite(SYS_PS, LOW);
        }
        else
        {
          ps_value = 0;
          digitalWrite(SYS_PS, HIGH);
        }
        Serial.print("page1.bt3.val=");
        Serial.print(ps_value);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        break;
      case 'E':
        if (quat_value == 0) 
        {
          quat_value = 1;
          digitalWrite(SYS_QUAT, LOW);
        }
        else
        {
          quat_value = 0;
          digitalWrite(SYS_QUAT, HIGH);
        }
        Serial.print("page1.bt4.val=");
        Serial.print(quat_value);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        break;
    }
  }
  // if ((unsigned long)(millis() - time_post_to_ESP) > 1000) {
  //   Serial2.println("CDE");
  //   Serial2.flush();
  //   time_post_to_ESP = millis();
  // }
}
