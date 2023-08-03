
#include <DS1302.h>
#include <SerialCommand.h>
#include <SoftwareSerial.h>

#define MayBom 4
#define DoAm 7
#define TrangThaiMayBom     A1
#define AutoDoAm            A2
#define AutoThoiGian        A3 

SerialCommand SCmd;
DS1302 rtc(13,SDA,SCL);
Time t;
int giomo;
int phutmo;
int giotat;
int phuttat;

void setup() {
  Serial.begin(115200);
  rtc.halt(false);
  rtc.writeProtect(false);
  pinMode(MayBom, OUTPUT);
  pinMode(DoAm, INPUT);
  pinMode(TrangThaiMayBom, OUTPUT);
  pinMode(AutoDoAm, INPUT);
  pinMode(AutoThoiGian, INPUT);
  digitalWrite(MayBom, HIGH);
  digitalWrite(TrangThaiMayBom, HIGH);
  SCmd.addCommand("TIME_ON",SetTimeOn);
  SCmd.addCommand("TIME_OFF",SetTimeOff);
  SCmd.addCommand("D1_ON",BatMayBom);
  SCmd.addCommand("D1_OFF",TatMayBom);
  SCmd.addCommand("SET_T",setTimeRTC);
  SCmd.addCommand("D2_UPD",d2Update);
  SCmd.addCommand("D3_UPD",d3Update);
}

void loop() {
 SCmd.readSerial();
 if(digitalRead(TrangThaiMayBom)==1){
    if(digitalRead(AutoThoiGian) == 0){
      t = rtc.getTime();
      if(t.hour == giomo && t.min == phutmo){
        digitalWrite(MayBom, LOW); 
      }
    if(t.hour == giotat && t.min == phuttat){
        digitalWrite(MayBom, HIGH);
      };  
    };
    if(digitalRead(AutoDoAm) == 0) {
      
      Serial.println(digitalRead(DoAm));
      if(digitalRead(DoAm) == 1){
        digitalWrite(MayBom, LOW);
        Serial.println("Do am thap");
      }else {
        digitalWrite(MayBom, HIGH);
        Serial.println("Do am cao");
      };
      delay(1000);
    };
 };
}
void TatMayBom(){
  digitalWrite(MayBom, HIGH);
  digitalWrite(TrangThaiMayBom, HIGH);
  Serial.println("Tat may bom"); 
}
void BatMayBom(){
  digitalWrite(MayBom, LOW);  
  digitalWrite(TrangThaiMayBom, LOW);
  Serial.println("Bat may bom");
}

void SetTimeOn(){
  char *arg;  
  arg = SCmd.next();    
  if (arg != NULL) 
  {
    String h,m;
    h.concat(arg[0]);
    h.concat(arg[1]);
    m.concat(arg[3]);
    m.concat(arg[4]);
    giomo = h.toInt();
    phutmo = m.toInt();
    Serial.print("Set time on: ");
    Serial.print(giomo);
    Serial.print(":");
    Serial.println(phutmo);
  } 
}
void SetTimeOff(){  
  char *arg;  
  arg = SCmd.next();    
  if (arg != NULL) 
  {
    String h,m;
    h.concat(arg[0]);
    h.concat(arg[1]);
    m.concat(arg[3]);
    m.concat(arg[4]);
    giotat = h.toInt();
    phuttat = m.toInt();
    Serial.print("Set time off: ");
    Serial.print(giotat);
    Serial.print(":");
    Serial.println(phuttat);
  }
}
void setTimeRTC(){
   char *arg;  
  arg = SCmd.next();  
  if (arg != NULL) 
  {
    String h,m,s;
    h.concat(arg[0]);
    h.concat(arg[1]);
    m.concat(arg[3]);
    m.concat(arg[4]);
    s.concat(arg[6]);
    s.concat(arg[7]);
    rtc.setTime(h.toInt(), m.toInt(), s.toInt());
    Serial.print("Gio he thong: "); 
    Serial.println(rtc.getTimeStr());
  }
}
void d2Update(){  
  char *arg;  
  arg = SCmd.next();    
  if (arg != NULL) 
  {
    int b = atoi(arg);
    if(b == 1){
    Serial.println("Bat do am !!!");
    }else Serial.println("Tat do am!!!");
  }
}
void d3Update(){  
  char *arg;  
  arg = SCmd.next();    
  if (arg != NULL) 
  {
    int b = atoi(arg);
    if(b == 1){
    Serial.println("Bat hen gio!!!");
    }else Serial.println("Tat hen gio!!!");
  }
}
