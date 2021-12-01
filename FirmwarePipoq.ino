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
int relayOut = 53;
//sensor input pins
int cupSensor = 24;
int kernelSensor = 26;
int canSensor = 42 ;
//lcd pins
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
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
  //lcd
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Palomitas");
  lcd.setCursor(0,1);
  lcd.print("Pronta");
}

void loop(){
  if (Serial.available() > 0) {
    inByte = Serial.read();
    Serial.print("in: ");
    Serial.println(inByte, DEC);
  }
  manageCooking();
  
  
}

long manageCooking (){
  if (!full && !cooking && inByte == 'c') {
    
    if (!hasK) {
      lcd.print("Estamos sem milho!")
    } else {
      if (!hasCup) {
      lcd.print("Insira o copo")
    } else {
      
    
      //start cooking when received order 
     dispenseKernel(1);
     full = true;
     heatCorn();
     lcd.print("Preparando");
     StartTime = millis(); 
     }
     
    }
    
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
    csConf = 0;
  }
  return csConf > 10 ? true : false;
}

bool hasK() {
  if (digitalRead(kernelSensor) == LOW) {
    //cup is in place
    ksConf++;
  } else {
    ksConf = 0;
  }
  return ksConf > 10 ? true : false;
}

bool hasSoda() {
  if (digitalRead(canSensor) == LOW) {
    //cup is in place
    rsConf++;
  } else {
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

 
