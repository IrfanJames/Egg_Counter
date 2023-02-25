const int NUM_SENSORS = 15;
const int SENSOR_PINS[NUM_SENSORS] = {1, 2, 3, 4,5,6,7,8,9,10,11,12,13,14,15/* ... */ };
unsigned long ONE_EGG_TIME_MS = 2000;
unsigned long MARGIN_US = 200000;
volatile int eggCount = 0;

class Sensor {
  public:
    Sensor(int pin) : pin_(pin), timeUp_(0) {}
    void handle() {
      static bool objectDetected = false;
      bool newDetection = !digitalRead(pin_);

      // Debounce logic to ignore false positives
      static unsigned long lastDetectionTime = 0;
      unsigned long currentDetectionTime = millis();
      if (newDetection && !objectDetected && (currentDetectionTime - lastDetectionTime) > 50) {
        // Object detected
        timeUp_ = micros();
        Serial.print("\tObject Detected");
      } else if (!newDetection && objectDetected) {
        // Object passed
        unsigned long egg_time_us = micros() - timeUp_;
        Serial.print("\tObject Passed, egg time: ");
        Serial.print(egg_time_us);

        // Update average egg time
        avgEggTimeMs = (avgEggTimeMs * eggCount + egg_time_us / 1000) / (eggCount + 1);

        // Adjust ONE_EGG_TIME_MS and MARGIN_US if necessary
        unsigned long margin_adjustment = 0;
        if (egg_time_us > (ONE_EGG_TIME_MS + MARGIN_US)) {
          // Eggs are taking too long, reduce ONE_EGG_TIME_MS and MARGIN_US by 5%
          ONE_EGG_TIME_MS *= 0.95;
          MARGIN_US *= 0.95;
          margin_adjustment = 1;
        } else if (egg_time_us < (ONE_EGG_TIME_MS - MARGIN_US)) {
          // Eggs are too fast, increase ONE_EGG_TIME_MS and MARGIN_US by 5%
          ONE_EGG_TIME_MS *= 1.05;
          MARGIN_US *= 1.05;
          margin_adjustment = -1;
        }

        eggCount++;
        Serial.print(", egg count: ");
        Serial.print(eggCount);
        Serial.print(", avg egg time: ");
        Serial.print(avgEggTimeMs);
        Serial.print(", ONE_EGG_TIME_MS: ");
        Serial.print(ONE_EGG_TIME_MS);
        Serial.print(", MARGIN_US: ");
        Serial.print(MARGIN_US);
        Serial.print(", margin adjustment: ");
        Serial.print(margin_adjustment);
      }
      objectDetected = newDetection;
      lastDetectionTime = currentDetectionTime;
    }
  private:
    int pin_;
    unsigned long timeUp_;
};

Sensor sensors[NUM_SENSORS] = {
  Sensor(SENSOR_PINS[0]),
  Sensor(SENSOR_PINS[1]),
  Sensor(SENSOR_PINS[2]),
  Sensor(SENSOR_PINS[3]),
  Sensor(SENSOR_PINS[4]),
  Sensor(SENSOR_PINS[5]),
  Sensor(SENSOR_PINS[6]),
  Sensor(SENSOR_PINS[7]),
  Sensor(SENSOR_PINS[8]),
  Sensor(SENSOR_PINS[9]),
  Sensor(SENSOR_PINS[10]),
  Sensor(SENSOR_PINS[11]),
  Sensor(SENSOR_PINS[12]),
  Sensor(SENSOR_PINS[13]),
  Sensor(SENSOR_PINS[14])
  // ... and so on for the rest of the sensors
};

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_SENSORS; i++) {
    attachInterrupt(digitalPinToInterrupt(sensors[i].pin_), sensors[i].handle, CHANGE);
  }
}

void loop() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensors[i].handle();
  }
  
  Serial.printf("\nEgg Count: %d\t", eggCount);
}
