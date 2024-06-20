const int motor1Enable = 9;
const int motor2Enable = 10; // Enable pin for Motor (EN-B)
const int motor1Pin1 = 2;
const int motor1Pin2 = 3;
const int motor2Pin1 = 5; // Control pin 1 for Motor (IN3)
const int motor2Pin2 = 4; // Control pin 2 for Motor (IN4)
const int tempPin = A0;

float fanSpeed = 0;    // Variable to store the speed of Fan 2
float setPoint = 70.0; // Desired temperature setpoint for the hot side of Peltier

// PI constants
float Kp = 10.0; // Proportional gain
float Ki = 0.1;  // Integral gain

float integralTerm = 0.0;
float lastError = 0.0;

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor1Enable, OUTPUT);
  pinMode(motor2Enable, OUTPUT);

  digitalWrite(motor1Enable, LOW);
  digitalWrite(motor2Enable, LOW);
  Serial.begin(9600); // Use higher baud rate for faster data transmission

  // Print headers for SerialPlot
  Serial.println("TemperatureC,SetPoint,PWMValue");
}

void loop() {
  int analogValue = analogRead(tempPin);
  float voltage = analogValue * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0 + 30;

  // Calculate error
  float error = temperatureC - setPoint;

  if (temperatureC > setPoint) {
    // Set fan speed to maximum if temperature exceeds setpoint
    fanSpeed = 255;
  } else {
    // Proportional term
    float proportionalTerm = Kp * error;

    // Integral term
    integralTerm += Ki * error;

    // Apply constraints to integral term to prevent windup
    integralTerm = constrain(integralTerm, -255.0 / Ki, 255.0 / Ki);

    // Calculate fan speed based on PI control
    fanSpeed = proportionalTerm + integralTerm;

    // Ensure fan speed does not exceed maximum PWM value
    fanSpeed = constrain(fanSpeed, 0, 255);
  }

  // Apply PWM to enable pins
  int pwmValue = fanSpeed;
  analogWrite(motor1Enable, pwmValue);
  analogWrite(motor2Enable, pwmValue);

  // Determine direction based on fanSpeed sign
  if (fanSpeed > 0) {
    digitalWrite(motor1Pin1, HIGH); // Example: Set motor direction
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  } else {
    digitalWrite(motor1Pin1, LOW); // Turn off motors if fanSpeed is 0
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }

  // Output to SerialPlot
  Serial.print(temperatureC); // Current temperature
  Serial.print(" ");
  Serial.print(setPoint); // Setpoint temperature
  Serial.print(" ");
  Serial.println(pwmValue); // PWM value

  delay(1000); // Adjust delay as needed for control loop timing
}
