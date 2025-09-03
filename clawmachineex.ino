#include <Servo.h>

Servo myServo;
Servo myServo2;
Servo thirdServo;
Servo myServo4;

const int servoPin = 2;
const int servoPin2 = 3;
const int servoPin4 = 13;
const int thirdServoPin = 4;

const int buttonPin = 11;
const int trigPin = 5;
const int echoPin = 6;


const int joyX = A0;  
const int joyY = A1;  

int armAngle1 = 90;  
int armAngle2 = 90;  
bool pressed = false;
bool clawClosed = false;
bool clawClosed2 = false;

void setup() {
  myServo.attach(servoPin);
  myServo2.attach(servoPin2);
  myServo4.attach(servoPin4);
  thirdServo.attach(thirdServoPin);

  myServo.write(armAngle1);
  myServo2.write(armAngle2);
  thirdServo.write(90);      
  myServo4.write(90);

  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(12,OUTPUT);

  pinMode(7, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY); 

  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.println(yValue);

 
  if (yValue < 400 && armAngle1 < 180) {
    armAngle1 += 2;
    myServo.write(armAngle1);
    delay(10);
  } else if (yValue > 600 && armAngle1 > 0) {
    armAngle1 -= 2;
    myServo.write(armAngle1);
    delay(10);
  }

  
  if (xValue < 400 && armAngle2 < 180) {
    armAngle2 += 2;
    myServo2.write(armAngle2);
    delay(10);
  } else if (xValue > 600 && armAngle2 > 0) {
    armAngle2 -= 2;
    myServo2.write(armAngle2);
    delay(10);
  }


  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && !pressed && !clawClosed && !clawClosed2) {
    pressed = true;
    thirdServo.write(150); 
    myServo4.write(10);     
    clawClosed = true;
    clawClosed2 = true;
  } else if (buttonState == LOW) {
    pressed = false;
  }


  long duration = getUltrasonicDistance();
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  

  if (distance > 0 && distance < 5 && clawClosed && clawClosed2) {

    thirdServo.write(90);   
    myServo4.write(50);     
    clawClosed = false;
    clawClosed2 = false;
    for(int i=0;i<=2;i++){
      tone(12,1000,250);
    }
    
    delay(150);
    for (int i = 0; i <= 2; i++) {
      digitalWrite(8, HIGH); delay(200);
      digitalWrite(7, HIGH); delay(200);
      digitalWrite(8, LOW); delay(200);
      digitalWrite(7, LOW); delay(200);
    }
    delay(200);
  }

  delay(10);
}


long getUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH, 30000);
}
