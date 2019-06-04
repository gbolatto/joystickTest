/*
 * Basics for joystick controller
 * originally planned for DC motor speed changes with the vertical axis of the joystick but I do not have the proper components
 * instead I used an RGB LED to show bright green for up, red for down, with fading between to yellow in the middle.
 * the horizontal axis controls a 180 degree sweep of a servo
 * the button press controls an active buzzer
 * 
 * 06/03/2019
 * gbolatto
 */

#include <Servo.h>

const int BLUE = 3; // digital pin controlling blue part of RGB LED
const int GREEN = 5; // digital pin controlling green part of RGB LED
const int RED = 6; // digital pin controlling red part of RGB LED

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A1; // analog pin connected to X output
const int Y_pin = A0; // analog pin connected to Y output

int servoPos;

int redValue;
int greenValue;
int blueValue;

int xAxis;
int yAxis;

Servo turnServo;

int buzzer = 12;

void setup() {

  turnServo.attach(8);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);
}

// gets the percentage of the current position of the joystick as a float
float getFloatOfJoystick(int pos) {
  return float(pos) / 1024;
}

void loop() {

  servoPos = 90;

  xAxis = analogRead(X_pin);
  yAxis = analogRead(Y_pin);

  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.println("X-axis: ");
  Serial.println(xAxis);
  Serial.println(getFloatOfJoystick(xAxis));
  Serial.println("Y-axis: ");
  Serial.println(yAxis);
  Serial.println(getFloatOfJoystick(yAxis));
  Serial.print("\n\n");

  // buzzes horn while joystick button is pressed down
  if (digitalRead(SW_pin) == 0) {
    digitalWrite(buzzer, HIGH);
  }

  if (digitalRead(SW_pin) == 1) {
    digitalWrite(buzzer, LOW);
  }

  // as joystick is moved up, green goes brighter. as joystick goes down, red goes brighter. yellow in the middle.
  // sets the red value higher with the lower y axis 
  // upper bounds of 255
  redValue = 255 - getFloatOfJoystick(yAxis) * 255;
  // sets the green value higher with the higher y axis 
  // upper bounds of 128 to get a better yellow out of my cheap RGB LED
  greenValue = getFloatOfJoystick(yAxis) * 128; 
  blueValue = 0;

  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);

  // sets servoPos with upperbounds of 180 degrees
  servoPos = getFloatOfJoystick(xAxis) * 180;

  Serial.println(servoPos);
  Serial.print("\n\n");

  turnServo.write(servoPos);

  // half second delay between loops mainly to read the serial output easily while testing
  delay(500);
}
