const int motor1Enable = 9;
const int motor2Enable = 10; // Enable pin for Motor (EN-B)
const int motor1Pin1 = 2;
const int motor1Pin2 = 3;
const int motor2Pin1 = 5; // Control pin 1 for Motor (IN3)
const int motor2Pin2 = 4; // Control pin 2 for Motor (IN4)
const int tempPin = A0;
int fanSpeed = 0; // Variable to store the speed of Fan 2
int setPoint = 0;
 Comment
 Suggest edit
 Edit from here

void setup() {
pinMode(motor1Pin1, OUTPUT);
pinMode(motor1Pin2, OUTPUT);
pinMode(motor2Pin1, OUTPUT);
pinMode(motor2Pin2, OUTPUT);
pinMode(motor1Enable, OUTPUT);
pinMode(motor2Enable, OUTPUT);

digitalWrite(motor1Enable, LOW);
digitalWrite(motor2Enable, LOW);
Serial.begin(9600);
}

void loop() {
int analogValue = analogRead(tempPin);
float voltage = analogValue * (5.0 / 1023.0);
float temperatureC = (voltage - 0.5) * 100.0 + 30;

// Control the fan speed based on temperature
if (temperatureC > 60) {
fanSpeed = min(fanSpeed + 2, 255); // Increase to full power
setPoint = 255;
}
else if (temperatureC > 35 && temperatureC <= 60) { // Increase to half power
fanSpeed = min(fanSpeed + 2, 128);
setPoint = 128;
}
else if (temperatureC <= 35) {
fanSpeed = max(fanSpeed - 2, 0); // Decrease to off
setPoint = 0;
}

Serial.print(temperatureC);
Serial.print(" ");
Serial.print(fanSpeed);
Serial.print(" ");
Serial.println(setPoint);

analogWrite(motor1Enable, fanSpeed);
analogWrite(motor2Enable, fanSpeed);

if (fanSpeed > 0) {
digitalWrite(motor1Pin1, HIGH);
digitalWrite(motor1Pin2, LOW);
digitalWrite(motor2Pin1, HIGH);
digitalWrite(motor2Pin2, LOW);
} else {
digitalWrite(motor1Pin1, LOW);
digitalWrite(motor1Pin2, LOW);
digitalWrite(motor2Pin1, LOW);
digitalWrite(motor2Pin2, LOW);
}
delay(500);
}
