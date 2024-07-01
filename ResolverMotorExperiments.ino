#include <Resolver.h>

#include <arduino.h>

#include <ESP32Servo.h>

Resolver myResolver(34, 36, 39, 4, 208, 206, 20);  // calibrate to your specific application

// Parameters are:  Sensor Reference Pin, connect the exciter pulse directly to this pin to read
//                  Sin Pin
//                  Cos Pin
//                  Exciter Pin (output to exciter coil)
//                  Highest Cos sampled (for divider).  Run your resolver through all positions and use the highest number found
//                  Highest Sin sampled (for divider).  Run your resolver through all positions and use the highest number found

// PID control variables
const double Kp = .5;    // Proportional gain
const double Ki = 0.01;  // Integral gain
const double Kd = 0.02;  // Derivative gain

float setpoint = 0.0;  // Desired angle setpoint
double angle = 0.0;       // Current angle
int avgAngle = 0;

double error = 0.0;
double avgError = 0;
double prevError = 0.0;
double integral = 0.0;
double derivative = 0.0;
double output = 0.0;
const uint8_t VescOutputPin = 2;
int numReadings = 10;
int readings[20];
int readIndex;
int total;

Servo esc;

void setup() {
  
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  readIndex = 0;  // the index of the current reading
  total = 0;      // the running total
  avgError = 0;   // the average

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  esc.attach(VescOutputPin);

  //sweep through speeds from 1500(stopped) to 2000(max speed) to wake up VESC
  esc.writeMicroseconds(1500);
  for (int i = 1500; i < 2000; i += 10) {
    esc.writeMicroseconds(i);
    delay(500);
  }

  //calibrate max/min sin and cos values by completing a slow rotation for 30 seconds
  esc.writeMicroseconds(1750);
  delay(10000);
}



void loop() {

  //read angle measurement from resolver
  angle = myResolver.readResolver();
  //avgAngle =myResolver.ReturnAverage();

  //print resolver position and debugging info (highest lowest cos and sin)
  Serial.print("Resolver Position: ");
  Serial.print(angle);
  Serial.print(", average error: ");
  Serial.print(avgError);
  Serial.print(", highestSin: ");
  Serial.print(myResolver.ReturnHighestSin());
  Serial.print(", lowestSin: ");
  Serial.print(myResolver.ReturnLowestSin());
  Serial.print(", highestCos: ");
  Serial.print(myResolver.ReturnHighestCos());
  Serial.print(", lowestCos: ");
  Serial.print(myResolver.ReturnLowestCos());

  // Calculate error
  error = setpoint - angle;
  //avgError = setpoint - avgAngle;
  // compensates in case angle goes >360 or <0
  if (error > 180) {
    error -= 360.0;
  } else if (error < -180) {
    error += 360.0;
  }

  // Calculate PID terms
  double pTerm = Kp * error;
  integral += Ki * error;
  derivative = Kd * (error - prevError);

  // Calculate PID output
  output = pTerm + integral + derivative;


  // Apply PID output to control motor speed or PWM signal
  // Adjust motor speed or PWM output using the calculated output value
  /*int pwmValue = 1500;
  if (output > 0) {
    pwmValue = map(output, 0, 100, 1500, 2000);
  }
  if (output < 0) {
    pwmValue = map(output, -100, 0, 1000, 1500);
  }
  if (output == 0) {
    pwmValue = 1500;
  }*/
  int pwmValue = map(output, -100, 100, 1000, 2000);
  esc.writeMicroseconds(pwmValue);

  Serial.print(", error: ");
  Serial.print(error);

  Serial.print(", output ");
  Serial.print(output);

  Serial.print(", setpoint ");
  Serial.println(setpoint);

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = error;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  avgError = fabs(total / numReadings);
  // send it to the computer as ASCII digits

  if (avgError  <0.5) {
    //esc.writeMicroseconds(1500);
    Serial.print("Hooray! Now enter new angle: ");
    //delay(1000);
    while (Serial.available() == 0) {}
    // //String input = Serial.readStringUntil('\n');
    // //setpoint = input.toDouble();
    setpoint = Serial.parseFloat();
    Serial.print("New setpoint: ");
    Serial.println(setpoint);
    while (Serial.available() > 0) {
    Serial.read();
    }
    // setpoint = setpoint + 90;
    // if (setpoint > 360) {
    //   setpoint = 90;
    // }
  }

  if (output > 100 || output < -100) {
    integral = 0;
    // derivative=0;
    // pTerm=0;
  }

  // Update previous error
  prevError = error;

 
  
}
