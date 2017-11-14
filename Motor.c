const int RPM_SENSOR = 10;
const int MOTOR = 9;
const int SAMPLES = 50;

int MAX_FREQ = 80;

unsigned long valueHigh, valueLow;

int frequency, frequencyAverage, value;

int motorSpeed;
int desiredSpeed = 32;

boolean calibrated = false;
int i = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(RPM_SENSOR, INPUT);  
  pinMode(MOTOR, OUTPUT); 
}

void loop() {
  if(!calibrated) {
    if(i < SAMPLES) {
      measure();

      if(frequency > 0) {
        Serial.print("Calibrating: ");
        Serial.println(frequency);
    
        frequencyAverage += map(frequency, 0, MAX_FREQ, 0, 127);
         
        i++;
      }
      
    } else {
      frequencyAverage = frequencyAverage / SAMPLES;
 
      if(frequencyAverage != desiredSpeed) {
        MAX_FREQ *= (float)desiredSpeed / (float)frequencyAverage;
      }

      Serial.print("Average: ");
      Serial.println(frequencyAverage);

      calibrated = true;
    }    

    analogWrite(MOTOR, desiredSpeed);
  } else {
    measure();
    
    value = map(frequency, 0, MAX_FREQ, 0, 127);
    motorSpeed = 2 * desiredSpeed - value;
    
    Serial.print("RPM: ");
    Serial.println(value * 60);
    
    analogWrite(MOTOR, motorSpeed);
  }
}

void measure() {
    valueLow  = pulseIn(RPM_SENSOR, LOW);
    valueHigh = pulseIn(RPM_SENSOR, HIGH);
    
    frequency = 1000000 / (2 * (valueHigh + valueLow));

    if(frequency < 0) {
      frequency = 0;
    }
    
    if(frequency > MAX_FREQ) {
      frequency = MAX_FREQ;
    }
}