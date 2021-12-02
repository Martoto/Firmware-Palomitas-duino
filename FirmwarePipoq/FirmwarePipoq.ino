#include <Servo.h>
#include <LiquidCrystal.h>
Servo kServo;
Servo cServo;

unsigned long StartTime;
const unsigned long CookT = 60000;
const unsigned int cPerServ = 2;

//pins
int kernelServo = 3;
int canServo = 2;
int relayOut = 7;
//sensor input pins
int cupSensor = 6;
int kernelSensor = 5;
int canSensor = 4;
//led out pins
int kLed = 8;
int sLed = 9;
int cLed = 10;
//lcd pins
const int pin_RS = 11; 
const int pin_EN = 11; 
const int pin_d4 = 11; 
const int pin_d5 = 11; 
const int pin_d6 = 11;
const int pin_d7 = 11; 
const int pin_BL = 11; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

//input
int inByte = 0;

int ksConf = 0;
int rsConf = 0;
int csConf = 0;

bool full = false;
bool cooking = false;

void setup(){
  Serial.begin(9600);
  kServo.attach(kernelServo);
  cServo.attach(canServo);
  pinMode(relayOut, OUTPUT);  
  digitalWrite(relayOut, HIGH);
  pinMode(cupSensor, INPUT);
  pinMode(kernelSensor, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(kLed, OUTPUT);
  pinMode(cLed, OUTPUT);
  pinMode(sLed, OUTPUT);
  //lcd
  /*lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Palomitas");
  lcd.setCursor(0,1);
  lcd.print("Pronta");*/
}

void loop(){
  if (!hasK()) {
    turnOffPan();
    return;
  }
  full = true;
  if (!full) {
      dispenseKernel(3);  
      full = true;
  }
  heatCorn();
  if (!hasCup()) {
    digitalWrite(LED_BUILTIN, HIGH);
    turnOffPan();
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    heatCorn(); 
  }
   if(!hasSoda()){
    digitalWrite(sLed, HIGH);
    } else {
      digitalWrite(sLed, LOW);
    }
}

long manageCooking (){
  if (!full && !cooking && inByte == 'c') {
    if (!hasK()) {
      digitalWrite(kLed, HIGH);
    } else {
      digitalWrite(kLed, LOW);
    }
  } else {
      //start cooking when received order 
     dispenseKernel(1);
     full = true;
     heatCorn();
     lcd.print("Preparando");
     StartTime = millis(); 
    }
  if (!hasCup()) {
    //when cup is removed
    Serial.print("tirou copo");
    cooking = false;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
    turnOffPan();
  }
  if (hasCup() && full == true && !cooking) {
    //resume cooking after cup reinserted
    Serial.print("recolocou copo");
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
    cooking = true;
  }
  if (millis() - StartTime > CookT) {
    turnOffPan();
  }
  return millis() - StartTime;
}

bool hasCup() {
  if (digitalRead(cupSensor) == LOW) {
    //cup is in place
    csConf++;
  } else {
    csConf--;
  }
  if (csConf < 0) {
    csConf = 0;
  }
  return csConf > 10 ? true : false;
}

bool hasK() {
  if (digitalRead(kernelSensor) == LOW) {
    //cup is in place
    ksConf++;
  } else {
    ksConf--;
  }
  if (ksConf < 0) {
    ksConf = 0;
  }
  return ksConf > 10 ? true : false;
}

bool hasSoda() {
  if (digitalRead(canSensor) == LOW) {
    //cup is in place
    rsConf++;
  } else {
    rsConf--;
  }
  if (rsConf < 0) {
      rsConf = 0;
  }
  return rsConf > 10 ? true : false;
}

void dispenseKernel(int multiplier){
  kServo.write(180);
  delay(cPerServ*multiplier*1000);
  kServo.write(91);
}

void heatCorn() {
  digitalWrite(relayOut, LOW);
}

void turnOffPan() {
    digitalWrite(relayOut, HIGH);
}

void dispenseCan(){
  cServo.write(180);
  delay(1000);
  cServo.write(91);
}

 
