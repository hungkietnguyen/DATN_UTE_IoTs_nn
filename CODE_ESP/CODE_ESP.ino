
#define BLYNK_TEMPLATE_ID "TMPL6wjHwV87B"
#define BLYNK_TEMPLATE_NAME "IOTsLab2"
#define BLYNK_AUTH_TOKEN "Sg4oMb1j3fanawZXPWO1J_DIfxT3Cy49"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char ssid[] = "D-Link_DIR-809";    // TÀI KHOẢN WIFI
char pass[] = "403403403";  // MẬT KHẨU WIFI
#define CONNECT V6
#define NHIET_DO V0
#define DO_AM V1
#define DO_AM_DAT V8
#define ANH_SANG V9
#define MUA V10
#define AUTO V2
#define BOM V4
#define DEN V5
#define PHUNSUONG V3
#define QUAT V7
#include <SoftwareSerial.h>
//const byte rxPin = 3;
//const byte txPin = 1;
//SoftwareSerial Serial2(rxPin, txPin);
unsigned long time_post_to_MEGA, time_blink;
String chuoi;
int flag, blink;
int auto_value, den_value, nhiet_do_value, do_am_value,dat_value, mua_value, anhsang_value;
int bom_value;
int ps_value;
int quat_value;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(38400);
  //Serial2.begin(38400);
  Serial.println("HELLO");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
  Blynk.virtualWrite(V4, 0);
  Blynk.virtualWrite(V5, 0);
  Blynk.virtualWrite(V6, 0);
  Blynk.virtualWrite(V7, 0);
  Blynk.virtualWrite(V8, 0);
  Blynk.virtualWrite(V9, 0);
  Blynk.virtualWrite(V10, 0);
}

void loop() {
  Blynk.run();

  if (Serial2.available()) {
    chuoi = Serial2.readString();  //Serial đọc chuỗi
    flag = 1;
  }
  if (flag == 1) {
    flag = 0;
    //Serial.print(chuoi);
    if (chuoi[0] == 'A') 
    {
      if (auto_value == 1) {
        auto_value = 0;
      } else {
        auto_value = 1;
      }
      Blynk.virtualWrite(AUTO, auto_value);
      Blynk.virtualWrite(DEN, 0);
      Blynk.virtualWrite(BOM, 0);
      Blynk.virtualWrite(PHUNSUONG, 0);
      Blynk.virtualWrite(QUAT, 0);
    }
    if (chuoi[0] == 'B') {
      if (den_value == 1) {
        den_value = 0;
      } else {
        den_value = 1;
      }
      Blynk.virtualWrite(DEN, den_value);
    }
    if (chuoi[0] == 'C') 
    {
      if (bom_value == 1){
        bom_value = 0;
      } else {
        bom_value = 1;
      }
      Blynk.virtualWrite(BOM, bom_value);
    }
    if (chuoi[0] == 'D') 
    {
      if (ps_value == 1){
        ps_value = 0;
      } else {
        ps_value = 1;
      }
      Blynk.virtualWrite(PHUNSUONG, ps_value);
    }
    if (chuoi[0] == 'E') 
    {
      if (quat_value == 1){
        quat_value = 0;
      } else {
        quat_value = 1;
      }
      Blynk.virtualWrite(QUAT, quat_value);
    }
    if (chuoi[0] == 'F') {
      char temp[10] = { 0 };
      temp[0] = chuoi[1];
      temp[1] = chuoi[2];
      do_am_value = atoi(temp);
      //Serial.println(do_am_value);
  }
  if (chuoi[0] == 'G') {
      char temp1[10] = { 0 };
      temp1[0] = chuoi[1];
      temp1[1] = chuoi[2];
      nhiet_do_value = atoi(temp1);
      //Serial.println(nhiet_do_value);
    }
    if (chuoi[0] == 'H') {
      char temp2[10] = { 0 };
      temp2[0] = chuoi[1];
      temp2[1] = chuoi[2];
      dat_value = atoi(temp2);
      //Serial.println(dat_value);
    }
    if (chuoi[0] == 'I') {
      char temp3[10] = { 0 };
      temp3[0] = chuoi[1];
      temp3[1] = chuoi[2];
      mua_value = atoi(temp3);
      //Serial.println(mua_value);
    }
    if (chuoi[0] == 'K') {
      char temp4[10] = { 0 };
      temp4[0] = chuoi[1];
      temp4[1] = chuoi[2];
      anhsang_value = atoi(temp4);
      //Serial.println(anhsang_value);
    }
  if ((unsigned long)(millis() - time_post_to_MEGA) > 3000) {
    // Serial2.println("HELLO");
    // Serial2.flush();
    // //Serial.println("HELLO");
    // time_post_to_MEGA = millis();
  }
  if ((unsigned long)(millis() - time_blink) > 1500) {
    Blynk.virtualWrite(NHIET_DO, nhiet_do_value);
    Blynk.virtualWrite(DO_AM, do_am_value);
    Blynk.virtualWrite(ANH_SANG, anhsang_value);
    Blynk.virtualWrite(DO_AM_DAT, dat_value);
    Blynk.virtualWrite(MUA, mua_value);
    if (blink == 0) {
      blink = 1;
      Blynk.virtualWrite(CONNECT, 1);
    } else {
      blink = 0;
      Blynk.virtualWrite(CONNECT, 0);
    }
    time_blink = millis();
  }
}
}
BLYNK_WRITE(AUTO) {
  auto_value = param.asInt();
  Serial2.println("A");
  Serial2.flush();
  den_value = 0;
  bom_value = 0;
  ps_value = 0;
  quat_value = 0;
   Blynk.virtualWrite(DEN, 0);
   Blynk.virtualWrite(BOM, 0);
   Blynk.virtualWrite(PHUNSUONG, 0);
   Blynk.virtualWrite(QUAT, 0);
  if (auto_value == 1) {
    Serial.println("CHE DO AUTO");
  } else {
    Serial.println("CHE DO BANG TAY");
  }
}
BLYNK_WRITE(DEN) {
  if (auto_value == 0) 
  {
    den_value = param.asInt();
    Serial2.println("B");
    Serial2.flush();
  } 
  else 
  {
    Blynk.virtualWrite(DEN, 0);
  }
}
BLYNK_WRITE(BOM) {
  if (auto_value == 0) 
  {
    bom_value = param.asInt();
    Serial2.println("C");
    Serial2.flush();
  } 
  else 
  {
    Blynk.virtualWrite(BOM, 0);
  }
}
BLYNK_WRITE(PHUNSUONG) {
  if (auto_value == 0) 
  {
    ps_value = param.asInt();
    Serial2.println("D");
    Serial2.flush();
  } 
  else 
  {
    Blynk.virtualWrite(PHUNSUONG, 0);
  }
}
BLYNK_WRITE(QUAT) {
  if (auto_value == 0) 
  {
    quat_value = param.asInt();
    Serial2.println("E");
    Serial2.flush();
  } 
  else 
  {
    Blynk.virtualWrite(QUAT, 0);
  }
}
