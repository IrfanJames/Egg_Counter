#define Deg_To_Rad 0.0174533f
#define Rad_To_Deg 57.2958f

const int IR_Sensor_Count = 3;
int IR[IR_Sensor_Count] = {32, 34, 35};
const float one_EGG_Time_ms = 1500;
unsigned long time_Up = 0;
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
  Serial.printf("Egg Count: %d\t", egg_Count);
  
  if (Sense() == false) {
    if (edge == true) {
      Count();
      Serial.print("\tObject Passed");
    }

    edge = false;
    Serial.print("\tNo Object Detected");
  }
  else {
    if (edge == false) {
      time_Up = millis();
      Serial.print("\tObject Detected");
    }

    edge = true;
    Serial.print("\tObject Passing");
  }
}


bool Sense() {
  // Check through every IR Sensor
  // Returns 1 if even one of the sensors deect an object

  for (int i = 0; i < IR_Sensor_Count; i++)
    if (analogRead(IR[i] == HIGH)) return 1;
  
  return 0;
}

bool Count() {
  // This Functions gets evoked no object is detected
  // If the past HIGH-Time (time when sensors constantly detected an object) is
  // long enough (~one_EGG_Time_ms) the egg_Count increments

  // If the past HIGH-Time (time when sensors constantly detected an object) is
  // twice the (~one_EGG_Time_ms * k) the egg_Count incremented by k
  
  float margin = 0.3; // 0.1 == 10%
  float k = (time_Up - one_EGG_Time_ms) / one_EGG_Time_ms;
  
  if (k < abs(1 - margin))
    return 0;
  else {
    egg_Count =+ (int)k; // (int)k = no 0f eggs passed
    return 1;
  }
}
