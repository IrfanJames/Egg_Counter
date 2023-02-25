const int IR_Sensor_Count = 1; //3;
int IR[IR_Sensor_Count] = {32}; // 34, 35};
const unsigned long one_EGG_Time_ms = 6000;
unsigned long time_Up = millis();
int egg_Count = 0;

bool Sense();
bool Count();

bool edge = false;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < IR_Sensor_Count; i++)
    pinMode(IR[i], INPUT);
}

void loop() {
  Serial.printf("\nEgg Count: %d\t", egg_Count);
  Serial.print(millis());
  Serial.print(" - ");
  Serial.print(time_Up);
  Serial.print(" = ");
  Serial.print(millis() - time_Up);
  
  if (Sense() == false) {
    if (edge == true) {
      Count();
      Serial.print("\tObject Passed");
    }
    else
      Serial.print("\tNo Object Detected");
    edge = false;
  }
  else {
    if (edge == false) {
      time_Up = millis();
      Serial.print("\tObject Detected");
    }
    else    
      Serial.print("\tObject Passing");
    
    edge = true;
  }
}


bool Sense() {
  // Check through every IR Sensor
  // Returns 1 if even one of the sensors deect an object

  for (int i = 0; i < IR_Sensor_Count; i++)
    if (digitalRead(IR[i]) == LOW)
      return 1;
  
  return 0;
}

bool Count() {
  // This Functions gets evoked when no object is detected
  // If the past HIGH-Time (time when sensors constantly detected an object) is
  // long enough (~one_EGG_Time_ms) the egg_Count increments

  // If the past HIGH-Time (time when sensors constantly detected an object) is
  // twice the (~one_EGG_Time_ms * k) the egg_Count incremented by k
  
  unsigned long time = millis() - time_Up;
  Serial.print("\t In Count: ");
  Serial.print(time);

  float margin = 0.1; // 0.1 == 10%
  float k = (double)time / (double)one_EGG_Time_ms;
  
  Serial.print("\t, k: ");
  Serial.print(k);
  Serial.print(", int(k): ");
  Serial.print(int(k));
  
  if (k < abs(1.0f - margin))
    return 0;
  else {
    egg_Count += (int)abs(k); // (int)k = no 0f eggs passed
    return 1;
  }
}